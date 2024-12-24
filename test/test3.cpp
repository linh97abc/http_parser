#include <HttpParser.hpp>


class WebServerHandler : public HttpParser
{
    virtual IProcessPost& OnParserCmdPost() {}
    virtual int OnHttpGet() { return 0; }
    

public:
    WebServerHandler(/* args */);
    ~WebServerHandler();
};
