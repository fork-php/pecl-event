FROM php:7.4.33-cli

ARG SANITIZER=""
ENV SANITIZER="${SANITIZER}"

# libssl1.1_1.1.1w-0+deb11u8_arm64.deb disappeared from the normal deb.debian.org/debian-security location.
# Try to download it from the archive.
RUN sed -i \
        -e 's|http://deb.debian.org/debian-security|http://archive.debian.org/debian-security|g' \
        -e 's|http://security.debian.org/debian-security|http://archive.debian.org/debian-security|g' \
        -e 's|http://deb.debian.org/debian|http://archive.debian.org/debian|g' \
        -e '/bullseye-updates/d' \
        /etc/apt/sources.list \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
        libssl1.1 \
        libssl-dev \
        libevent-dev \
    && rm -rf /var/lib/apt/lists/*

RUN docker-php-ext-install sockets posix pcntl \
    && docker-php-ext-enable --ini-name 0-sockets.ini sockets \
    && docker-php-ext-enable posix pcntl

WORKDIR /usr/src/pecl-event
COPY . .

RUN ./ci/setup-php-docker.sh
RUN ./ci/build.sh
RUN docker-php-ext-enable --ini-name z-event.ini event

ENV NO_INTERACTION=1
CMD ["./ci/test.sh"]
