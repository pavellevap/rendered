//
// Created by pavel on 08.04.16.
//

#include "Renderer.h"

Renderer::Renderer(size_t pixelsInWidth, size_t pixelsInHeight, size_t antialiasingRate)
        : pixelsInWidth(pixelsInWidth), pixelsInHeight(pixelsInHeight),
          ssMask(antialiasingRate, antialiasingRate, 1.0 / antialiasingRate / antialiasingRate) {
    if (antialiasingRate == 3) {
        ssMask.set(0, 0, 0.1); ssMask.set(0, 1, 0.1); ssMask.set(0, 2, 0.1);
        ssMask.set(1, 0, 0.1); ssMask.set(1, 1, 0.2); ssMask.set(1, 2, 0.1);
        ssMask.set(2, 0, 0.1); ssMask.set(2, 1, 0.1); ssMask.set(2, 2, 0.1);
    }
}

RGBImage Renderer::createImage(const Scene& scene) const {
    size_t width = pixelsInWidth * ssMask.width();
    size_t height = pixelsInHeight * ssMask.height();

    RGBImage image(width, height, RGBColor(0, 0, 0)); // image with higher resolution, to prevent aliasing

    ThreadPool<void> threadPool(std::thread::hardware_concurrency() + 2);

    for (size_t i = 0; i < height; i++) {
        threadPool.submit(std::function<void()>([&scene, &image, width, height, i] () {
            for (size_t j = 0; j < width; j++) {
                Ray ray(scene.camera.getPosition(), scene.camera.getPixelPos(j, i, width, height));
                image.setPixel(i, j, scene.trace(ray, 10));
            }
        }));
    }
    threadPool.shutdown();

    return image;
}

RGBImage Renderer::reduceResolution(const RGBImage& image) const {
    RGBImage screenImage(pixelsInWidth, pixelsInHeight, RGBColor(0, 0, 0)); // image that will be shown on the screen

    for (size_t i = 0; i < pixelsInHeight; i++)
        for (size_t j = 0; j < pixelsInWidth; j++) {
            RGBColor pixelColor(0, 0, 0);
            for (size_t u = 0; u < ssMask.height(); u++)
                for (size_t v = 0; v < ssMask.width(); v++) {
                    size_t x = i * ssMask.height() + u;
                    size_t y = j * ssMask.width() + v;
                    pixelColor.r += ssMask.get(u, v) * image.getPixel(x, y).r;
                    pixelColor.g += ssMask.get(u, v) * image.getPixel(x, y).g;
                    pixelColor.b += ssMask.get(u, v) * image.getPixel(x, y).b;
                }
            screenImage.setPixel(i, j, pixelColor);
        }

    return screenImage;
}

RGBImage Renderer::renderScene(const Scene& scene) const {
    return reduceResolution(createImage(scene));
}

