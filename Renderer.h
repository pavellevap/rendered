//
// Created by pavel on 08.04.16.
//

#ifndef RENDERER_RENDERER_H
#define RENDERER_RENDERER_H

#include "Scene.h"
#include "RGB/RGBImage.h"
#include "Linalg/Point.h"
#include "Linalg/Matrix.h"
#include "Concurrency/ThreadPool.h"
#include <iostream>

class Renderer {
public:
    Renderer(size_t pixelsInWidth, size_t pixelsInHeight, size_t antialiasingRate = 1);

    RGBImage renderScene(const Scene& scene) const;

private:
    size_t pixelsInWidth;
    size_t pixelsInHeight;

    Matrix ssMask; // supersampling mask

    RGBImage createImage(const Scene& scene) const;

    RGBImage reduceResolution(const RGBImage& image) const;
};


#endif //RENDERER_RENDERER_H
