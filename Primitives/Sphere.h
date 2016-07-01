//
// Created by pavel on 08.04.16.
//

#ifndef RENDERER_SPHERE_H
#define RENDERER_SPHERE_H

#include "Primitive.h"
#include "../ClosedThing.h"

class Sphere : public Primitive, public ClosedThing {
public:
    virtual RayIntersection intersect(Ray ray) const override;

    virtual Box getBoundingBox() const override;

    virtual bool isInside(Point3D point) const override {
        const static double EPS = 1e-5;
        return Vector3D(point, center).len() < radius - EPS;
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
    Point3D center;
    double radius;

    const Material* material;
    //const RGBImage* texture;
};


#endif //RENDERER_SPHERE_H
