#pragma once

#include <string>
#include <vector>
#include <memory>

#include "command.h"

class CommandsFactory {
private:
    std::vector<std::string> splitToWords(std::string);
    int stringToInt(std::string str);
public:
    std::unique_ptr<Command> getCommandFromString(std::string command);
};