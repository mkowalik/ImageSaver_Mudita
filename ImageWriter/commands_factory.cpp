#include "commands_factory.h"

std::unique_ptr<Command> CommandsFactory::getCommandFromString(std::string command){
    std::vector<std::string> commandSplitted = splitToWords(command);
    if (commandSplitted[0].compare("SET_WIDTH")){
        if (commandSplitted.size() != 2){
            throw std::logic_error("Wrong arguments number for SET_WIDTH command.");
        }
        int width = stringToInt(commandSplitted[1]);
        return std::unique_ptr<SetWidthCommand>(new SetWidthCommand(width));
    } else if (commandSplitted[0].compare("SET_HEIGHT")){
        if (commandSplitted.size() != 2){
            throw std::logic_error("Wrong arguments number for SET_HEIGHT command.");
        }
        int width = stringToInt(commandSplitted[1]);
        return std::unique_ptr<SetHeightCommand>(new SetHeightCommand(width));
    } else if (commandSplitted[0].compare("DRAW_RECTANGLE")){
        if (commandSplitted.size() != 5){
            throw std::logic_error("Wrong arguments number for DRAW_RECTANGLE command.");
        }
        int x = stringToInt(commandSplitted[1]);
        int y = stringToInt(commandSplitted[2]);
        int width = stringToInt(commandSplitted[3]);
        int height = stringToInt(commandSplitted[4]);
        return std::unique_ptr<DrawRectangleCommand>(new DrawRectangleCommand(x, y, width, height));
    } else if (commandSplitted[0].compare("DRAW_TRIANGLE")){
        if (commandSplitted.size() != 7){
            throw std::logic_error("Wrong arguments number for DRAW_TRIANGLE command.");
        }
        int x1 = stringToInt(commandSplitted[1]);
        int y1 = stringToInt(commandSplitted[2]);
        int x2 = stringToInt(commandSplitted[3]);
        int y2 = stringToInt(commandSplitted[4]);
        int x3 = stringToInt(commandSplitted[5]);
        int y3 = stringToInt(commandSplitted[6]);
        return std::unique_ptr<DrawTriangleCommand>(new DrawTriangleCommand(x1, y1, x2, y2, x3, y3));
    } else if (commandSplitted[0].compare("RENDER")){
        if (commandSplitted.size() != 2){
            throw std::logic_error("Wrong arguments number for RENDER command.");
        }
        return std::unique_ptr<RenderNameCommand>(new RenderNameCommand(std::move(commandSplitted[1])));
    } else {
        throw std::logic_error("Command unknown.");
    }
}

std::vector<std::string> CommandsFactory::splitToWords(std::string command){
    std::vector<std::string> ret;
    std::string::size_type posL = 0;
    while (posL < command.length()){
        std::string::size_type posR = command.find(' ', posL);
        if (posR - posL == 1){
            continue;
        }
        ret.push_back(std::string(command, posL, posR - posL));
        posL = posR + 1;
    }
    return ret;
}

int CommandsFactory::stringToInt(std::string str){
    try {
        return std::stoi(str);
    } catch (std::invalid_argument const& e){
        throw std::logic_error("Wrong argument format - wrong format of integer.");
    } catch (std::out_of_range const& e){
        throw std::logic_error("Wrong argument format - integer out_of_range.");
    }
}