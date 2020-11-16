#pragma once

#include <memory>
#include <ostream>

#include "image.h"

class Image;

class Command {
public:
    //< Return information if it is finish (RENDER) command end executes it >//
    virtual bool executeCommand(Image& img) = 0;
};

class SetWidthCommand : public Command {
    unsigned int width;
public:
    SetWidthCommand(unsigned int width);
    virtual bool executeCommand(Image& img) override;
};

class SetHeightCommand : public Command {
    unsigned int height;
public:
    SetHeightCommand(unsigned int height);
    virtual bool executeCommand(Image& img) override;
};

class DrawRectangleCommand : public Command {
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
public:
    DrawRectangleCommand(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
    virtual bool executeCommand(Image& img) override;
};

class DrawTriangleCommand : public Command {
    unsigned int x1;
    unsigned int y1;
    unsigned int x2;
    unsigned int y2;
    unsigned int x3;
    unsigned int y3;
public:
    DrawTriangleCommand(unsigned int x1,
                        unsigned int y1,
                        unsigned int x2,
                        unsigned int y2,
                        unsigned int x3,
                        unsigned int y3);
    virtual bool executeCommand(Image& img) override;
};

class RenderNameCommand : public Command {
    std::string name;
public:
    RenderNameCommand(std::string&& name);
    virtual bool executeCommand(Image& img) override;
};