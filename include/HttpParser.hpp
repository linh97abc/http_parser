#pragma once

#include "http_parser.h"
#include <string>

class HttpParser : http_parser
{

protected:
    std::string url;

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
            return self->OnHttpPostFinish();
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
            self->OnParserCmdPost();
        }

        return 0;
    }

    static int on_body(http_parser *parser, const char *at, size_t length)
    {
        if (parser->method == HTTP_POST)
        {
            HttpParser *self = (HttpParser *)parser;
            return self->OnHttpPostBody(at, length);
        }

        return 0;
    }

    virtual void OnParserCmdPost() { }
    virtual int OnHttpGet() { return 0; }
    virtual int OnHttpPostFinish() { return 0; }
    virtual int OnHttpPostBody(const char *at, size_t length) { return 0; }

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