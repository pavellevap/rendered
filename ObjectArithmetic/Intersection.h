//
// Created by pavel on 02.05.16.
//

#ifndef RENDERER_INTERSECTION_H
#define RENDERER_INTERSECTION_H


#include "../ClosedThing.h"

class Intersection : public ClosedThing {
public:
    Intersection(ClosedThing* a, ClosedThing* b) : a(a), b(b) { }

    Box getBoundingBox() const override {
        return a->getBoundingBox();
    }

    RayIntersection intersect(Ray ray) const override {
        RayIntersection inter;
        bool inside[2] = {a->isInside(ray.origin), b->isInside(ray.origin)};
        double dist = 0;

        while (true) {
            RayIntersection interA = a->intersect(ray);
            RayIntersection interB = b->intersect(ray);
            if (!interA.isValid || !interB.isValid)
                break;

            RayIntersection firstInter = std::min(interA, interB);
            if (inside[0] && inside[1]) {
                inter = firstInter;
                break;
            }

            if (interA < interB)
                inside[0] = !inside[0];
            if (interB < interA)
                inside[1] = !inside[1];

            if (inside[0] && inside[1]) {
                inter = firstInter;
                break;
            }

            dist += firstInter.rayCoord;
            ray = Ray(firstInter.point, ray.dir);
        }

        if (inter.isValid)
            inter.rayCoord += dist;
        return inter;
    };

    bool isInside(Point3D point) const override {
        return a->isInside(point) || b->isInside(point);
    }

private:
    ClosedThing* a;
    ClosedThing* b;
};


#endif //RENDERER_INTERSECTION_H
