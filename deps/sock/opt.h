
/**
 * `opt.h' - libsock
 *
 * copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
 */

#ifndef SOCK_OPT_H
#define SOCK_OPT_H 1

#include "common.h"
#include "socket.h"

#define X(o) SOCK_OPT_ ## o
enum {
  X(BACKLOG) = 0xff,
  X(PORT),
  X(ADDR),
  X(HOST)
};
#undef X

/**
 * Sets an opt on socket
 */

SOCK_EXTERN int
sock_set_opt (socket_t *, int, const void *);

#endif
