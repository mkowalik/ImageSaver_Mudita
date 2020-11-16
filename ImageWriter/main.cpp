// Michal Kowalik <kowalik.michal1@gmail.com>
#include "../common/ip_communication_fifo.h"
#include "commands_factory.h"
#include "bmpimage.h"

#include <stdexcept>
#include <memory>

std::unique_ptr<IPCommunication> getIPCommunication(){
    return std::unique_ptr<IPCommunication>(new IPCommunicationFIFO(IPCommunication::Role::ImageWriter));
}

std::unique_ptr<Image> getImage(){
    return std::unique_ptr<Image>(new BMPImage());
}

std::unique_ptr<CommandsFactory> getCommandsFactory(){
    return std::unique_ptr<CommandsFactory>(new CommandsFactory());
}

int main(){
    auto ipCom = getIPCommunication();
    auto img = getImage();
    auto commandsFactory = getCommandsFactory();

    bool finish = false;
    while (!finish){
        IPCommunication::Request req = ipCom->waitForRequest();
        try {
            std::unique_ptr<Command> command = commandsFactory->getCommandFromString(req.getCommand());
            finish = command->executeCommand(*img);
            IPCommunication::Response resp = finish 
                    ? IPCommunication::Response(IPCommunication::Response::ResponseType::ACK_FINISH, "") 
                    : IPCommunication::Response(IPCommunication::Response::ResponseType::ACK, "");
            ipCom->sendResponse(resp);
        } catch (std::logic_error& e){
            IPCommunication::Response resp(IPCommunication::Response::ResponseType::ERROR, req.getCommand() + " - " + e.what());
            ipCom->sendResponse(resp);
        }
    }
    return 0;
}