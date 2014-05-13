
/**
 * `dgram.h' - libsock
 *
 * copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
 */

#ifndef SOCK_DGRAM_H
#define SOCK_DGRAM_H 1

#include "common.h"
#include "socket.h"

typedef struct socket_dgram_s {
  SOCK_SOCKET_FIELDS
} socket_dgram_t;

/**
 * Creates a new datagram socket
 */

SOCK_EXTERN socket_t *
sock_dgram_new (int, int);

#endif
