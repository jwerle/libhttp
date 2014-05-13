
/**
 * `socket.c' - libhttp
 *
 * copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
 */

#include <stdlib.h>
#include <sock/sock.h>
#include "http.h"

http_socket_t *
http_socket_new () {
  http_socket_t *sock = (http_socket_t *) sock_tcp_new();
  if (NULL == sock) { return NULL; }
  sock = (http_socket_t *) sock_init((socket_t *) sock, -1);
  if (NULL == sock) { return NULL; }
  return sock;
}
