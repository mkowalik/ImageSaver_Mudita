#pragma once

#include <string>

class Image {
public:
    virtual void drawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) = 0;
    virtual void setWidth(unsigned int w) = 0;
    virtual void setHeight(unsigned int h) = 0;
    virtual void saveImage(std::string fileName) = 0;
};