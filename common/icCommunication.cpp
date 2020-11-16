#include "icCommunication.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <chrono>
#include <thread>

ICCommunication::ICCommunication(Role _role/*, std::string _FIFO_REQUEST_NAME*/) : role(_role){

    mkfifo(FIFO_REQUEST_NAME, 0666);
    mkfifo(FIFO_RESPONSE_NAME, 0666);

    if (role == Role::CommandsReader){
        fdRequest = open(FIFO_REQUEST_NAME, O_RDONLY);
        fdResponse = open(FIFO_RESPONSE_NAME, O_WRONLY);
    } else {
        fdRequest = open(FIFO_REQUEST_NAME, O_WRONLY);
        fdResponse = open(FIFO_RESPONSE_NAME, O_RDONLY);
    }
}

ICCommunication::Response ICCommunication::sendRequest(std::string data){

    unsigned char buffer[sizeof(std::size_t)];
    std::size_t length = data.length();
    ssize_t a = write(fdRequest, reinterpret_cast<char*>(&length), sizeof(std::size_t));
    ssize_t b = write(fdRequest, data.c_str(), data.length());

    fd_set set;
    FD_ZERO(&set);
    FD_SET(fdResponse, &set); /* add our file descriptor to the set */

    Response readResponseBuffer;
    int rv = select(fdResponse + 1, &set, NULL, NULL, &timeoutValue);
    if (rv == -1){ //error while executing select 
        throw std::runtime_error("sendRequest: Error during reading inter-process communication.");
    } else if (rv == 0){ //timeout while waiting for data fot fdResponse FIFO
        throw std::runtime_error("sendRequest: Error while waiting for the request ACK.");
    } 

    ssize_t read_bytes = read(fdResponse, &readResponseBuffer, sizeof(Response));
    if (read_bytes != sizeof(Response)){
        throw std::runtime_error("Error during reading inter-process communication.");
    }
    return readResponseBuffer;
}

std::string ICCommunication::waitForRequest(){

    std::size_t commandLength;
    static char read_buffer[BUFFER_SIZE];
    std::string ret;

    ssize_t read_bytes = read(fdRequest, &commandLength, sizeof(std::size_t));
    if (read_bytes != sizeof(std::size_t)){
        throw std::runtime_error("waitForRequest: Error during reading inter-process communication.");
    }
    while (commandLength > 0){
        read_bytes = read(fdRequest, read_buffer, std::min(BUFFER_SIZE, commandLength));
        ret.append(read_buffer, read_bytes);
        commandLength -= read_bytes;
    }
    return ret;
}

ICCommunication::~ICCommunication(){
    close(fdRequest);
     if (role == Role::CommandsReader) {
        std::cout << "close" << std::endl;
    }
}