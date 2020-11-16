#pragma once

#include "image.h"

class PNGImage : public Image {
public:
    virtual void drawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) override;
    virtual void setWeight(unsigned int w) override;
    virtual void setHeight(unsigned int h) override;
    virtual void saveImage(std::string fileName) override;
};