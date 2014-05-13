

/**
 * `header.h' - libhttp
 *
 * copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
 */

#ifndef HTTP_HEADER_H
#define HTTP_HEADER_H 1

#include <sock/sock.h>

#define http_header_free(h) (hash_clear(h), hash_free(h))
#define http_header_set(r, k, v) (hash_set(r->headers, k, v))
#define http_header_get(r, k) (hash_get(r->headers, k))

#endif
