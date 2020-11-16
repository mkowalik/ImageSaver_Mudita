
#include "icCommunication.h"
#include <thread>
#include <iostream>

int main(){

    std::cout << "client started" << std::endl;
    ICCommunication reader(ICCommunication::Role::CommandsReader);
    std::string mis = reader.waitForRequest();
    std::cout << mis << std::endl;

    return 0;
}
