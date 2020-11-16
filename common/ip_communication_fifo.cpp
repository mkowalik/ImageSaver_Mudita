#include "ip_communication_fifo.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdexcept>

static constexpr std::size_t BUFFER_SIZE = 80;

IPCommunicationFIFO::IPCommunicationFIFO(IPCommunication::Role _role, 
                                         const std::string& FIFO_REQUEST_NAME, 
                                         const std::string& FIFO_RESPONSE_NAME, 
                                         const std::string& DIR_PATH) 
: 
role(_role), fifoRequestName(DIR_PATH + FIFO_REQUEST_NAME), fifoResponseName(DIR_PATH + FIFO_RESPONSE_NAME)
{
    mkfifo(fifoRequestName.c_str(), 0666);
    mkfifo(fifoResponseName.c_str(), 0666);
    if (role == Role::CommandsReader){
        fdRequest = open(fifoRequestName.c_str(), O_WRONLY);
        fdResponse = open(fifoResponseName.c_str(), O_RDONLY);
    } else {
        fdRequest = open(fifoRequestName.c_str(), O_RDONLY);
        fdResponse = open(fifoResponseName.c_str(), O_WRONLY);
    }
    if (fdRequest <= 0 || fdResponse <= 0){
        throw std::runtime_error("Could not create system FIFO.");
    }
}

IPCommunication::Response IPCommunicationFIFO::sendRequest(const Request& request)
{
    auto writeBuffer = request.writeToBuffer();
    auto writtenBytes = write(fdRequest, writeBuffer.data(), writeBuffer.size());
    if (writtenBytes != static_cast<decltype(writtenBytes)>(writeBuffer.size())){
        throw std::runtime_error("sendRequest: Error during write in inter-process communication. Wrote bytes number different than expected.");
    }

    fd_set set;
    FD_ZERO(&set);
    FD_SET(fdResponse, &set); /* add our file descriptor to the set */

    struct timeval timeoutValue = {
        .tv_sec = 0,
        .tv_usec = 500000 //500ms
    };

    char readResponseBuffer[BUFFER_SIZE];
    auto rv = select(fdResponse + 1, &set, NULL, NULL, &timeoutValue);
    if (rv == -1){ //error while executing select 
        throw std::runtime_error("sendRequest: Error during reading inter-process communication. select() function error.");
    } else if (rv == 0){ //timeout while waiting for data fot fdResponse FIFO
        throw std::runtime_error("sendRequest: Timeout while waiting for the request ACK.");
    } 

    std::vector<char> receivedBytes;
    auto readBytes = read(fdResponse, readResponseBuffer, BUFFER_SIZE);
    if (readBytes <= 0){ 
        throw std::runtime_error("sendRequest: Error during reading inter-process communication. read() function error.");
    }
    receivedBytes.insert(receivedBytes.end(), readResponseBuffer, readResponseBuffer + readBytes);
    while (readBytes == BUFFER_SIZE){
        readBytes = read(fdResponse, readResponseBuffer, BUFFER_SIZE);
        receivedBytes.insert(receivedBytes.end(), readResponseBuffer, readResponseBuffer + readBytes);
    }
    return Response(receivedBytes);
}

IPCommunication::Request IPCommunicationFIFO::waitForRequest()
{
    static char readBuffer[BUFFER_SIZE];
    std::string ret;
    std::vector<char> receivedBytes;

    fd_set set;
    FD_ZERO(&set);
    FD_SET(fdRequest, &set); /* add our file descriptor to the set */

    auto rv = select(fdRequest + 1, &set, NULL, NULL, NULL);
    if (rv == -1 || rv == 0){ //error while executing select 
        throw std::runtime_error("waitForRequest: Error while waiting for the request.");
    }
    auto readBytes = read(fdRequest, readBuffer, BUFFER_SIZE);
    if (readBytes == -1 || readBytes == 0){
        throw std::runtime_error("waitForRequest: Error while reading the request.");
    }
    receivedBytes.insert(receivedBytes.end(), readBuffer, readBuffer + readBytes);
    while (readBytes == BUFFER_SIZE){
        readBytes = read(fdRequest, readBuffer, BUFFER_SIZE);
        receivedBytes.insert(receivedBytes.end(), readBuffer, readBuffer + readBytes);
    }
    return Request(receivedBytes);
}

void IPCommunicationFIFO::sendResponse(const Response& resp)
{
    std::vector<char> respBytes = resp.writeToBuffer();
    auto writtenBytes = write(fdResponse, respBytes.data(), respBytes.size());
    if (writtenBytes != static_cast<decltype(writtenBytes)>(respBytes.size())){
        throw std::runtime_error("sendResponse: Error during write in inter-process communication. Wrote bytes number different than expected.");
    }
}

IPCommunicationFIFO::~IPCommunicationFIFO()
{
    close(fdRequest);
    close(fdResponse);
    remove(fifoRequestName.c_str());
    remove(fifoResponseName.c_str());
}