#pragma once

#include <string>
#include <vector>
#include <memory>

#include "command.h"

class CommandsFactory {
private:
    std::vector<std::string> splitToWords(const std::string&, char splitChar);
    unsigned int stringToUInt(const std::string& str);
    void trimWhitespaces(std::string& str);
public:
    std::unique_ptr<Command> getCommandFromString(const std::string& command);
};