//
// Created by pavel on 02.05.16.
//

#ifndef RENDERER_CLOSEDTHING_H
#define RENDERER_CLOSEDTHING_H


#include "Closed.h"
#include "Thing.h"

class ClosedThing : public Thing, public Closed {
public:
    bool isInside(Point3D point) const override {
        RayIntersection inter1 = intersect(Ray(point, Vector3D(1, 0, 0)));
        RayIntersection inter2 = intersect(Ray(point, Vector3D(-1, 0, 0)));
        return inter1.isValid && inter1.norm.x > 0 && inter2.isValid && inter2.norm.x < 0;
    }
};


#endif //RENDERER_CLOSEDTHING_H
