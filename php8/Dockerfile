FROM php:8-cli

ARG SANITIZER=""
ENV SANITIZER="${SANITIZER}"

RUN apt-get update \
    && apt-get install -y libevent-dev libssl-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /usr/src/pecl-event
COPY . .

RUN ./ci/setup-php-docker.sh
RUN ./ci/build.sh
RUN docker-php-ext-enable --ini-name z-event.ini event

ENV NO_INTERACTION=1
CMD ["./ci/test.sh"]
