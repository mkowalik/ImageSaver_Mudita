#include "commands_factory.h"
#include <cctype>
#include <algorithm>
#include <cstdio>

std::unique_ptr<Command> CommandsFactory::getCommandFromString(const std::string& command){
    std::vector<std::string> commandSplitted = splitToWords(command, ' ');
    if (commandSplitted.empty()){
        throw std::logic_error("Empty command.");
    }
    if (commandSplitted[0].compare("SET_WIDTH") == 0){
        if (commandSplitted.size() != 2){
            throw std::logic_error("Wrong arguments number.");
        }
        unsigned long width = stringToUInt(commandSplitted[1]);
        return std::unique_ptr<SetWidthCommand>(new SetWidthCommand(width));
    } else if (commandSplitted[0].compare("SET_HEIGHT") == 0){
        if (commandSplitted.size() != 2){
            throw std::logic_error("Wrong arguments number.");
        }
        unsigned long width = stringToUInt(commandSplitted[1]);
        return std::unique_ptr<SetHeightCommand>(new SetHeightCommand(width));
    } else if (commandSplitted[0].compare("DRAW_RECTANGLE") == 0){

        commandSplitted = splitToWords(commandSplitted[1], ',');
        if (commandSplitted.size() != 4){
            throw std::logic_error("Wrong arguments number.");
        }
        unsigned long x = stringToUInt(commandSplitted[0]);
        unsigned long y = stringToUInt(commandSplitted[1]);
        unsigned long width = stringToUInt(commandSplitted[2]);
        unsigned long height = stringToUInt(commandSplitted[3]);
        return std::unique_ptr<DrawRectangleCommand>(new DrawRectangleCommand(x, y, width, height));
    } else if (commandSplitted[0].compare("DRAW_TRIANGLE") == 0){
        commandSplitted = splitToWords(commandSplitted[1], ',');
        if (commandSplitted.size() != 6){
            throw std::logic_error("Wrong arguments number.");
        }
        unsigned long x1 = stringToUInt(commandSplitted[0]);
        unsigned long y1 = stringToUInt(commandSplitted[1]);
        unsigned long x2 = stringToUInt(commandSplitted[2]);
        unsigned long y2 = stringToUInt(commandSplitted[3]);
        unsigned long x3 = stringToUInt(commandSplitted[4]);
        unsigned long y3 = stringToUInt(commandSplitted[5]);
        return std::unique_ptr<DrawTriangleCommand>(new DrawTriangleCommand(x1, y1, x2, y2, x3, y3));
    } else if (commandSplitted[0].compare("RENDER") == 0){
        if (commandSplitted.size() != 2){
            throw std::logic_error("Wrong arguments number.");
        }
        return std::unique_ptr<RenderNameCommand>(new RenderNameCommand(std::move(commandSplitted[1])));
    } else {
        throw std::logic_error("Command unknown.");
    }
}

std::vector<std::string> CommandsFactory::splitToWords(const std::string& command, char splitChar){
    std::vector<std::string> ret;
    std::string::size_type posL = 0;
    while (posL < command.length()){
        std::string::size_type posR = std::min(command.length(), command.find(splitChar, posL));
        if (posR == posL){ //< found space next to another space
            ++posL;
            continue;
        }
        ret.push_back(std::string(command, posL, posR - posL));
        posL = posR + 1;
    }
    return ret;
}

unsigned int CommandsFactory::stringToUInt(const std::string& str){
    int ret = 0;
    try {
        ret = std::stoi(str);
    } catch (std::invalid_argument const& e){
        throw std::logic_error("Wrong argument format - wrong format of integer.");
    } catch (std::out_of_range const& e){
        throw std::logic_error("Wrong argument format - integer out_of_range.");
    }
    if (ret < 0){
        throw std::logic_error("Wrong argument format - integer out_of_range - value must be greater or equal to 0.");
    }
    return static_cast<unsigned int>(ret);
}

void CommandsFactory::trimWhitespaces(std::string& str){   
    str.erase(
        std::remove_if(str.begin(), str.end(), ::isspace), 
        str.end()
    );
}   