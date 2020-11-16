
#include "icCommunication.h"
#include <iostream>

int main(){

   system ("./test_client.exe &");

   ICCommunication writer(ICCommunication::Role::ImageWriter);
    std::cout << "S: Writer constructed" << std::endl;
   ICCommunication::Response r = writer.sendRequest(ICCommunication::Request(std::string("mise!")));
    std::cout << "S: Request done" << std::endl;
    std::cout << r.getMessage() << std::endl;

   return 0;
}
