#include "icCommunication.h"

#include <cstdio>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>

static constexpr std::size_t BUFFER_SIZE = 80;

ICCommunication::Request::Request(std::string _command) : command(_command)
{
}

ICCommunication::Request::Request(std::vector<char> _buffer)
{
    readFromBuffer(_buffer);
}

void ICCommunication::Request::readFromBuffer(std::vector<char> _buffer){
    command = std::string(_buffer.begin(), _buffer.end());
}

std::vector<char> ICCommunication::Request::writeToBuffer(){
    return std::vector<char>(command.begin(), command.end());
}

std::string ICCommunication::Request::getCommand(){
    return command;
}

ICCommunication::Response::Response(ICCommunication::Response::ResponseType _type, std::string _message) : 
type(_type), message(_message) 
{
}

ICCommunication::Response::Response(std::vector<char> bytes)
{
    readFromBuffer(bytes);
}

ICCommunication::Response::ResponseType ICCommunication::Response::getType()
{
    return type;
}

std::string ICCommunication::Response::getMessage()
{
    return message;
}

void ICCommunication::Response::readFromBuffer(std::vector<char> bytes)
{
    type = *(reinterpret_cast<const ResponseType*>(bytes.data()));
    bytes.erase(bytes.begin(), bytes.begin() + sizeof(ResponseType));

    message = std::string(bytes.begin(), bytes.end());
}

std::vector<char> ICCommunication::Response::writeToBuffer()
{
    std::vector<char> ret;

    ret.insert(ret.end(), reinterpret_cast<char*>(&type), reinterpret_cast<char*>(&type) + sizeof(ResponseType));
    ret.insert(ret.end(), message.begin(), message.end());
    return ret;
}

ICCommunication::ICCommunication(Role _role, std::string FIFO_REQUEST_NAME, std::string FIFO_RESPONSE_NAME, std::string DIR_PATH) : role(_role), fifoRequestName(FIFO_REQUEST_NAME), fifoResponseName(FIFO_RESPONSE_NAME)
{
    FIFO_REQUEST_NAME.insert(0, DIR_PATH);
    FIFO_RESPONSE_NAME.insert(0, DIR_PATH);
    mkfifo(FIFO_REQUEST_NAME.c_str(), 0666);
    mkfifo(FIFO_RESPONSE_NAME.c_str(), 0666);

    if (role == Role::CommandsReader){
        fdRequest = open(FIFO_REQUEST_NAME.c_str(), O_RDONLY);
        fdResponse = open(FIFO_RESPONSE_NAME.c_str(), O_WRONLY);
    } else {
        fdRequest = open(FIFO_REQUEST_NAME.c_str(), O_WRONLY);
        fdResponse = open(FIFO_RESPONSE_NAME.c_str(), O_RDONLY);
    }
}

ICCommunication::Response ICCommunication::sendRequest(Request request)
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
        .tv_usec = 200000 //200ms
    };

    char readResponseBuffer[BUFFER_SIZE];
    auto rv = select(fdResponse + 1, &set, NULL, NULL, &timeoutValue);
    if (rv == -1){ //error while executing select 
        throw std::runtime_error("sendRequest: Error during reading inter-process communication. select() function error.");
    } else if (rv == 0){ //timeout while waiting for data fot fdResponse FIFO
        throw std::runtime_error("sendRequest: Error while waiting for the request ACK.");
    } 

    std::vector<char> receivedBytes;
    auto readBytes = read(fdResponse, readResponseBuffer, BUFFER_SIZE);
    receivedBytes.insert(receivedBytes.end(), readResponseBuffer, readResponseBuffer + readBytes);
    while (readBytes == BUFFER_SIZE){
        readBytes = read(fdResponse, readResponseBuffer, BUFFER_SIZE);
        receivedBytes.insert(receivedBytes.end(), readResponseBuffer, readResponseBuffer + readBytes);
    }
    return Response(receivedBytes);
}

ICCommunication::Request ICCommunication::waitForRequest()
{
    static char readBuffer[BUFFER_SIZE];
    std::string ret;

    std::vector<char> receivedBytes;
    auto readBytes = read(fdRequest, readBuffer, BUFFER_SIZE);
    receivedBytes.insert(receivedBytes.end(), readBuffer, readBuffer + readBytes);
    while (readBytes == BUFFER_SIZE){
        readBytes = read(fdRequest, readBuffer, BUFFER_SIZE);
        receivedBytes.insert(receivedBytes.end(), readBuffer, readBuffer + readBytes);
    }
    return Request(receivedBytes);
}

void ICCommunication::sendResponse(Response resp)
{
    std::vector<char> respBytes = resp.writeToBuffer();
    auto writtenBytes = write(fdResponse, respBytes.data(), respBytes.size());
    if (writtenBytes != static_cast<decltype(writtenBytes)>(respBytes.size())){
        throw std::runtime_error("sendResponse: Error during write in inter-process communication. Wrote bytes number different than expected.");
    }
}

ICCommunication::~ICCommunication()
{
    close(fdRequest);
    close(fdResponse);
    remove(fifoRequestName.c_str());
    remove(fifoResponseName.c_str());
}