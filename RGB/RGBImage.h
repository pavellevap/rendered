//
// Created by pavel on 08.04.16.
//

#ifndef RENDERER_RGBIMAGE_H
#define RENDERER_RGBIMAGE_H

#include <vector>
#include <cstddef>
#include "RGBColor.h"

class RGBImage {
public:
    RGBImage(size_t width, size_t height, RGBColor background = RGBColor::BLACK) :
            image(height, std::vector<RGBColor>(width, background)) {}

    RGBColor getPixel(size_t i, size_t j) const {
        return image[i][j];
    }

    void setPixel(size_t i, size_t j, RGBColor color) {
        image[i][j] = color;
    }

    size_t width() const {
        return image[0].size();
    }

    size_t height() const {
        return image.size();
    }

private:
    std::vector< std::vector<RGBColor> > image;
};


#endif //RENDERER_RGBIMAGE_H
