
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <http/header.h>
#include <http/socket.h>
#include <http/request.h>
#include <sock/sock.h>
#include <ok/ok.h>
#include <assert.h>

#define EQ(a,b) (NULL != a && NULL != b && 0 == strcmp(a, b))

#define I(x) x

#define REQ_A(body, len)                         \
  "GET / HTTP/1.1\r\n"                           \
  "Host: 0.0.0.0\r\n"                            \
  "Connection: keep-alive\r\n"                   \
  "Cache-Control: no-cache\r\n"                  \
  "Pragma: no-cache\r\n"                         \
  "Accept: */*;\r\n"                             \
  "User-Agent: Test/libhttp\r\n"                 \
  "Content-Length: " # len "\r\n"                \
  "\r\n\r\n" I(body)

int
main (void) {
  {
    http_socket_t *sock = NULL;
    http_request_t *req = NULL;
    char *buf = REQ_A("foo", 3);

    sock = http_socket_new();
    assert(sock);

    ok("http_socket_new()");

    req = http_request_new(sock, buf);
    assert(req);

    ok("http_request_new()");

    assert(EQ("/", req->url));
    assert(EQ("GET", req->method_s));
    assert(1 == req->http_ver_major);
    assert(1 == req->http_ver_minor);
    assert(1 == (req->version >> 8 & 0xff));
    assert(1 == (req->version & 0xff));
    assert(EQ("0.0.0.0", http_header_get(req, "Host")));
    assert(EQ("keep-alive", http_header_get(req, "Connection")));
    assert(EQ("no-cache", http_header_get(req, "Cache-Control")));
    assert(EQ("*/*;", http_header_get(req, "Accept")));
    assert(EQ("Test/libhttp", http_header_get(req, "User-Agent")));
    assert(EQ("foo", req->body));

    http_request_free(req);
    http_socket_free(sock);

    ok("headers");
  }

  {

  }

  ok_done();
  return 0;
}
