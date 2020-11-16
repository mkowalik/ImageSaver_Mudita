
#include "../common/ip_communication.h"
#include "commands_factory.h"
#include "bmpimage.h"
#include <iostream>

#include <stdexcept>

int main(){

    // pid_t pid = fork(); //create child process

    // if (pid == -1){ //error
        
    // } else if (pid == 0){ //child process
    //     execl("./CommandsReader/CommandsReader.out", "");
    //     //if returned there is an error
    // }

    // usleep(200);

    IPCommunication icCom(IPCommunication::Role::ImageWriter);
    CommandsFactory commandsFactory;
    BMPImage        img;
    std::cout << "Init Done" << std::endl;
    bool finish = false;
    while (!finish){
        IPCommunication::Request req = icCom.waitForRequest();
        std::cout << "Command read: " << req.getCommand() << std::endl;
        try {
            std::unique_ptr<Command> command = commandsFactory.getCommandFromString(req.getCommand());
            finish = command->executeCommand(img);
            IPCommunication::Response resp = finish 
                    ? IPCommunication::Response(IPCommunication::Response::ResponseType::FINISH, "") 
                    : IPCommunication::Response(IPCommunication::Response::ResponseType::ACK, "");
            icCom.sendResponse(resp);
            int a = 4;
        } catch (std::logic_error& e){
            std::cout << "#Logic Error:" << e.what() << std::endl;
            IPCommunication::Response resp(IPCommunication::Response::ResponseType::ERROR, e.what());
            icCom.sendResponse(resp);
        }
    }

    return 0;
}