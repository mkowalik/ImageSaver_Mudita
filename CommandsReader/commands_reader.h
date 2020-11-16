// Michal Kowalik <kowalik.michal1@gmail.com>
#pragma once

#include "../common/ip_communication.h"

#include <fstream>

class CommandsReader 
{
public:
    virtual IPCommunication::Request readCommand() = 0;
};

class StreamCommandsReader : public CommandsReader
{
    std::istream& str;
public:
    StreamCommandsReader(std::istream& str);
    virtual IPCommunication::Request readCommand() override;
};

class FileCommandsReader : public StreamCommandsReader
{
    std::ifstream ifStream;
public:
    FileCommandsReader(const std::string& filePath);
};

class CINCommandsReader : public StreamCommandsReader
{
public:
    CINCommandsReader();
};