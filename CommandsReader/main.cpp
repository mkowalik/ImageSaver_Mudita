#include <iostream>
#include <cstdlib>
#include <fstream>
#include <memory>

#include "../common/icCommunication.h"


int main(int argc, char* argv[]){

    std::ifstream fil;
    if (argc > 1){
        fil = std::ifstream(argv[1]);
    }
    std::istream& str = (argc > 1) ? fil : std::cin;

    // std::system("firefox"); //< runs another app

    ICCommunication icCom(ICCommunication::Role::CommandsReader);

    std::string command;
    while (true){
        str >> command;
        ICCommunication::Response resp = icCom.sendRequest(ICCommunication::Request(command));
        if (resp.getType() == ICCommunication::Response::ResponseType::ERROR){
            std::cout << "ERROR: " << resp.getMessage() << std::endl;
        } else if (resp.getType() == ICCommunication::Response::ResponseType::FINISH){
            std::cout << "Image saved!" << std::endl;
        }
    }
    return 0;
}