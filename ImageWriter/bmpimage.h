// Michal Kowalik <kowalik.michal1@gmail.com>
#pragma once

#include "image.h"

#define cimg_display 0
#include "../lib/CImg-2.9.3_pre101320/CImg.h"

#include <vector>
#include <tuple>

using namespace cimg_library;

class BMPImage : public Image {
    CImg<int> img;
    std::vector<std::tuple<int, int, int, int>> linesToDraw;
    static constexpr int blackColor[] = {0, 0, 0};
    static constexpr int whiteColor[] = {255, 255, 255};
    static constexpr int defaultWidth = 640;
    static constexpr int defaultHeight = 480;
public:
    BMPImage();
    virtual void drawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) override;
    virtual void setWidth(unsigned int w) override;
    virtual void setHeight(unsigned int h) override;
    virtual void saveImage(std::string fileName) override;
};