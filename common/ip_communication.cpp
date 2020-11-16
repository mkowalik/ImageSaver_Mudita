#include "ip_communication.h"

#include <cstdio>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <iostream>

static constexpr std::size_t BUFFER_SIZE = 80;

IPCommunication::Request::Request(const std::string& _command) : command(_command)
{
}

IPCommunication::Request::Request(const std::vector<char>& _buffer)
{
    readFromBuffer(_buffer);
}

void IPCommunication::Request::readFromBuffer(const std::vector<char>& _buffer)
{
    command = std::string(_buffer.begin(), _buffer.end());
}

std::vector<char> IPCommunication::Request::writeToBuffer() const
{
    return std::vector<char>(command.begin(), command.end());
}

std::string IPCommunication::Request::getCommand() const
{
    return command;
}

IPCommunication::Response::Response(const IPCommunication::Response::ResponseType& _type, const std::string& _message) : 
type(_type), message(_message) 
{
}

IPCommunication::Response::Response(const std::vector<char>& bytes)
{
    readFromBuffer(bytes);
}

IPCommunication::Response::ResponseType IPCommunication::Response::getType() const
{
    return type;
}

std::string IPCommunication::Response::getMessage() const
{
    return message;
}

void IPCommunication::Response::readFromBuffer(const std::vector<char>& bytes)
{
    type = *(reinterpret_cast<const ResponseType*>(bytes.data()));

    message = std::string(bytes.begin() + sizeof(ResponseType), bytes.end());
}

std::vector<char> IPCommunication::Response::writeToBuffer() const
{
    std::vector<char> ret;

    ret.insert(ret.end(), reinterpret_cast<const char*>(&type), reinterpret_cast<const char*>(&type) + sizeof(ResponseType));
    ret.insert(ret.end(), message.begin(), message.end());
    return ret;
}

IPCommunication::IPCommunication(Role _role, 
                                 const std::string& FIFO_REQUEST_NAME, 
                                 const std::string& FIFO_RESPONSE_NAME, 
                                 const std::string& DIR_PATH) 
: 
role(_role), fifoRequestName(DIR_PATH + FIFO_REQUEST_NAME), fifoResponseName(DIR_PATH + FIFO_RESPONSE_NAME)
{
    fifoRequestName.append("_" + std::to_string(getpid()));
    fifoResponseName.append("_" + std::to_string(getpid()));
    mkfifo(FIFO_REQUEST_NAME.c_str(), 0666);
    mkfifo(FIFO_RESPONSE_NAME.c_str(), 0666);

    if (role == Role::CommandsReader){
        fdRequest = open(FIFO_REQUEST_NAME.c_str(), O_WRONLY);
        fdResponse = open(FIFO_RESPONSE_NAME.c_str(), O_RDONLY);
/*
        while ((readBytes = read(fdResponse, buffer, BUFFER_SIZE)) == BUFFER_SIZE){}
        if (readBytes < 0){
            throw std::runtime_error("IPCommunication: Error during inter-process communication initialisation.");
        }*/
    } else {
        fdRequest = open(FIFO_REQUEST_NAME.c_str(), O_RDONLY);
        fdResponse = open(FIFO_RESPONSE_NAME.c_str(), O_WRONLY);
        /*readBytes = read(fdRequest, buffer, BUFFER_SIZE);
        while ((readBytes = read(fdRequest, buffer, BUFFER_SIZE)) == BUFFER_SIZE){}
        if (readBytes < 0){
            throw std::runtime_error("IPCommunication: Error during inter-process communication initialisation.");
        }*/
    }

    std::cout << fdRequest << " " << fdResponse << std::endl;
}

IPCommunication::Response IPCommunication::sendRequest(const Request& request)
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
        throw std::runtime_error("sendRequest: Timeout while waiting for the request ACK.");
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

IPCommunication::Request IPCommunication::waitForRequest()
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

void IPCommunication::sendResponse(const Response& resp)
{
    std::vector<char> respBytes = resp.writeToBuffer();
    auto writtenBytes = write(fdResponse, respBytes.data(), respBytes.size());
    if (writtenBytes != static_cast<decltype(writtenBytes)>(respBytes.size())){
        throw std::runtime_error("sendResponse: Error during write in inter-process communication. Wrote bytes number different than expected.");
    }
}

IPCommunication::~IPCommunication()
{
    close(fdRequest);
    close(fdResponse);
    remove(fifoRequestName.c_str());
    remove(fifoResponseName.c_str());
}