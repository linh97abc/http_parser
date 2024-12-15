// Server side C/C++ program to demonstrate Socket
// programming

#define _WIN32_WINNT 0x601
#pragma comment(lib, "Ws2_32.lib")
// #include <winsock.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <ws2spi.h>
// #include <sys/types.h>

// #include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <sys/socket.h>
// #include <unistd.h>
#define PORT 9000

void Start()
{
    WSADATA info;
    if (WSAStartup(MAKEWORD(2, 0), &info))
    {
        throw "Could not start WSA";
    }
}

#include <stdio.h>
#include <string.h>
#include <http_parser.h>

#include <webutil.h>
// #include "websrc/index.html.c"
// #include "websrc/content.css.c"
// #include "websrc/control.css.c"
// #include "websrc/control.js.c"
// #include "websrc/firmware.css.c"
// #include "websrc/fixheight.js.c"
// #include "websrc/log.css.c"
// #include "websrc/menu.css.c"
// #include "websrc/page.css.c"
// #include "websrc/page.js.c"

#include "websrc/generated/webSrc.h"

struct WebAppInfo
{
    const char *fileName;
    const char *ext;
    const char *data;
    unsigned size;
    bool isCompress;
};

// WebAppInfo webAppInfo[] = {
//     {"/", NULL, index_html, sizeof(index_html)-1},
//     {"/css/content.css", "css", content_css, sizeof(content_css)-1},
//     {"/css/control.css", "css", control_css, sizeof(control_css)-1},
//     {"/css/firmware.css", "css", firmware_css, sizeof(firmware_css)-1},
//     {"/css/log.css", "css", log_css, sizeof(log_css)-1},
//     {"/css/menu.css", "css", menu_css, sizeof(menu_css)-1},
//     {"/css/page.css", "css", page_css, sizeof(page_css)-1},
//     {"/js/control.js", "js", control_js, sizeof(control_js)-1},
//     {"/js/fixheight.js", "js", fixheight_js, sizeof(fixheight_js)-1},
//     {"/js/page.js", "js", page_js, sizeof(page_js)-1},
// };

WebAppInfo webAppInfo[] = {
    {"/", NULL, index_html, sizeof(index_html), true},
    {"/css/content.css", "css", content_css, sizeof(content_css), true},
    {"/css/control.css", "css", control_css, sizeof(control_css), true},
    {"/css/firmware.css", "css", firmware_css, sizeof(firmware_css), true},
    {"/css/log.css", "css", log_css, sizeof(log_css), true},
    {"/css/menu.css", "css", menu_css, sizeof(menu_css), true},
    {"/css/page.css", "css", page_css, sizeof(page_css), true},
    {"/js/control.js", "js", control_js, sizeof(control_js), true},
    {"/js/fixheight.js", "js", fixheight_js, sizeof(fixheight_js), true},
    {"/js/page.js", "js", page_js, sizeof(page_js), true},
};

char notfound_html[] =
    "<html> \
	<head> \
		<title>404</title> \
		<style type=\"text/css\"> \
		div#request {background: #eeeeee} \
		</style> \
	</head> \
	<body> \
	<h1>404 Page Not Found</h1> \
	<div id=\"request\"> \
	</div> \
	</body> \
	</html>";

WebAppInfo notFoundHtmlInfo = {"", NULL, notfound_html, sizeof(notfound_html) - 1, false};

WebAppInfo *getHttpSrc(const char *fileName)
{
    for (size_t i = 0; i < sizeof(webAppInfo) / sizeof(WebAppInfo); i++)
    {
        if (strcmp(fileName, webAppInfo[i].fileName) == 0)
        {
            return &webAppInfo[i];
        }
    }

    return &notFoundHtmlInfo;
}

http_parser httpParerInst;
http_parser_settings setting;

struct Http_request_data
{
    http_method method;
    char url[64];
    int sock;
};

struct Http_request_data http_request_data;

int on_message_begin(http_parser *parser)
{
    puts(__FUNCTION__);

    return 0;
}

int on_headers_complete(http_parser *parser)
{
    puts(__FUNCTION__);

    const char *method = http_method_str((http_method)parser->method);

    printf("method: %s\n", method);

    return 0;
}

int on_message_complete(http_parser *parser)
{
    puts(__FUNCTION__);

    char reponse[4096];

    const char *method = http_method_str((http_method)parser->method);

    printf("method: %s\n", method);
    // printf("data: %s\n", parser->data);

    if (parser->method == HTTP_GET)
    {
        WebAppInfo *responseData = getHttpSrc(http_request_data.url);

        int reponseLen = generate_http_header(
            reponse,
            responseData->ext,
            responseData->isCompress,
            responseData->size);

        send(http_request_data.sock, reponse, reponseLen, 0);
        send(http_request_data.sock, responseData->data, responseData->size, 0);
        send(http_request_data.sock, "\r\n", 2, 0);
    }

    return 0;
}

int on_url(http_parser *, const char *at, size_t length)
{
    puts(__FUNCTION__);

    char path[64];

    memcpy(path, at, length);
    path[length] = '\0';

    printf("path: %s\n", path);

    memcpy(http_request_data.url, at, length);
    http_request_data.url[length] = '\0';

    return 0;
}

int on_body(http_parser *, const char *at, size_t length)
{
    puts(__FUNCTION__);

    char data[1025];

    memcpy(data, at, length);
    data[length] = '\0';

    printf("data: %s\n", data);

    return 0;
}

void process_response(int new_socket)
{
    int valread;
    char buffer[1024] = {0};

    http_parser_init(&httpParerInst, HTTP_REQUEST);
    http_request_data.sock = new_socket;

    do
    {
        valread = recv(new_socket, buffer, 1024, 0);
        int len = http_parser_execute(&httpParerInst, &setting, buffer, valread);

    } while (valread > 0);

    // send(new_socket, hello, strlen(hello), 0);
}

int main(int argc, char const *argv[])
{
    Start();

    setting.on_message_begin = on_message_begin;
    setting.on_headers_complete = on_headers_complete;
    setting.on_url = on_url;
    setting.on_message_complete = on_message_complete;
    setting.on_body = on_body;

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    const char *hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR, (const char *)&opt,
                   sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        /* code */
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                 (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        printf("new connect\n");

        process_response(new_socket);
        // closing the connected socket
        printf("close connect\n");
        closesocket(new_socket);
    }

    return 0;
}
