//
// Created by pavel on 30.04.16.
//

#ifndef RENDERER_CUBOID_H
#define RENDERER_CUBOID_H

#include "Quadrangle.h"
#include "../ClosedThing.h"
#include "../Linalg/Matrix.h"

class Parallelepiped : public Primitive, public ClosedThing {
public:

    virtual RayIntersection intersect(Ray ray) const override {
        RayIntersection inter;

        Point3D otherVertex = (Vector3D(vertex) + edges[0] + edges[1] + edges[2]).toPoint3D();
        inter = std::min(inter, intersectParallelogram(ray, vertex, edges[0], edges[1]));
        inter = std::min(inter, intersectParallelogram(ray, vertex, edges[1], edges[2]));
        inter = std::min(inter, intersectParallelogram(ray, vertex, edges[2], edges[0]));
        inter = std::min(inter, intersectParallelogram(ray, otherVertex, -edges[1], -edges[0]));
        inter = std::min(inter, intersectParallelogram(ray, otherVertex, -edges[2], -edges[1]));
        inter = std::min(inter, intersectParallelogram(ray, otherVertex, -edges[0], -edges[2]));

        return inter;
    }

    virtual Box getBoundingBox() const override {
        Box box = Box(vertex, vertex);

        Point3D otherVertex;

        otherVertex = (Vector3D(vertex) + edges[0]).toPoint3D();
        box = Box::unite(box, Box(otherVertex, otherVertex));

        otherVertex = (Vector3D(vertex) + edges[1]).toPoint3D();
        box = Box::unite(box, Box(otherVertex, otherVertex));

        otherVertex = (Vector3D(vertex) + edges[2]).toPoint3D();
        box = Box::unite(box, Box(otherVertex, otherVertex));

        otherVertex = (Vector3D(vertex) + edges[0] + edges[1]).toPoint3D();
        box = Box::unite(box, Box(otherVertex, otherVertex));

        otherVertex = (Vector3D(vertex) + edges[1] + edges[2]).toPoint3D();
        box = Box::unite(box, Box(otherVertex, otherVertex));

        otherVertex = (Vector3D(vertex) + edges[2] + edges[0]).toPoint3D();
        box = Box::unite(box, Box(otherVertex, otherVertex));

        otherVertex = (Vector3D(vertex) + edges[0] + edges[1] + edges[2]).toPoint3D();
        box = Box::unite(box, Box(otherVertex, otherVertex));

        return box;
    }

    virtual bool isInside(Point3D point) const {
        const static double EPS = 1e-5;
        Matrix3x3 matrix(edges[0], edges[1], edges[2]);
        matrix = matrix.inverse();
        Vector3D v = dot(matrix, Vector3D(vertex, point));
        return v.x > EPS && v.x < 1 - EPS && v.y > EPS && v.y < 1 - EPS && v.z > EPS && v.z < 1 - EPS;
    }

    virtual void setMaterial(const Material* material) override {
        this->material = material;
    }

    virtual const Material* getMaterial() const override {
        return material;
    }

    virtual void setTexture (const RGBImage* texture) override {
        //this->texture = texture;
    }

    virtual const RGBImage* getTexture() const override {
        //return texture;
        return nullptr;
    }

//private:
    Point3D vertex;
    Vector3D edges[3];

    const Material* material;
    //const RGBImage* texture;

private:
    RayIntersection intersectParallelogram(Ray ray, Point3D p, Vector3D v1, Vector3D v2) const {
        Quadrangle parallelorgam;
        parallelorgam.material = material;
        parallelorgam.texture = nullptr;
        parallelorgam.norms[0] = parallelorgam.norms[1] = parallelorgam.norms[2] = parallelorgam.norms[3] = cross(v1, v2).normalize();
        parallelorgam.points[0] = p;
        parallelorgam.points[1] = (Vector3D(p) + v1).toPoint3D();
        parallelorgam.points[2] = (Vector3D(p) + v1 + v2).toPoint3D();
        parallelorgam.points[3] = (Vector3D(p) + v2).toPoint3D();

        return parallelorgam.intersect(ray);

        /*const static double EPS = 1e-5;
        RayIntersection inter;

        double b = dot(v1, cross(v2, ray.dir));
        if (std::abs(b) < EPS)
            return inter; // invalid
        double a = dot(v1, cross(v2, ray.origin));
        inter.rayCoord = -a / b;
        if (inter.rayCoord < EPS)
            return inter; // invalid
        inter.point = ray.getPoint(inter.rayCoord);
        Vector3D v(p, inter.point);
        if (dot(v, v1) < 0 || dot(v, v1) > v1.squaredLen() || dot(v, v2) < 0 || dot(v, v2) > v2.squaredLen())
            return inter; // invalid
        inter.isValid = true;
        inter.norm = cross(v1, v2).normalize();
        return inter;*/
    }
};


#endif //RENDERER_CUBOID_H
