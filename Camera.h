//
// Created by pavel on 22.04.16.
//

#ifndef RENDERER_CAMERA_H
#define RENDERER_CAMERA_H

#include <cstddef>
#include <iostream>
#include "Linalg/Ray.h"

class Camera {
public:
    Camera() {
        pos = Point3D(0, 0, -1);
        dir = Vector3D(0, 0, 1);
        upDir = Vector3D(0, 1, 0);
        rightDir = Vector3D(-1, 0, 0);
        distToScreen = 1.0f;
        screenWidth = 1.0f;
        screenHeight = 1.0f;
        screenCenter = Point3D(0, 0, 0);
    }

    Camera(Point3D pos, Vector3D dir, Vector3D upDir, double distToScreen, double screenWidth, double screenHeight) :
            pos(pos), dir(dir), upDir(upDir), distToScreen(distToScreen), screenWidth(screenWidth), screenHeight(screenHeight) {
        dir = dir.normalize();
        upDir = upDir - dir * dot(upDir, dir);
        upDir = upDir.normalize();
        screenCenter = Ray(pos, dir).getPoint(distToScreen);
        rightDir = cross(dir, upDir);
    }

    Point3D getPixelPos(size_t x, size_t y, size_t pixelsInWidth, size_t pixelsInHeight) const {
        return (Vector3D(screenCenter) + rightDir * screenWidth  * (x - pixelsInWidth / 2.0f) / pixelsInWidth
                                       - upDir    * screenHeight * (y - pixelsInHeight / 2.0f) / pixelsInHeight).toPoint3D();
    }

    Point3D getPosition() const {
        return pos;
    }

    Vector3D getDirection() const {
        return dir;
    }

    Vector3D getUpDirection() const {
        return upDir;
    }

private:
    Point3D pos;
    Vector3D dir;
    Vector3D upDir;        // up direction for camera
    Vector3D rightDir;     // right direction for camera
    double distToScreen;   // distance to screen surface
    double screenWidth;    // screen width in 3D space
    double screenHeight;   // screen height in 3D space
    Point3D screenCenter;
};


#endif //RENDERER_CAMERA_H
