# syntax=docker/dockerfile:1
FROM debian:trixie-slim AS ci

ARG SONAR_SCANNER_VERSION=8.0.1.6346-1
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -yq --no-install-recommends \
        ca-certificates curl git make jq unzip pkgconf \
        default-jre-headless \
        gcc g++ clang clang-tidy clang-tools cppcheck gcovr valgrind bear flawfinder \
        shellcheck libsdl2-dev libsdl2-mixer-dev libsdl2-image-dev libsdl2-ttf-dev \
    && rm -rf /var/lib/apt/lists/*

ADD https://master.debci.antonialoytorrens.com/key.gpg /etc/apt/keyrings/master.debci.antonialoytorrens.com.gpg
RUN chmod 644 /etc/apt/keyrings/master.debci.antonialoytorrens.com.gpg \
    && printf 'deb [signed-by=/etc/apt/keyrings/master.debci.antonialoytorrens.com.gpg] https://master.debci.antonialoytorrens.com/debian trixie main\n' \
       > /etc/apt/sources.list.d/master.debci.antonialoytorrens.com.list

ADD https://master.debci.antonialoytorrens.com/debian/pool/main/s/sonar-scanner/sonar-scanner_8.0.1.6346-1~aatdeb13u1_all.deb /tmp/sonar-scanner.deb
RUN dpkg -i /tmp/sonar-scanner.deb || (apt-get update && apt-get install -yqf) \
    && rm -f /tmp/sonar-scanner.deb \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

RUN useradd --create-home --shell /bin/bash builder \
	&& mkdir -p /workspace \
	&& chown builder:builder /workspace

USER builder
WORKDIR /workspace
CMD ["bash"]
