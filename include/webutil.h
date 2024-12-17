#ifndef __WEBUTIL_H__
#define __WEBUTIL_H__

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

enum WebUtilFileExt
{
    WebUtilFileExt_unknow, //  text/plain   
    WebUtilFileExt_html, // text/html  
    WebUtilFileExt_css, //  text/css  
    WebUtilFileExt_js, //  application/javascript //text/javascript
    WebUtilFileExt_csv, //  text/csv
    WebUtilFileExt_xml, //  text/xml
    WebUtilFileExt_jpg, //  image/jpeg 
    WebUtilFileExt_png, //  image/png 
    WebUtilFileExt_gif, //  image/gif
    WebUtilFileExt_json, // application/json  
    WebUtilFileExt_zip, // application/zip
    WebUtilFileExt_pdf, // application/pdf 
    WebUtilFileExt_mp4, // video/mp4
    WebUtilFileExt_webm, // video/webm
    WebUtilFileExt_end, // video/webm
};

int generate_http_header(char *buf, enum WebUtilFileExt fext, bool isCompress, int fsize);

#ifdef __cplusplus
}
#endif

#endif // __WEBUTIL_H__