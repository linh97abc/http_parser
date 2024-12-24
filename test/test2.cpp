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
// #include <sys/socket.h>
// #include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
#include <HttpParser.hpp>

#include <webutil.h>

static const uint8_t webFsSrc[] = {
#include "webfs.inc"
};

class WebFS
{
    const uint8_t *src;
    unsigned srcLen;

    static bool IsFileHasSamePath(const char *path, const char *s)
    {
        if (*path == '/')
        {
            path++;
        }

        while (true)
        {
            if (*s == '\0')
                return (*path == '\0') || (*path == '?');
            if (*s != *path)
                return false;

            s++;
            path++;
        }
    }

public:
    struct WebFileData
    {
        enum WebUtilFileExt ext;
        const void *data;
        unsigned size;
        bool isCompress;
    };

    void GetWebFileData(const char *fpath, WebFileData &out)
    {
        const uint8_t *pdata = src;

        static const char notfound_html[] =
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

        out.data = nullptr;

        while (pdata < (this->src + this->srcLen))
        {
            bool isThisFile = IsFileHasSamePath(fpath, (const char *)pdata);
            while (*pdata != '\0')
            {
                pdata++;
            }
            pdata++;

            uint32_t size;
            memcpy(&size, pdata, 4);
            pdata += 4;

            if (isThisFile)
            {
                auto ext = (enum WebUtilFileExt) * pdata;

                if (ext >= WebUtilFileExt_end)
                {
                    ext = WebUtilFileExt_unknow;
                }
                pdata++;

                out.ext = ext;

                out.data = pdata;
                out.size = size;
                out.isCompress = true;

                puts("has file");
                break;
            }
            else
            {
                pdata++;
            }

            pdata += size;
        }

        if (out.data == nullptr)
        {
            puts("file not found");
            out.data = notfound_html;
            out.size = sizeof(notfound_html) - 1;
            out.isCompress = false;
            out.ext = WebUtilFileExt_html;
        }
    }

    void ImportSource(const void *data, unsigned len)
    {
        this->src = (const uint8_t *)data;
        this->srcLen = len;
    }
};

static WebFS webFs;

// http_parser httpParerInst;
// http_parser_settings setting;

class WebHandler : public HttpParser
{
    int sock;

    virtual int Send(const char *data, int len) override
    {
        return send(this->sock, data, len, 0);
    }

    virtual int OnHttpGet() override
    {

        WebFS::WebFileData responseData;

        if (this->url == "/")
        {
            webFs.GetWebFileData("/index.html", responseData);
        }
        else
        {

            webFs.GetWebFileData(this->url.c_str(), responseData);
        }

        this->SendResponse(
            (const char *)responseData.data,
            responseData.size,
            responseData.ext,
            responseData.isCompress);

        return 0;
    }

public:
    void SetSock(int sock)
    {
        this->sock = sock;
    }
};

WebHandler webHandler;

void process_response(int new_socket)
{

    webHandler.NewSession();
    webHandler.SetSock(new_socket);

    int valread;
    char buffer[1024] = {0};
    do
    {
        valread = recv(new_socket, buffer, 1024, 0);
        int len = webHandler.execute(buffer, valread);

    } while (valread > 0);

    // send(new_socket, hello, strlen(hello), 0);
}

int main(int argc, char const *argv[])
{
    webFs.ImportSource(webFsSrc, sizeof(webFsSrc));

    Start();

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
