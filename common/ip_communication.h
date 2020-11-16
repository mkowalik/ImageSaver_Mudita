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
        enum class ResponseType {ACK = 0, ERROR = 1, FINISH = 2};
    private:
        ResponseType    type;
        std::string     message;
    public:
        Response(const ResponseType& _type, const std::string& _message);
        Response(const std::vector<char>& _buffer);
        void readFromBuffer(const std::vector<char>& buffer);
        std::vector<char>   writeToBuffer() const;
        ResponseType        getType() const ;
        std::string         getMessage() const;
    };

private:
    int fdRequest, fdResponse;
    Role role;
    std::string fifoRequestName, fifoResponseName;

public:

    IPCommunication(Role role, 
                    const std::string& FIFO_REQUEST_NAME = "FIFO_REQ", 
                    const std::string& FIFO_RESPONSE_NAME = "FIFO_RESP", 
                    const std::string& DIR_PATH = "/tmp/");

    Response    sendRequest(const Request& request);

    Request     waitForRequest();
    void        sendResponse(const Response& resp);

    ~IPCommunication();
    
};
