Event - PECL extension
======================

Event is a PECL extension providing interface to `libevent` C library.

> [!IMPORTANT]
> **Repository moved to GitHub**
>
> The canonical repository for pecl-event is now hosted at <https://github.com/rosmanov/pecl-event>.
>
> Please report bugs and submit pull requests on GitHub.
>
> The Bitbucket repository remains available as a compatibility mirror so existing automated builds and clone URLs continue to work.
>
> Existing clones can switch with:
>
> ```bash
> git remote set-url origin https://github.com/rosmanov/pecl-event.git
> ```

ABOUT LIBEVENT
--------------

The `libevent` API provides a mechanism to execute a callback function when a
specific event occurs on a file descriptor or after a timeout has been reached.
Furthermore, libevent also support callbacks due to _signals_ or regular
_timeouts_.

`libevent` is meant to replace the event loop found in event driven network
servers. An application just needs to call `event_dispatch()` and then add or
remove events dynamically without having to change the event loop.

SEE ALSO
========

- [Libevent homepage](http://libevent.org/)
- [Installation instructions](INSTALL.md).

AUTHOR
======

Ruslan Osmanov <osmanov@php.net>

LICENSE
=======

[PHP 3.01](LICENSE)
