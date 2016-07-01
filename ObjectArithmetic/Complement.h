//
// Created by pavel on 02.05.16.
//

#ifndef RENDERER_COMPLEMENT_H
#define RENDERER_COMPLEMENT_H


#include "../ClosedThing.h"

class Complement : public ClosedThing {
public:
    Complement(ClosedThing* a) : a(a) { }

    virtual Box getBoundingBox() const override {
        return a->getBoundingBox();
    }

    virtual RayIntersection intersect(Ray ray) const override {
        RayIntersection inter = a->intersect(ray);
        if (!inter.isValid)
            return inter;

        inter.norm = -inter.norm;

        return inter;
    };

    virtual bool isInside(Point3D point) const override {
        return !a->isInside(point);
    }

private:
    ClosedThing* a;
};


#endif //RENDERER_COMPLEMENT_H
