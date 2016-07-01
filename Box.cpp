//
// Created by pavel on 13.04.16.
//

#include "Box.h"

RayIntersection Box::intersect(Ray ray) const {
    RayIntersection inter;
    inter.isValid = false;

    if (isInside(ray.origin)) {
        inter.isValid = true;
        inter.rayCoord = 0;
        inter.point = ray.origin;
        return inter;
    }

    for (Dimension dim : {Dimension::X, Dimension::Y, Dimension::Z}) {
        if (ray.dir[dim] > 0 && ray.origin[dim] < min[dim]) {
            inter.rayCoord = (min[dim] - ray.origin[dim]) / ray.dir[dim];
            inter.point = ray.getPoint(inter.rayCoord);
            if (isInside(inter.point)) {
                inter.isValid = true;
                return inter;
            }
        }
        else if (ray.dir[dim] < 0 && ray.origin[dim] > max[dim]) {
            inter.rayCoord = (max[dim] - ray.origin[dim]) / ray.dir[dim];
            inter.point = ray.getPoint(inter.rayCoord);
            if (isInside(inter.point)) {
                inter.isValid = true;
                return inter;
            }
        }
    }
    return inter;
}

bool Box::isInside(Point3D point) const {
    static const double EPS = 1e-5;

    return point.x >= min.x - EPS && point.x <= max.x + EPS &&
           point.y >= min.y - EPS && point.y <= max.y + EPS &&
           point.z >= min.z - EPS && point.z <= max.z + EPS;
}

Box Box::unite(Box b1, Box b2) {
    b1.min.x = std::min(b1.min.x, b2.min.x);
    b1.min.y = std::min(b1.min.y, b2.min.y);
    b1.min.z = std::min(b1.min.z, b2.min.z);

    b1.max.x = std::max(b1.max.x, b2.max.x);
    b1.max.y = std::max(b1.max.y, b2.max.y);
    b1.max.z = std::max(b1.max.z, b2.max.z);

    return b1;
}