// Michal Kowalik <kowalik.michal1@gmail.com>
#include "ip_communication.h"

IPCommunication::Request::Request(const std::string& _command) : command(_command)
{
}

IPCommunication::Request::Request(const std::vector<char>& _buffer)
{
    readFromBuffer(_buffer);
}

void IPCommunication::Request::readFromBuffer(const std::vector<char>& _buffer)
{
    command = std::string(_buffer.begin(), _buffer.end());
}

std::vector<char> IPCommunication::Request::writeToBuffer() const
{
    return std::vector<char>(command.begin(), command.end());
}

std::string IPCommunication::Request::getCommand() const
{
    return command;
}

IPCommunication::Response::Response(const IPCommunication::Response::ResponseType& _type, const std::string& _message) : 
type(_type), message(_message) 
{
}

IPCommunication::Response::Response(const std::vector<char>& bytes)
{
    readFromBuffer(bytes);
}

IPCommunication::Response::ResponseType IPCommunication::Response::getType() const
{
    return type;
}

std::string IPCommunication::Response::getMessage() const
{
    return message;
}

void IPCommunication::Response::readFromBuffer(const std::vector<char>& bytes)
{
    type = *(reinterpret_cast<const ResponseType*>(bytes.data()));

    message = std::string(bytes.begin() + sizeof(ResponseType), bytes.end());
}

std::vector<char> IPCommunication::Response::writeToBuffer() const
{
    std::vector<char> ret;

    ret.insert(ret.end(), reinterpret_cast<const char*>(&type), reinterpret_cast<const char*>(&type) + sizeof(ResponseType));
    ret.insert(ret.end(), message.begin(), message.end());
    return ret;
}