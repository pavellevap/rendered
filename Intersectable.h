//
// Created by pavel on 29.04.16.
//

#ifndef RENDERER_INTERSECTABLE_H
#define RENDERER_INTERSECTABLE_H

#include "Linalg/Ray.h"
#include "Linalg/Vector.h"
#include "Material.h"
#include "RGB/RGBImage.h"


class RayIntersection {
public:
    RayIntersection() : isValid(false), material(nullptr), texture(nullptr) {}
    bool isValid;

    double rayCoord;
    Point3D point;

    Vector3D norm;
    Point2D texCoords; // texture coordinates

    const Material* material;
    const RGBImage* texture;

    bool operator < (const RayIntersection& other) const {
        if (!isValid)
            return false;
        if (!other.isValid)
            return true;
        return rayCoord < other.rayCoord;
    }
};


class Intersectable {
public:
    virtual RayIntersection intersect(Ray ray) const = 0;
};




#endif //RENDERER_INTERSECTABLE_H
