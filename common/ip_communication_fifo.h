#pragma once

#include <string>

#include "ip_communication.h"

class IPCommunicationFIFO : public IPCommunication {
    int         fdRequest, fdResponse;
    Role        role;
    std::string fifoRequestName, fifoResponseName;
public:
    IPCommunicationFIFO(IPCommunication::Role role, 
                        const std::string& FIFO_REQUEST_NAME = "FIFO_REQ", 
                        const std::string& FIFO_RESPONSE_NAME = "FIFO_RESP", 
                        const std::string& DIR_PATH = "/tmp/");

    virtual Response    sendRequest(const Request& request) override;
    virtual Request     waitForRequest() override;
    virtual void        sendResponse(const Response& resp) override;

    virtual             ~IPCommunicationFIFO();
};
