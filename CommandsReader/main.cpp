#include <iostream>
#include <cstdlib>
#include <fstream>
#include <memory>

#include <sys/wait.h>
#include <unistd.h>

#include "../common/ip_communication.h"


int main(int argc, char* argv[]){
    std::cout << "child done" << std::endl;

    pid_t pid = fork(); //create child process

    if (pid == -1){ //error
        
    } else if (pid == 0){ //child process
        execl("./ImageWriter/ImageWriter.out", "");
        //if returned there is an error
    }
    //< parent process below >//
    std::ifstream fil;
    if (argc > 1){
        fil = std::ifstream(argv[1]);
    }
    std::istream& str = (argc > 1) ? fil : std::cin;

    IPCommunication icCom(IPCommunication::Role::CommandsReader);

    std::string command;
    while (true){
        std::getline(str, command);
        IPCommunication::Response resp = icCom.sendRequest(IPCommunication::Request(command));
        if (resp.getType() == IPCommunication::Response::ResponseType::ERROR){
            std::cout << "ERROR: " << resp.getMessage() << std::endl;
        } else if (resp.getType() == IPCommunication::Response::ResponseType::FINISH){
            std::cout << "Image saved!" << std::endl;
            break;
        }
    }
    int status;
    // waitpid(pid, &status, 0);
    return 0;
}