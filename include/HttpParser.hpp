#pragma once

#include "http_parser.h"
#include "webutil.h"
#include <string>

class HttpParser : http_parser
{
    char reponse[1024];

public:
    class IProcessPost
    {
    public:
        virtual int OnHttpPostBody(const char *at, size_t length) = 0;
        virtual int OnHttpPostFinish() = 0;
        virtual ~IProcessPost() = default;
    };

    class ProcessPostDefault : public IProcessPost
    {
    public:
        static ProcessPostDefault &GetInstance()
        {
            static ProcessPostDefault inst;
            return inst;
        }

        virtual int OnHttpPostBody(const char *at, size_t length) override
        {
            return 0;
        }

        virtual int OnHttpPostFinish() override
        {
            return 0;
        }
    };

protected:
    std::string url;

    IProcessPost *processPost;

    void SendResponse(
        const char *content,
        int content_len,
        WebUtilFileExt ext,
        bool isCompress)
    {
        int reponseLen = generate_http_header(
            (char *)this->reponse,
            ext,
            isCompress,
            content_len);

        this->Send(this->reponse, reponseLen);
        this->Send(content, content_len);
        this->Send("\r\n", 2);
    }

private:
    static int on_headers_complete(http_parser *parser)
    {

        return 0;
    }

    static int on_message_complete(http_parser *parser)
    {
        HttpParser *self = (HttpParser *)parser;

        if (parser->method == HTTP_GET)
        {
            return self->OnHttpGet();
        }

        if (parser->method == HTTP_POST)
        {
            return self->processPost->OnHttpPostFinish();
        }

        return 0;
    }

    static int on_url(http_parser *parser, const char *at, size_t length)
    {
        HttpParser *self = (HttpParser *)parser;
        for (size_t i = 0; i < length; i++)
        {
            self->url.push_back(at[i]);
        }

        if (parser->method == HTTP_POST)
        {
            self->processPost = &self->OnParserCmdPost();
        }

        return 0;
    }

    static int on_body(http_parser *parser, const char *at, size_t length)
    {
        if (parser->method == HTTP_POST)
        {
            HttpParser *self = (HttpParser *)parser;
            return self->processPost->OnHttpPostBody(at, length);
        }

        return 0;
    }

    virtual int Send(const char *data, int len) = 0;

    virtual IProcessPost &OnParserCmdPost() { return ProcessPostDefault::GetInstance(); }
    virtual int OnHttpGet() { return 0; }

public:
    HttpParser()
    {
        this->settings = {0};
        this->settings.on_headers_complete = on_headers_complete;
        this->settings.on_url = on_url;
        this->settings.on_message_complete = on_message_complete;
        this->settings.on_body = on_body;

        this->url.reserve(1024);
        this->url = "";

        this->processPost = &ProcessPostDefault::GetInstance();
    }

    virtual ~HttpParser() = default;

    void NewSession()
    {
        http_parser_init(this, HTTP_REQUEST);
        this->url = "";
    }

    int execute(const char *data, size_t len)
    {
        return http_parser_execute(this, &settings, data, len);
    }

private:
    http_parser_settings settings;
};