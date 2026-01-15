#!/usr/bin/make -f
# scan.mk - Static analysis tools for C/C++ projects

# Configuration
REPORT_DIR ?= report
NPROCS ?= $(shell grep -c 'processor' /proc/cpuinfo 2>/dev/null || echo 4)
SCAN_CFLAGS ?= $(CXXFLAGS)
SCAN_INCLUDES ?= -I include

# Clang toolchain parameters
CLANG_CC ?= clang
CLANG_CXX ?= clang++
CLANG_TIDY ?= clang-tidy
CLANG_CHECK ?= clang-check
CLANG_FORMAT ?= clang-format
SCAN_BUILD ?= scan-build-11
IWYU ?= include-what-you-use

# Additional checks, apart from .clang-tidy
CLANG_TIDY_CHECKS ?=

# cppcheck - C/C++ static analyzer
.PHONY: scan-cppcheck
scan-cppcheck:
	@mkdir -p $(REPORT_DIR)
	cppcheck --enable=all --xml --xml-version=2 \
		--suppress=missingIncludeSystem \
		--language=c --std=c99 \
		$(SCAN_INCLUDES) \
		src/ 2> $(REPORT_DIR)/cppcheck-report.xml

# Generate compilation database for clang tools
.PHONY: scan-compile-commands
scan-compile-commands:
	@mkdir -p $(REPORT_DIR)
	@echo "Generating compilation database..."
	@$(MAKE) clean >/dev/null 2>&1 || true
	bear -- $(MAKE) $(PROG) 2>&1 | tee $(REPORT_DIR)/bear-build.log || \
		echo "Warning: bear not installed, clang-tidy may have limited functionality"

# clang-tidy - Linter and static checker
.PHONY: scan-clang-tidy
scan-clang-tidy:
	@mkdir -p $(REPORT_DIR)
	@echo "Running clang-tidy..."
	@find src -name "*.c" -o -name "*.cpp" | while read file; do \
		$(CLANG_TIDY) "$$file" \
			-checks='$(CLANG_TIDY_CHECKS)' \
			-- $(CLANG_COMPATIBLE_FLAGS) $(SCAN_INCLUDES) \
			2>&1 || true; \
	done | tee $(REPORT_DIR)/clang-tidy-report.txt

# clang static analyzer (scan-build) - Deep static analysis
.PHONY: scan-clang-analyzer
scan-clang-analyzer:
	@mkdir -p $(REPORT_DIR)/scan-build
	@$(MAKE) clean >/dev/null 2>&1 || true
	$(SCAN_BUILD) --use-cc=$(CLANG_CC) --use-c++=$(CLANG_CXX) \
		--status-bugs -o $(REPORT_DIR)/scan-build -plist-html \
		$(MAKE) $(PROG) 2>&1 | tee $(REPORT_DIR)/scan-build-report.txt

# clang-check - Syntax and semantic analysis
.PHONY: scan-clang-check
scan-clang-check:
	@mkdir -p $(REPORT_DIR)
	find src -name "*.c" -o -name "*.cpp" | \
		xargs $(CLANG_CHECK) -analyze \
		-- $(SCAN_CFLAGS) $(SCAN_INCLUDES) \
		2>&1 | tee $(REPORT_DIR)/clang-check-report.txt || true

# clang-format - Code formatting check
.PHONY: scan-clang-format
scan-clang-format:
	@mkdir -p $(REPORT_DIR)
	find src -name "*.c" -o -name "*.h" -o -name "*.cpp" | \
		xargs $(CLANG_FORMAT) -style=file --dry-run --Werror \
		2>&1 | tee $(REPORT_DIR)/clang-format-report.txt || true

# include-what-you-use - Header dependency analysis
.PHONY: scan-iwyu
scan-iwyu:
	@mkdir -p $(REPORT_DIR)
	find src -name "*.c" -o -name "*.cpp" | \
		xargs -I {} $(IWYU) {} $(SCAN_CFLAGS) $(SCAN_INCLUDES) \
		2>&1 | tee $(REPORT_DIR)/iwyu-report.txt || true

# infer - Facebook's static analyzer
.PHONY: scan-infer
scan-infer:
	@mkdir -p $(REPORT_DIR)
	@$(MAKE) clean >/dev/null 2>&1 || true
	infer run --report-json $(REPORT_DIR)/infer-report.json \
		-- $(MAKE) $(PROG) 2>&1 | tee $(REPORT_DIR)/infer-console.log

# flawfinder - Security scanner for C/C++
.PHONY: scan-flawfinder
scan-flawfinder:
	@mkdir -p $(REPORT_DIR)
	flawfinder --html --context --minlevel=0 src/ \
		> $(REPORT_DIR)/flawfinder-report.html 2>&1

