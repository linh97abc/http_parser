#ifndef __WEBUTIL_H__
#define __WEBUTIL_H__

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif


int generate_http_header(char *buf, const char *fext, bool isCompress, int fsize);

#ifdef __cplusplus
}
#endif

#endif // __WEBUTIL_H__