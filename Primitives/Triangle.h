//
// Created by pavel on 08.04.16.
//

#ifndef RENDERER_TRIANGLE_H
#define RENDERER_TRIANGLE_H

#include "Primitive.h"

class Triangle : public Primitive {
public:

    virtual RayIntersection intersect(Ray ray) const override;

    virtual Box getBoundingBox() const override;

    virtual void setMaterial(const Material* material) override {
        this->material = material;
    }

    virtual const Material* getMaterial() const override {
        return material;
    }

    virtual void setTexture (const RGBImage* texture) override {
        this->texture = texture;
    }

    virtual const RGBImage* getTexture() const override {
        return texture;
    }

//private:
    Point3D points[3];
    Vector3D norms[3];
    Point2D texCoords[3]; // texture coordinates

    const Material* material;
    const RGBImage* texture;
};


#endif //RENDERER_TRIANGLE_H
