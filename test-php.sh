#!/bin/sh

set -eu

dir="$(cd "$(dirname "$0")" && pwd)"
local_libs_dir="${dir}/.libs"

if [ ! -e "${local_libs_dir}/sockets.so" ]; then
    sockets_so_path="$(php-config --extension-dir)/sockets.so"
    cp "$sockets_so_path" "${local_libs_dir}"
fi

if [ "${SANITIZER:-none}" = "asan" ]; then
    libasan="$(gcc -print-file-name=libasan.so)"

    exec env \
        LD_PRELOAD="$libasan" \
        ASAN_OPTIONS="${ASAN_OPTIONS:-detect_leaks=0:abort_on_error=1}" \
        php -n \
            -d extension=sockets.so \
            -d extension=event.so \
            -d extension_dir="${local_libs_dir}" \
            "$@"
else
    exec php -n \
        -d extension=sockets.so \
        -d extension=event.so \
        -d extension_dir="${local_libs_dir}" \
        "$@"
fi
