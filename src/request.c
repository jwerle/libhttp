
/**
 * `request.c' - libhttp
 *
 * copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <http-parser/http_parser.h>
#include <sock/sock.h>
#include <hash/hash.h>
#include "http.h"

#define MAX_REQUEST_BUFFER_SIZE 4096

#define set_state(r, s) ({                                 \
  http_request_t *req = ((http_request_t *) r->data);      \
  req->state_ = s;                                         \
})

#define get_state(r) (((http_request_t *) self->data)->state_)

#define STATE(state) HTTP_REQUEST_STATE_ ## state

static int
substrcat (char *dst, const char *src, int s, int e) {
  size_t size = 0;
  size_t mlen = strlen(src);
  dst[size] = '\0';
  e = e > mlen ? mlen : e;
  while (s < e) { dst[size++] = src[s++]; }
  dst[size] = '\0';
  return size;
}

static int
on_message_begin (http_parser *self) {
  set_state(self, STATE(MESSAGE_BEGIN));
  return 0;
}

static int
on_message_complete (http_parser *self) {
  http_request_t *req = (http_request_t *) self->data;

  req->needs_upgrade = self->upgrade;

  req->status = self->status_code;
  req->version = self->http_major << 8 | self->http_minor;

  req->http_ver_major = self->http_major;
  req->http_ver_minor = self->http_minor;

  req->method = self->method;
  req->method_s = http_method_str(req->method);

  set_state(self, STATE(MESSAGE_COMPLETE));
  return 0;
}

static int
on_headers_complete (http_parser *self) {
  set_state(self, STATE(HEADERS_COMPLETE));
  return 0;
}

static int
on_url (http_parser *self, const char *buf, size_t len) {
  set_state(self, STATE(URL));
  http_request_t *req = (http_request_t *) self->data;
  substrcat(req->url, buf, 0, len);
  return 0;
}

static int
on_header_field (http_parser *self, const char *buf, size_t len) {
  http_request_t *req = NULL;
  if (get_state(self) == STATE(HEADER_FIELD)) { return 1; }
  set_state(self, STATE(HEADER_FIELD));
  req = (http_request_t *) self->data;
  substrcat(req->last_header_, buf, 0, len);
  return 0;
}

static int
on_header_value (http_parser *self, const char *buf, size_t len) {
  char value[1024];
  http_request_t *req = NULL;
  if (get_state(self) != STATE(HEADER_FIELD)) { return 1; }
  set_state(self, STATE(HEADER_VALUE));
  req = (http_request_t *) self->data;
  substrcat(value, buf, 0, len);
  http_header_set(req, strdup(req->last_header_), strdup(value));
  return 0;
}

static int
on_body (http_parser *self, const char *buf, size_t len) {
  http_request_t *req = (http_request_t *) self->data;
  set_state(self, STATE(BODY));
  substrcat(req->body, buf, 2, len + 2);
  return 0;
}

static http_parser_settings settings = {

  // meta
  .on_message_begin = on_message_begin,
  .on_message_complete = on_message_complete,
  .on_headers_complete = on_headers_complete,

  // data
  .on_url = on_url,
  .on_body = on_body,
  .on_header_field = on_header_field,
  .on_header_value = on_header_value,

};

http_request_t *
http_request_new (http_socket_t* sock, const char *buf) {
  http_request_t *self = NULL;
  http_parser *parser = NULL;
  int nparsed = 0;
  size_t size = 0;

  // ensure alloc
  if (NULL == buf) { return NULL; }

  size = strlen(buf);

  // malloc
  self = (http_request_t *) malloc(sizeof(http_request_t));
  if (NULL == self) { return NULL; }

  parser = (http_parser *) malloc(sizeof(http_parser));
  if (NULL == parser) { return NULL; }

  // parser
  parser->data = self;
  http_parser_init(parser, HTTP_REQUEST);

  // init
  self->socket_ = sock;
  self->parser_ = parser;
  self->headers = hash_new();

  // parse
  nparsed = http_parser_execute(parser, &settings, buf, size);

  return self;
}

void
http_request_free (http_request_t *req) {
  http_header_free(req->headers);
  free(req->parser_);
  free(req);
}

http_request_t *
http_request (const char *url, int port) {
  http_request_t *req = NULL;
  socket_t *sock = NULL;
  char *buf = NULL;
  char *data = NULL;
  size_t len = 0;
  size_t size = 0;
  int rc = 0;

  // init
  sock = sock_tcp_client_new(url, port);
  if (NULL == sock) { return NULL; }

  data = (char *) malloc(sizeof(char) * MAX_REQUEST_BUFFER_SIZE);
  if (NULL == data) {
    sock_free(sock);
    return NULL;
  }

  // connect
  rc = sock_connect(sock);
  if (rc < 0) {
    sock_free(sock);
    return NULL;
  }

  // read
  while ((len = sock_read(sock, buf, MAX_REQUEST_BUFFER_SIZE)) > 0) {
    printf("%s\n", buf);
  }

  req = http_request_new((http_socket_t *) sock, buf);
  if (NULL == req) {
    sock_free(sock);
    return NULL;
  }

  sock_close(sock);

  return NULL;
}
