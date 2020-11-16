#pragma once

#include <string>
#include <vector>

class IPCommunication {
public:

    enum class Role {CommandsReader, ImageWriter};

    class Request {
        std::string command;
    public:
        Request(const std::string& _command);
        Request(const std::vector<char>& _buffer);
        void                readFromBuffer(const std::vector<char>& buffer);
        std::vector<char>   writeToBuffer() const;
        std::string         getCommand() const;
    };
    
    class Response {

    public:
        enum class ResponseType {ACK = 0, ACK_FINISH = 1, ERROR = 2};
    private:
        ResponseType    type;
        std::string     message;
    public:
        Response(const ResponseType& _type, const std::string& _message);
        Response(const std::vector<char>& _buffer);
        void                readFromBuffer(const std::vector<char>& buffer);
        std::vector<char>   writeToBuffer() const;
        ResponseType        getType() const ;
        std::string         getMessage() const;
    };

    virtual Response    sendRequest(const Request& request) = 0;
    
    virtual Request     waitForRequest() = 0;
    virtual void        sendResponse(const Response& resp) = 0;
};
