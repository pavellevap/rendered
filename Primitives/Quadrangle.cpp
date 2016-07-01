//
// Created by pavel on 09.04.16.
//

#include "Quadrangle.h"

RayIntersection Quadrangle::intersect(Ray ray) const {
        // it's easier to keep two triangles instead of 4 points. But it was forbidden.
        Triangle triangle1;
        triangle1.points[0] = points[0];
        triangle1.points[1] = points[1];
        triangle1.points[2] = points[2];
        triangle1.texCoords[0] = texCoords[0];
        triangle1.texCoords[1] = texCoords[1];
        triangle1.texCoords[2] = texCoords[2];
        triangle1.norms[0] = norms[0];
        triangle1.norms[1] = norms[1];
        triangle1.norms[2] = norms[2];
        triangle1.material = material;
        triangle1.texture = texture;
        Triangle triangle2;
        triangle2.points[0] = points[0];
        triangle2.points[1] = points[2];
        triangle2.points[2] = points[3];
        triangle2.texCoords[0] = texCoords[0];
        triangle2.texCoords[1] = texCoords[2];
        triangle2.texCoords[2] = texCoords[3];
        triangle2.norms[0] = norms[0];
        triangle2.norms[1] = norms[2];
        triangle2.norms[2] = norms[3];
        triangle2.material = material;
        triangle2.texture = texture;

        RayIntersection inter1 = triangle1.intersect(ray);
        RayIntersection inter2 = triangle2.intersect(ray);
        if (!inter1.isValid)
                return inter2;
        if (!inter2.isValid)
                return inter1;

        return std::min(inter1, inter2);
}