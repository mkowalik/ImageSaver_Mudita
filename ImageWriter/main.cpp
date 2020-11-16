
#include "../common/icCommunication.h"
#include "commands_factory.h"
#include "pngimage.h"

#include <stdexcept>

int main(){

    ICCommunication icCom(ICCommunication::Role::ImageWriter);
    CommandsFactory commandsFactory;
    BMPImage img;

    while (true){
        ICCommunication::Request req = icCom.waitForRequest();
        
        try {
            Command& command = commandsFactory.getCommandFromString(req.getCommand());
            bool finish = command.execute(img);
            ICCommunication::Response resp = finish 
                    ? ICCommunication::Response(ICCommunication::Response::ResponseType::FINISH, "") 
                    : ICCommunication::Response(ICCommunication::Response::ResponseType::ACK, "");
            icCom.sendResponse(resp);
        } catch (std::logic_error& e){
            ICCommunication::Response resp(ICCommunication::Response::ResponseType::ERROR, e.what());
            icCom.sendResponse(resp);
        }
    }

    return 0;
}