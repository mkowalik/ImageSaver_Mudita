// Michal Kowalik <kowalik.michal1@gmail.com>
#include "bmpimage.h"

BMPImage::BMPImage() : img(defaultWidth, defaultHeight, 1, 3, 255) {

}
    
void BMPImage::drawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
    linesToDraw.push_back(std::make_tuple(x1,y1,x2,y2));
}

void BMPImage::setWidth(unsigned int w) {
    img.resize(w, img.height());
}

void BMPImage::setHeight(unsigned int h) {
    img.resize(img.width(), h);
}

void BMPImage::saveImage(std::string fileName) {
    for (auto line : linesToDraw){
        img.draw_line(std::get<0>(line), std::get<1>(line), std::get<2>(line), std::get<3>(line), blackColor);
    }
    fileName.append(".bmp");
    img.save_bmp(fileName.c_str());
}