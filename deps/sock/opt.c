
/**
 * `opt.c' - libsock
 *
 * copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include "sock.h"

int
sock_set_opt (socket_t *sock, int opt, const void *value) {

  socket_tcp_t *tcp = (socket_tcp_t *) sock;
  /*socket_dgram_t *dgram = NULL; */

  /* option values */
  struct hostent *host = NULL;
  int port = 0;
  int backlog = 0;
  uint32_t hl = 0;

  switch (opt) {
    case SOCK_OPT_BACKLOG:
      switch (sock->type) {
        case SOCK_TYPE_TCP:
          backlog = *recast(int, value);
          tcp->backlog = backlog;
          break;
        default: return -1;
      }
      break;
    case SOCK_OPT_PORT:
      port = *recast(int, value);
      tcp->addr->sin_port = htons(port);
      break;

    case SOCK_OPT_ADDR:
      hl = *recast(uint32_t, value);
      tcp->addr->sin_addr.s_addr = (uint32_t) htonl(hl);
      break;

    case SOCK_OPT_HOST:
      host = gethostbyname((char *)value);
      if (NULL == host) { return -1; }
      memmove(
          &tcp->addr->sin_addr,
          (char *) host->h_addr,
           host->h_length);
      sock->host = host;
      break;

    default:
      return setsockopt(sock->fd,
                 ((struct sockaddr *) sock->addr)->sa_family,
                 opt, value, sizeof(value));
  }
  return 0;
}