# splint - Secure programming lint
.PHONY: scan-splint
scan-splint:
	@mkdir -p $(REPORT_DIR)
	find src -name "*.c" | \
		xargs splint +posixlib $(SCAN_INCLUDES) \
		2>&1 | tee $(REPORT_DIR)/splint-report.txt || true

# oclint - Static code analysis tool
.PHONY: scan-oclint
scan-oclint:
	@mkdir -p $(REPORT_DIR)
	@$(MAKE) clean >/dev/null 2>&1 || true
	oclint-json-compilation-database -- \
		-report-type html -o $(REPORT_DIR)/oclint-report.html \
		2>&1 | tee $(REPORT_DIR)/oclint-console.log || true

# pmccabe - Cyclomatic complexity analyzer
.PHONY: scan-complexity
scan-complexity:
	@mkdir -p $(REPORT_DIR)
	find src -name "*.c" | xargs pmccabe \
		> $(REPORT_DIR)/complexity-report.txt 2>&1

# lizard - Code complexity analyzer (requires Python)
.PHONY: scan-lizard
scan-lizard:
	@mkdir -p $(REPORT_DIR)
	lizard src/ --xml > $(REPORT_DIR)/lizard-report.xml 2>&1

# compiler warnings - Capture GCC/Clang warnings
.PHONY: scan-compiler-warnings
scan-compiler-warnings:
	@mkdir -p $(REPORT_DIR)
	@$(MAKE) clean >/dev/null 2>&1 || true
	$(MAKE) CC=$(CLANG_CC) $(PROG) \
		2>&1 | tee $(REPORT_DIR)/compiler-warnings.txt || true

# address sanitizer - Memory error detector
.PHONY: scan-asan
scan-asan:
	@mkdir -p $(REPORT_DIR)
	@$(MAKE) clean >/dev/null 2>&1 || true
	$(MAKE) CC=$(CLANG_CC) \
		CXXFLAGS="$(CXXFLAGS) -fsanitize=address -fno-omit-frame-pointer -g" \
		$(PROG)
	ASAN_OPTIONS=log_path=$(REPORT_DIR)/asan.log \
		SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy \
		timeout 5 ./$(PROG) || [ $$? -eq 124 ]

# memory sanitizer - Uninitialized memory detector
.PHONY: scan-msan
scan-msan:
	@mkdir -p $(REPORT_DIR)
	@$(MAKE) clean >/dev/null 2>&1 || true
	$(MAKE) CC=$(CLANG_CC) \
		CXXFLAGS="$(CXXFLAGS) -fsanitize=memory -fno-omit-frame-pointer -g" \
		$(PROG)
	MSAN_OPTIONS=log_path=$(REPORT_DIR)/msan.log \
		SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy \
		timeout 5 ./$(PROG) || [ $$? -eq 124 ]

# undefined behavior sanitizer - UB detector
.PHONY: scan-ubsan
scan-ubsan:
	@mkdir -p $(REPORT_DIR)
	@$(MAKE) clean >/dev/null 2>&1 || true
	$(MAKE) CC=$(CLANG_CC) \
		CXXFLAGS="$(CXXFLAGS) -fsanitize=undefined -fno-omit-frame-pointer -g" \
		$(PROG)
	UBSAN_OPTIONS=log_path=$(REPORT_DIR)/ubsan.log \
		SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy \
		timeout 5 ./$(PROG) || [ $$? -eq 124 ]

# thread sanitizer - Data race detector
.PHONY: scan-tsan
scan-tsan:
	@mkdir -p $(REPORT_DIR)
	@$(MAKE) clean >/dev/null 2>&1 || true
	$(MAKE) CC=$(CLANG_CC) \
		CXXFLAGS="$(CXXFLAGS) -fsanitize=thread -fno-omit-frame-pointer -g" \
		$(PROG)
	TSAN_OPTIONS=log_path=$(REPORT_DIR)/tsan.log \
		SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy \
		timeout 5 ./$(PROG) || [ $$? -eq 124 ]

# valgrind - Memory debugger
.PHONY: scan-valgrind
scan-valgrind:
	@mkdir -p $(REPORT_DIR)
	SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy \
		valgrind --leak-check=full --xml=yes \
		--xml-file=$(REPORT_DIR)/valgrind-report.xml \
		--suppressions=valgrind.supp \
		timeout 5 ./$(PROG) || [ $$? -eq 124 ]

