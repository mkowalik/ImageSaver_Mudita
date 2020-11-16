
#include <string>
#include <cstdio>

class ICCommunication {
public:

    enum class Role {CommandsReader, CommandsWriter};
private:
    constexpr static char FIFO_REQUEST_NAME[] = "FIFO_REQUEST_NAME";
    constexpr static char FIFO_RESPONSE_NAME[] = "FIFO_RESPONSE_NAME";
    static constexpr std::size_t BUFFER_SIZE = 80;

    char readbuf[80];
    int fdRequest, fdResponse;
    Role role;
    struct timeval timeoutValue = {
        .tv_sec = 0,
        .tv_usec = 200000 //200ms
    };

public:

/*
    class Request {
        std::string command;
        void readFromStream(std::istream& str);
        void writeToStream(std::ostream& str);
    };
    */
    class Response {
        bool ack;
        bool finish;
        std::string error;
        // void readFromStream(std::istream& str);
        // void writeToStream(std::ostream& str);
    };

    ICCommunication(Role role/*, std::string FIFO_REQUEST_NAME = "FIFO_REQUEST_NAME"*/);

    Response sendRequest(std::string data);

    // Request waitForRequest();
    std::string waitForRequest();
    void sendResponse(Response resp);

    ~ICCommunication();
    
};
