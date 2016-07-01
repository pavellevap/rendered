//
// Created by pavel on 09.04.16.
//

#ifndef RENDERER_QUADRANGLE_H
#define RENDERER_QUADRANGLE_H


#include "Triangle.h"

class Quadrangle : public Primitive {
public:
    virtual RayIntersection intersect(Ray ray) const override;

    virtual Box getBoundingBox() const override {
        Box box;
        box.max = box.min = points[0];

        for (int i = 1; i < 4; i++)
            box = Box::unite(box, Box(points[i], points[i]));

        return box;
    }

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
    Point3D points[4];
    Vector3D norms[4];
    Point2D texCoords[4]; // texture coordinates

    const Material* material;
    const RGBImage* texture;
};


#endif //RENDERER_QUADRANGLE_H
