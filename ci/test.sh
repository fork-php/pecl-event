#!/bin/sh -

set -eux

root="$(cd "$(dirname "$0")/.." || exit 1; pwd)"
cd "${root}" || exit 1

case "${SANITIZER:-none}" in
    none)
        ;;
    asan)
        export ASAN_OPTIONS="${ASAN_OPTIONS:-detect_leaks=1:abort_on_error=1}"
        ;;
    *)
        echo "Unknown sanitizer: ${SANITIZER}" >&2
        exit 1
        ;;
esac

NO_INTERACTION=1 "${root}/run-tests.sh"