# dr. memory - Memory debugger (commented out - use sanitizers instead)
# .PHONY: scan-drmemory
# scan-drmemory:
# 	@mkdir -p $(REPORT_DIR)
# 	SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy \
# 		drmemory -batch -results_to_stderr \
# 		-logdir $(REPORT_DIR)/drmemory \
# 		-- ./$(PROG) 2>&1 | tee $(REPORT_DIR)/drmemory-report.txt || true

# coverage - Code coverage analysis (requires gcov build)
.PHONY: scan-coverage
scan-coverage:
	@mkdir -p $(REPORT_DIR)
	@$(MAKE) clean >/dev/null 2>&1 || true
	$(MAKE) BUILD_TYPE=coverage $(PROG)
	SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy \
		timeout 5 ./$(PROG) || [ $$? -eq 124 ]
	gcovr -r . --xml -o $(REPORT_DIR)/coverage.xml
	gcovr -r . --html --html-details -o $(REPORT_DIR)/coverage.html

# coverage with ASAN - Address sanitizer coverage
.PHONY: scan-coverage-asan
scan-coverage-asan:
	@mkdir -p $(REPORT_DIR)
	@$(MAKE) clean >/dev/null 2>&1 || true
	$(MAKE) CC=$(CLANG_CC) BUILD_TYPE=coverage \
		CXXFLAGS="$(CXXFLAGS) -fprofile-arcs -ftest-coverage -fsanitize=address -fno-omit-frame-pointer -g" \
		LDFLAGS="$(LDFLAGS) -lgcov --coverage" \
		$(PROG)
	ASAN_OPTIONS=log_path=$(REPORT_DIR)/asan-coverage.log \
		SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy \
		timeout 5 ./$(PROG) || [ $$? -eq 124 ]
	gcovr -r . --xml -o $(REPORT_DIR)/coverage-asan.xml
	gcovr -r . --html --html-details -o $(REPORT_DIR)/coverage-asan.html

# coverage with UBSAN - Undefined behavior sanitizer coverage
.PHONY: scan-coverage-ubsan
scan-coverage-ubsan:
	@mkdir -p $(REPORT_DIR)
	@$(MAKE) clean >/dev/null 2>&1 || true
	$(MAKE) CC=$(CLANG_CC) BUILD_TYPE=coverage \
		CXXFLAGS="$(CXXFLAGS) -fprofile-arcs -ftest-coverage -fsanitize=undefined -fno-omit-frame-pointer -g" \
		LDFLAGS="$(LDFLAGS) -lgcov --coverage" \
		$(PROG)
	UBSAN_OPTIONS=log_path=$(REPORT_DIR)/ubsan-coverage.log \
		SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy \
		timeout 5 ./$(PROG) || [ $$? -eq 124 ]
	gcovr -r . --xml -o $(REPORT_DIR)/coverage-ubsan.xml
	gcovr -r . --html --html-details -o $(REPORT_DIR)/coverage-ubsan.html

# coverage with MSAN - Memory sanitizer coverage
.PHONY: scan-coverage-msan
scan-coverage-msan:
	@mkdir -p $(REPORT_DIR)
	@$(MAKE) clean >/dev/null 2>&1 || true
	$(MAKE) CC=$(CLANG_CC) BUILD_TYPE=coverage \
		CXXFLAGS="$(CXXFLAGS) -fprofile-arcs -ftest-coverage -fsanitize=memory -fno-omit-frame-pointer -g" \
		LDFLAGS="$(LDFLAGS) -lgcov --coverage" \
		$(PROG)
	MSAN_OPTIONS=log_path=$(REPORT_DIR)/msan-coverage.log \
		SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy \
		timeout 5 ./$(PROG) || [ $$? -eq 124 ]
	gcovr -r . --xml -o $(REPORT_DIR)/coverage-msan.xml
	gcovr -r . --html --html-details -o $(REPORT_DIR)/coverage-msan.html

# coverage with TSAN - Thread sanitizer coverage
.PHONY: scan-coverage-tsan
scan-coverage-tsan:
	@mkdir -p $(REPORT_DIR)
	@$(MAKE) clean >/dev/null 2>&1 || true
	$(MAKE) CC=$(CLANG_CC) BUILD_TYPE=coverage \
		CXXFLAGS="$(CXXFLAGS) -fprofile-arcs -ftest-coverage -fsanitize=thread -fno-omit-frame-pointer -g" \
		LDFLAGS="$(LDFLAGS) -lgcov --coverage" \
		$(PROG)
	TSAN_OPTIONS=log_path=$(REPORT_DIR)/tsan-coverage.log \
		SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy \
		timeout 5 ./$(PROG) || [ $$? -eq 124 ]
	gcovr -r . --xml -o $(REPORT_DIR)/coverage-tsan.xml
	gcovr -r . --html --html-details -o $(REPORT_DIR)/coverage-tsan.html

