//
// Created by pavel on 08.04.16.
//

#ifndef RENDERER_PRIMITIVE_H
#define RENDERER_PRIMITIVE_H

#include "../Thing.h"

class Primitive : public Thing {
public:
    virtual void setMaterial(const Material* material) = 0;

    virtual const Material* getMaterial() const = 0;

    virtual void setTexture(const RGBImage* material) = 0;

    virtual const RGBImage* getTexture() const = 0;
};


#endif //RENDERER_PRIMITIVE_H
