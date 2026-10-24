FROM php:5.6-cli

RUN apt-get update && apt-get install -y libssl1.1 libssl-dev libevent-dev


WORKDIR /usr/src/pecl-event
COPY . .

RUN ./ci/setup-php-docker.sh
RUN ./ci/build.sh
RUN docker-php-ext-enable --ini-name z-event.ini event

ENV NO_INTERACTION=1
CMD ["./ci/test.sh"]

# vim: ft=dockerfile