# combined coverage - Merge all sanitizer coverage reports
.PHONY: scan-coverage-all
scan-coverage-all: scan-coverage scan-coverage-asan scan-coverage-ubsan
	@echo "Merged coverage reports generated in $(REPORT_DIR)/"

# cpplint - Google's C++ style checker
.PHONY: scan-cpplint
scan-cpplint:
	@mkdir -p $(REPORT_DIR)
	find src -name "*.c" -o -name "*.cpp" -o -name "*.h" | \
		xargs cpplint --output=eclipse \
		2>&1 | tee $(REPORT_DIR)/cpplint-report.txt || true

# cppncss - Function metrics and complexity
.PHONY: scan-cppncss
scan-cppncss:
	@mkdir -p $(REPORT_DIR)
	cppncss -r -v -x -k -f=report/cppncss-report.xml src/ || true

# Combined static analysis
.PHONY: scan-static
scan-static: scan-cppcheck scan-clang-tidy scan-clang-analyzer scan-flawfinder

# Combined runtime analysis
.PHONY: scan-runtime
scan-runtime: scan-asan scan-ubsan scan-valgrind

# All scans
.PHONY: scan-all
scan-all: scan-static scan-runtime scan-coverage

# SonarQube scan - Run all supported analyzers for sonar-cxx plugin
.PHONY: scan-sonar
scan-sonar: scan-cppcheck scan-clang-tidy scan-clang-analyzer scan-compiler-warnings scan-iwyu scan-valgrind scan-coverage
	@echo "Running SonarQube scanner with all reports..."
	sonar-scanner -Dsonar.token=$(SONAR_TOKEN) -Dsonar.host.url=$(SONAR_HOST_URL)

# Clean reports
.PHONY: scan-clean
scan-clean:
	rm -rf $(REPORT_DIR)

.PHONY: scan-help
scan-help:
	@echo "Available scan targets:"
	@echo "  scan-compile-commands  - Generate compile_commands.json (for clang-tidy)"
	@echo "  scan-cppcheck          - Run cppcheck static analyzer"
	@echo "  scan-clang-tidy        - Run clang-tidy linter"
	@echo "  scan-clang-analyzer    - Run clang static analyzer (scan-build)"
	@echo "  scan-clang-check       - Run clang-check"
	@echo "  scan-clang-format      - Check code formatting"
	@echo "  scan-iwyu              - Check header dependencies"
	@echo "  scan-infer             - Run Facebook Infer"
	@echo "  scan-flawfinder        - Run security scanner"
	@echo "  scan-splint            - Run splint"
	@echo "  scan-oclint            - Run OCLint"
	@echo "  scan-cpplint           - Run Google cpplint"
	@echo "  scan-complexity        - Analyze cyclomatic complexity"
	@echo "  scan-lizard            - Run Lizard complexity analyzer"
	@echo "  scan-cppncss           - Run function metrics"
	@echo "  scan-compiler-warnings - Capture compiler warnings"
	@echo "  scan-asan              - Run with Address Sanitizer"
	@echo "  scan-msan              - Run with Memory Sanitizer"
	@echo "  scan-ubsan             - Run with UB Sanitizer"
	@echo "  scan-tsan              - Run with Thread Sanitizer"
	@echo "  scan-valgrind          - Run Valgrind"
	@echo "  scan-coverage          - Generate code coverage (GCC/gcov)"
	@echo "  scan-coverage-asan     - Coverage with AddressSanitizer"
	@echo "  scan-coverage-ubsan    - Coverage with UBSanitizer"
	@echo "  scan-coverage-msan     - Coverage with MemorySanitizer"
	@echo "  scan-coverage-tsan     - Coverage with ThreadSanitizer"
	@echo "  scan-coverage-all      - Run all coverage variants"
	@echo "  scan-static            - Run all static analyzers"
	@echo "  scan-runtime           - Run all runtime analyzers"
	@echo "  scan-all               - Run all scans"
	@echo "  scan-sonar             - Run SonarQube analysis (all supported tools)"
	@echo "  scan-clean             - Clean all reports"
	@echo ""
	@echo "Configure with:"
	@echo "  CLANG_CC=clang-17      - Set clang C compiler"
	@echo "  CLANG_CXX=clang++-17   - Set clang C++ compiler"
	@echo "  REPORT_DIR=my-reports  - Set report directory"
	@echo ""
	@echo "Note: For best clang-tidy results, install 'bear' and run:"
	@echo "  make scan-compile-commands  # Generates compile_commands.json"
	@echo "  make scan-clang-tidy        # Uses compilation database"
