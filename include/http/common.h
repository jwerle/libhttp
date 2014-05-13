
/**
 * `common.h' - libhttp
 *
 * copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
 */

#ifndef HTTP_COMMON_H
#define HTTP_COMMON_H 1

#if __GNUC__ >= 4
# define HTTP_EXTERN __attribute__((visibility("default")))
#else
# define HTTP_EXTERN
#endif

#endif
