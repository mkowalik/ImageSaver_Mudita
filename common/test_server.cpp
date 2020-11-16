
#include "icCommunication.h"
#include <iostream>

int main(){

   system ("./test_client.exe &");

   ICCommunication writer(ICCommunication::Role::CommandsWriter);
   std::cout << "construct done" <<std::endl;
   writer.sendRequest(std::string("mise!"));

   return 0;
}
