VERSION = 1.0
REVISION = 1

OUT = bin

DEPS += src/structs.h
DEPS += src/common.h
DEPS += src/defs.h

GAME_SOURCES := $(shell find src/ -mindepth 2 -name '*.c' -not -path 'src/plat/*' | sort)

OBJS := $(addprefix $(OUT)/,$(GAME_SOURCES:%.c=%.o))

GAME_OBJS = $(OBJS) $(OUT)/src/main.o

MAP_OBJS = $(OBJS) $(OUT)/src/mapEditor.o

RAW_CREDITS = raw/credits.txt

GENERATED_CREDITS = data/misc/credits.txt

all: $(PROG) $(MAP_PROG) $(GENERATED_CREDITS)

$(OUT)/%.o: %.c %.h $(DEPS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CXXFLAGS) -c -o $@ $<

clean:
	$(RM) -rf $(OUT) $(PROG) $(PROG).wasm $(MAP_PROG) index.html index.js data/misc/credits.txt $(LOCALE_MO)