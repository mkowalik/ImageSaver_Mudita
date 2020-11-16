#pragma once

#include <string>
#include <vector>

class ICCommunication {
public:

    enum class Role {CommandsReader, ImageWriter};
private:
    int fdRequest, fdResponse;
    Role role;
    std::string fifoRequestName, fifoResponseName;

public:

    class Request {
        std::string command;
    public:
        Request(std::string _command);
        Request(std::vector<char> _buffer);
        void                readFromBuffer(std::vector<char>);
        std::vector<char>   writeToBuffer();
        std::string         getCommand();
    };
    
    class Response {

    public:
        enum class ResponseType {ACK = 0, ERROR = 1, FINISH = 2};
    private:
        ResponseType    type;
        std::string     message;
    public:
        Response(ResponseType _type, std::string _message);
        Response(std::vector<char> _buffer);
        void readFromBuffer(std::vector<char>);
        std::vector<char>   writeToBuffer();
        ResponseType        getType();
        std::string         getMessage();
    };

    ICCommunication(Role role, std::string FIFO_REQUEST_NAME = "FIFO_REQUEST_NAME", std::string FIFO_RESPONSE_NAME = "FIFO_RESPONSE_NAME", std::string DIR_PATH = "../");

    Response    sendRequest(Request request);

    Request     waitForRequest();
    void        sendResponse(Response resp);

    ~ICCommunication();
    
};
