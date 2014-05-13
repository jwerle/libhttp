
/**
 * `request.h' - libhttp
 *
 * copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
 */

#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H 1

#include <http-parser/http_parser.h>
#include <hash/hash.h>
#include "socket.h"
#include "common.h"

#define HTTP_REQUEST_MAX_HEADERS 24
#define HTTP_REQUEST_MAX_ELEMENT_SIZE 2048

#define X(s) HTTP_REQUEST_STATE_ ## s
typedef enum {
  X(MESSAGE_BEGIN),
  X(MESSAGE_COMPLETE),
  X(HEADERS_COMPLETE),
  X(HEADER_FIELD),
  X(HEADER_VALUE),
  X(STATUS),
  X(URL),
  X(BODY),
} http_request_state_t;
#undef X

typedef struct http_request_s {

  /* public */
  const char *raw;

  int needs_upgrade:1;

  unsigned int status:4;
  unsigned int version:16;
  unsigned int http_ver_major:1;
  unsigned int http_ver_minor:1;

  uint64_t content_length;

  enum http_parser_type type;
  enum http_method method;

  char url[HTTP_REQUEST_MAX_ELEMENT_SIZE];
  char body[HTTP_REQUEST_MAX_HEADERS];
  const char *method_s;

  hash_t *headers;

  /* private */
  http_socket_t *socket_;
  http_parser *parser_;
  http_request_state_t state_;

  char last_header_[16];

} http_request_t;

/**
 * Parses buffer with `http_parser'
 * and returns an allocated `request_t'
 */

HTTP_EXTERN http_request_t *
http_request_new (http_socket_t *, const char *);

/**
 * Free a `http_request_t'
 */

HTTP_EXTERN void
http_request_free (http_request_t *);

HTTP_EXTERN http_request_t *
http_request (const char *, int);

#endif
