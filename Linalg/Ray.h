//
// Created by pavel on 08.04.16.
//

#ifndef RENDERER_RAY_H
#define RENDERER_RAY_H


#include "Vector.h"
#include "Point.h"

class Ray {
public:
    Ray() : origin(0, 0, 0), dir(0, 0, 1) {}
    Ray(Point3D p1, Point3D p2) : origin(p1), dir(p1, p2) {
        dir = dir.normalize();
    }
    Ray(Point3D origin, Vector3D dir) : origin(origin), dir(dir) {
        this->dir = this->dir.normalize();
    }

    Point3D getPoint(double rayCoord) {
        return (origin + dir * rayCoord).toPoint3D();
    }

    Point3D origin;
    Vector3D dir;
};


#endif //RENDERER_RAY_H
