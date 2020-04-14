//
// Created by pavel on 08.04.16.
//

#ifndef RENDERER_SPHERE_H
#define RENDERER_SPHERE_H

#include "Primitive.h"
#include "../ClosedThing.h"

class Sphere : public Primitive, public ClosedThing {
public:
    Sphere() : center(Point3D()), radius(0), material(nullptr), texture(nullptr) {}

    RayIntersection intersect(Ray ray) const override;

    Box getBoundingBox() const override;

    bool isInside(Point3D point) const override {
        const static double EPS = 1e-5;
        return Vector3D(point, center).len() < radius - EPS;
    }

    void setMaterial(const Material* material) override {
        this->material = material;
    }

    const Material* getMaterial() const override {
        return material;
    }

    void setTexture (const RGBImage* texture) override {
        this->texture = texture;
    }

    const RGBImage* getTexture() const override {
        return texture;
    }

//private:
    Point3D center;
    double radius;

    const Material* material;
    const RGBImage* texture;
};


#endif //RENDERER_SPHERE_H
