
/**
 * `tcp.h' - libsock
 *
 * copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
 */

#ifndef SOCK_TCP_H
#define SOCK_TCP_H 1

#include "common.h"
#include "socket.h"

typedef struct socket_tcp_s {
  SOCK_SOCKET_FIELDS
  int backlog;
} socket_tcp_t;

/**
 * Create a new tcp socket
 */

SOCK_EXTERN socket_t *
sock_tcp_new ();

/**
 * Listens for connections on socket
 */

SOCK_EXTERN int
sock_tcp_listen (socket_t *);

/**
 * Closes socket with `SHUT_RDWR'
 * using `shutdown(int, int)'
 */

SOCK_EXTERN int
sock_tcp_close (socket_t *);

/**
 * Creates a new tcp socket client
 * for a given host and port
 */

SOCK_EXTERN socket_t *
sock_tcp_client_new (const char *, int);

#endif
