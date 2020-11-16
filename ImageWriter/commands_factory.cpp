#include "commands_factory.h"
#include <iostream>
#include <cctype>
#include <algorithm>
#include <cstdio>

std::unique_ptr<Command> CommandsFactory::getCommandFromString(const std::string& command){
    std::vector<std::string> commandSplitted = splitToWords(command, ' ');
    std::cout << "splitted" << std::endl;
    if (commandSplitted[0].compare("SET_WIDTH") == 0){
        if (commandSplitted.size() != 2){
            throw std::logic_error("Wrong arguments number for SET_WIDTH command.");
        }
        int width = stringToInt(commandSplitted[1]);
        return std::unique_ptr<SetWidthCommand>(new SetWidthCommand(width));
    } else if (commandSplitted[0].compare("SET_HEIGHT") == 0){
        if (commandSplitted.size() != 2){
            throw std::logic_error("Wrong arguments number for SET_HEIGHT command.");
        }
        int width = stringToInt(commandSplitted[1]);
        return std::unique_ptr<SetHeightCommand>(new SetHeightCommand(width));
    } else if (commandSplitted[0].compare("DRAW_RECTANGLE") == 0){

        commandSplitted = splitToWords(commandSplitted[1], ',');
        if (commandSplitted.size() != 4){
            throw std::logic_error("Wrong arguments number for DRAW_RECTANGLE command.");
        }
        int x = stringToInt(commandSplitted[0]);
        int y = stringToInt(commandSplitted[1]);
        int width = stringToInt(commandSplitted[2]);
        int height = stringToInt(commandSplitted[3]);
        return std::unique_ptr<DrawRectangleCommand>(new DrawRectangleCommand(x, y, width, height));
    } else if (commandSplitted[0].compare("DRAW_TRIANGLE") == 0){
        commandSplitted = splitToWords(commandSplitted[1], ',');
        if (commandSplitted.size() != 6){
            throw std::logic_error("Wrong arguments number for DRAW_TRIANGLE command.");
        }
        int x1 = stringToInt(commandSplitted[0]);
        int y1 = stringToInt(commandSplitted[1]);
        int x2 = stringToInt(commandSplitted[2]);
        int y2 = stringToInt(commandSplitted[3]);
        int x3 = stringToInt(commandSplitted[4]);
        int y3 = stringToInt(commandSplitted[5]);
        return std::unique_ptr<DrawTriangleCommand>(new DrawTriangleCommand(x1, y1, x2, y2, x3, y3));
    } else if (commandSplitted[0].compare("RENDER") == 0){
        if (commandSplitted.size() != 2){
            throw std::logic_error("Wrong arguments number for RENDER command.");
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

int CommandsFactory::stringToInt(const std::string& str){
    try {
        int ret = std::stoi(str);
        return ret;
    } catch (std::invalid_argument const& e){
        throw std::logic_error("Wrong argument format - wrong format of integer.");
    } catch (std::out_of_range const& e){
        throw std::logic_error("Wrong argument format - integer out_of_range.");
    }
}

void CommandsFactory::trimWhitespaces(std::string& str){   
    str.erase(
        std::remove_if(str.begin(), str.end(), ::isspace), 
        str.end()
    );
}   