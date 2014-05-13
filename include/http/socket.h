
/**
 * `socket.h' - libhttp
 *
 * copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
 */

#ifndef HTTP_SOCKET_H
#define HTTP_SOCKET_H 1

#include <sock/sock.h>
#include "common.h"

#define http_socket_free(s) (sock_free((socket_t *) s))

typedef struct http_socket_s {
  SOCK_SOCKET_FIELDS
} http_socket_t;

HTTP_EXTERN http_socket_t *
http_socket_new ();

#endif
