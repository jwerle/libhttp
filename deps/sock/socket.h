
/**
 * `socket.h' - libsock
 *
 * copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
 */

#ifndef SOCK_SOCKET_H
#define SOCK_SOCKET_H 1

#include <sys/socket.h>
#include "common.h"

#define SOCK_BUFSIZE 4096

#define SOCK_SOCKET_FIELDS   \
  int type:2;                \
  int fd:16;                 \
  int sfd;                   \
  socklen_t len;             \
  struct sockaddr_in *addr;  \
  struct hostent *host;      \

typedef struct socket_s {
  SOCK_SOCKET_FIELDS
} socket_t;

/**
 * Allocates a new `socket_t *' type
 * with an internal call to `socket(int, int, int)'
 * defaulting to `0' as the protocol family.
 * To create a socket with a set protocol family
 * use `sock_raw_new (int, int, int)' or
 * `socket(int, int, int)' and pass its
 * return value to `sock_init(int)' to get a
 * newly allocated `socket_t *'.
 */

SOCK_EXTERN socket_t *
sock_new (int, int);

/**
 * Calls `socket(int, int, int)' internally
 * and returns file descriptor.
 */

SOCK_EXTERN int
sock_raw_new (int, int, int);

/**
 * Allocates a new `socket_t *' from
 * a given file descriptor created with
 * `socket(int, int, int)'
 */

SOCK_EXTERN socket_t *
sock_init (socket_t *, int);

/**
 * Accepts a connection on `socket_t *'
 */

SOCK_EXTERN int
sock_accept (socket_t *);

/**
 * Recieves message of from `socket_t *' and returns
 * a `char *' pointer
 */

SOCK_EXTERN char *
sock_recv (socket_t *);

/**
 * Reads a message from `socket_t *' into
 * `char *' buffer of `size_t' size. `bero()'
 * is applied to buffer and return code
 * from `read()' is returned
 */

SOCK_EXTERN int
sock_read (socket_t *, char *, size_t);

/**
 * Binds socket
 */

SOCK_EXTERN int
sock_bind (socket_t *);

/**
 * Connects to a socket
 */

SOCK_EXTERN int
sock_connect (socket_t *);

/**
 * Writes to a socket
 */

SOCK_EXTERN int
sock_write (socket_t *, char *);

/**
 * Closes an open socket
 * descriptor
 */

SOCK_EXTERN int
sock_close (socket_t *);

/**
 * Shuts down a socket and
 * closes the open file descriptor
 * if none zero
 */

SOCK_EXTERN int
sock_shutdown (socket_t *);

/**
 * Free a socket from memory
 */

SOCK_EXTERN void
sock_free (socket_t *);

#endif
