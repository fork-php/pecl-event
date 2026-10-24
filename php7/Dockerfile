FROM php:7.4.33-cli

# libssl1.1 is no longer available on deb.debian.org. Try to download it from the archive.
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

RUN mkdir -p /usr/src/pecl-event
WORKDIR /usr/src/pecl-event

COPY . .

RUN ./rebuild.sh

ENV NO_INTERACTION=1
CMD ["./run-tests.sh"]

# vim: ft=dockerfile
