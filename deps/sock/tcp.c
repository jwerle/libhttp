
/**
 * `socket.c' - libsock
 *
 * copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
 */

#define _POSIX_C_SOURCE199309L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <netdb.h>
#include "sock.h"

socket_t *
sock_tcp_new () {
  int fd = sock_raw_new(SOCK_IP, SOCK_STREAM, IPPROTO_TCP);
  socket_tcp_t *self = (socket_tcp_t *) malloc(sizeof(socket_tcp_t));
  int t = 1;
  if (NULL == self) { return NULL; }

  sock_init((socket_t *) self, fd);

  self->backlog = 0;
  self->type = SOCK_TYPE_TCP;
  self->addr->sin_family = AF_INET;
  self->addr->sin_addr.s_addr = INADDR_ANY;

  sock_set_opt((socket_t *) self, SO_REUSEADDR, (void *) &t);

  return (socket_t *) self;
}

socket_t *
sock_tcp_client_new (const char *host, int port) {
  int rc = 0;
  socket_t * self = sock_tcp_new();

  rc = sock_set_opt(self, SOCK_OPT_PORT, (void *) &port);
  if (rc < 0) { return NULL; }

  rc = sock_set_opt(self, SOCK_OPT_HOST, (void *) host);
  if (rc < 0) { return NULL; }

  return self;
}

int
sock_tcp_listen (socket_t *sock) {
  socket_tcp_t *self = (socket_tcp_t *) sock;
  int rc = listen(sock->fd, self->backlog);
  if (rc < 0) { return perror("sock_tcp_listen"), rc; }
  return rc;
}

