#ifndef __WEBUTIL_H__
#define __WEBUTIL_H__

#ifdef __cplusplus
extern "C" {
#endif

int generate_http_header(char *buf, const char *fext, int fsize);

#ifdef __cplusplus
}
#endif

#endif // __WEBUTIL_H__