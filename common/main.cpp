
#include "icCommunication.h"
#include <thread>
#include <iostream>

void foo(){
    std::cout << "construct begin" <<std::endl;
    ICCommunication reader(ICCommunication::Role::CommandsReader);
    std::cout << "construct middle" <<std::endl;
    std::string mis = reader.waitForRequest();
    std::cout << mis << std::endl;
}

int main(){

    std::thread thr(foo);

    ICCommunication writer(ICCommunication::Role::ImageWriter);
    std::cout << "construct done" <<std::endl;
    writer.sendRequest(std::string("misemisemise"));



    thr.join();
    return 0;
}
