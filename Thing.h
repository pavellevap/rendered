//
// Created by pavel on 13.04.16.
//

#ifndef RENDERER_OBJECT3D_H
#define RENDERER_OBJECT3D_H

#include "Boundable.h"

class Thing : public Boundable, public Intersectable {
public:
    virtual Box getBoundingBox() const = 0;
    virtual RayIntersection intersect(Ray ray) const = 0;
};


#endif //RENDERER_OBJECT3D_H
