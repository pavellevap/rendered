//
// Created by pavel on 13.04.16.
//

#ifndef RENDERER_BOX_H
#define RENDERER_BOX_H


#include <algorithm>
#include "Intersectable.h"

class Box : public Intersectable {
public:
    Box() : min(Point3D(0, 0, 0)), max(Point3D(0, 0, 0)) {}
    Box(Point3D min, Point3D max) : min(min), max(max) {}

    RayIntersection intersect(Ray ray) const override;

    bool isInside(Point3D point) const;

    static Box unite(const Box& b1, const Box& b2);

//private:
    Point3D min;
    Point3D max;
};


#endif //RENDERER_BOX_H
