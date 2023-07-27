#include <stdio.h>
#include <string.h>
#include <http_parser.h>


http_parser httpParerInst;
http_parser_settings setting;

int on_message_begin(http_parser* parser)
{
    puts(__FUNCTION__);

    return 0;
}

int on_headers_complete(http_parser* parser)
{
    puts(__FUNCTION__);
    

    const char *method = http_method_str((http_method) parser->method);

    printf("method: %s\n", method);
    

    return 0;
}

int on_message_complete(http_parser* parser)
{
    puts(__FUNCTION__);


    const char* method = http_method_str((http_method)parser->method);

    printf("method: %s\n", method);
    //printf("data: %s\n", parser->data);


    return 0;
}

int on_url(http_parser*, const char* at, size_t length)
{
    puts(__FUNCTION__);
    
    char path[64];

    memcpy(path, at, length);
    path[length] = '\0';

    printf("path: %s\n", path);

    return 0;
}

int on_body(http_parser*, const char* at, size_t length)
{
    puts(__FUNCTION__);

    char data[64];

    memcpy(data, at, length);
    data[length] = '\0';

    printf("data: %s\n", data);

    return 0;
}


int main() {

     //char data[] = "GET /api/func?param1=1&param2=2 HTTP/1.1\r\n"
     //    "Host: localhost:9001\r\n"
     //    "Upgrade-Insecure-Requests: 1\r\n"
     //    "Sec-Fetch-Site: none\r\n"
     //    "Sec-Fetch-Mode: navigate\r\n"
     //    "Sec-Fetch-User: ?1\r\n"
     //    "Sec-Fetch-Dest: document\r\n"
     //    "Accept-Encoding: gzip, deflate, br\r\n"
     //    "Accept-Language: vi,en;q=0.9,en-GB;q=0.8,en-US;q=0.7\r\n\r\n";

    char data[] = "GET /api/func?";
        char data1[] = "param1=1&param2=2 HTTP/1.1\r\n"
         "Host: localhost:9001\r\n"
         "Upgrade-Insecure-Requests: 1\r\n"
         "Sec-Fetch-Site: none\r\n"
         "Sec-Fetch-Mode: navigate\r\n"

        "Sec-Fetch-User: ?1\r\n"
         "Sec-Fetch-Dest: document\r\n"
         "Accept-Encoding: gzip, deflate, br\r\n"
         "Accept-Language: vi,en;q=0.9,en-GB;q=0.8,en-US;q=0.7\r\n\r\n";


    //char data[] =
    //    "POST /index.html HTTP/1.1\r\n"
    //    "Host: 192.168.31.157:8002\r\n"
    //    "Connection: keep-alive\r\n"
    //    "Content-Length: 13\r\n"
    //    "Cache-Control: max-age=0\r\n"
    //    "Upgrade-Insecure-Requests: 1\r\n"
    //    "Origin: http://192.168.31.157:8002\r\n"
    //    "Content-Type: application/x-www-form-urlencoded\r\n"
    //    "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/114.0.0.0 Safari/537.36 Edg/114.0.1823.43\r\n"
    //    "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7\r\n"
    //    "Referer: http://192.168.31.157:8002/\r\n"
    //    "Accept-Encoding: gzip, deflate\r\n"
    //    "Accept-Language: vi,en;q=0.9,en-GB;q=0.8,en-US;q=0.7\r\n\r\n"
    //    "uname=0&psw=1\r\n";

    http_parser_init(&httpParerInst, HTTP_REQUEST);

    setting.on_message_begin = on_message_begin;
    setting.on_headers_complete = on_headers_complete;
    setting.on_url = on_url;
    setting.on_message_complete = on_message_complete;
    setting.on_body = on_body;

    int len = http_parser_execute(&httpParerInst, &setting, data, sizeof(data)-1);

    printf("parser-en len = %d\n", len);

    len = http_parser_execute(&httpParerInst, &setting, data1, sizeof(data1));

    printf("parser-en len = %d\n", len);

    return 0;
}