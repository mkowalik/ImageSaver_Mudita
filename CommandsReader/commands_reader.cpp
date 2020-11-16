// Michal Kowalik <kowalik.michal1@gmail.com>

#include <iostream>

#include "commands_reader.h"


StreamCommandsReader::StreamCommandsReader(std::istream& _str) : str{_str}
{

}

IPCommunication::Request StreamCommandsReader::readCommand(){
    std::string command;
    while (command.empty()){
        std::getline(str, command);
    }
    return IPCommunication::Request(command);
}

FileCommandsReader::FileCommandsReader(const std::string& _filePath) :  StreamCommandsReader{ifStream}, ifStream{_filePath}
{
}

CINCommandsReader::CINCommandsReader() : StreamCommandsReader{std::cin}
{

}