//
// Created by pavel on 08.04.16.
//

#include "Triangle.h"

RayIntersection Triangle::intersect(Ray ray) const {
    static const double EPS = 1e-5;

    RayIntersection inter;

    Vector3D a(points[0], points[1]);
    Vector3D b(points[0], points[2]);
    Vector3D c = -ray.dir;
    Vector3D d(points[0], ray.origin);
    Vector3D A = cross(b, c);
    Vector3D B = cross(c, a);
    Vector3D C = cross(a, b);

    double det = dot(a, A);
    if (fabs(det) < EPS) {
        inter.isValid = false;
        return inter;
    }
    double u = dot(d, A) / det;
    if (u < 0) {
        inter.isValid = false;
        return inter;
    }
    double v = dot(d, B) / det;
    if (v < 0 || (u + v) > 1) {
        inter.isValid = false;
        return inter;
    }
    double t = dot(d, C) / det;
    if (t < EPS) {
        inter.isValid = false;
        return inter;
    }

    inter.isValid = true;
    inter.rayCoord = t;
    inter.point = ray.getPoint(inter.rayCoord);

    inter.norm = norms[0] * (1 - u - v) + norms[1] * u + norms[2] * v;
    inter.norm.normalize();

    inter.texCoords.x = (1 - u - v) * texCoords[0].x + u * texCoords[1].x + v * texCoords[2].x;
    inter.texCoords.y = (1 - u - v) * texCoords[0].y + u * texCoords[1].y + v * texCoords[2].y;

    inter.material = material;
    inter.texture = texture;

    return inter;
}

Box Triangle::getBoundingBox() const {
    Box box;
    box.max = box.min = points[0];

    for (int i = 1; i < 3; i++)
        box = Box::unite(box, Box(points[i], points[i]));

    return box;
}