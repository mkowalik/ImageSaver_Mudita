#include "command.h"

SetWidthCommand::SetWidthCommand(unsigned int _width) : width(_width)
{
}

bool SetWidthCommand::executeCommand(Image& img) 
{

}

SetHeightCommand::SetHeightCommand(unsigned int _height) : height(_height)
{

}

bool SetHeightCommand::executeCommand(Image& img)
{

}

DrawRectangleCommand::DrawRectangleCommand(unsigned int _x, unsigned int _y, unsigned int _width, unsigned int _height) : 
    x(_x), y(_y), width(_width), height(_height)
{
}

bool DrawRectangleCommand::executeCommand(Image& img)
{
    
}

DrawTriangleCommand::DrawTriangleCommand(unsigned int _x1,
                        unsigned int _y1,
                        unsigned int _x2,
                        unsigned int _y2,
                        unsigned int _x3,
                        unsigned int _y3) 
    : 
    x1(_x1), y1(_y1), x2(_x2), y2(_y2), x3(_x3), y3(_y3)
{
}

bool DrawTriangleCommand::executeCommand(Image& img)
{

}

RenderNameCommand::RenderNameCommand(std::string&& _name) : name(_name)
{

}

bool RenderNameCommand::executeCommand(Image& img)
{

}
