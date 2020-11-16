
#include "icCommunication.h"
#include <thread>
#include <iostream>
#include <unistd.h>


int main(){

//    system ("./test_server.exe &");

    std::cout << "client started" << std::endl;
    ICCommunication reader(ICCommunication::Role::CommandsReader);
    ICCommunication::Request mis = reader.waitForRequest();
    std::cout << "C: Received Request" << std::endl;
    std::cout << mis.getCommand() << std::endl;
    ICCommunication::Response resp(ICCommunication::Response::ResponseType::ACK, "ACK!!!!");
    reader.sendResponse(resp);
    std::cout << "C: Response sent" << std::endl;

    return 0;
}
