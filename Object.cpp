//
// Created by pavel on 09.04.16.
//

#include "Object.h"

void Object::setTexture(const RGBImage* texture) {
    for (auto primitive : primitives)
        primitive->setTexture(texture);
}

void Object::setMaterial(const Material* material) {
    for (auto primitive : primitives)
        primitive->setMaterial(material);
}

Box Object::getBoundingBox() const {
    Box box = primitives[0]->getBoundingBox();
    for (auto primitive : primitives)
        box = Box::unite(box, primitive->getBoundingBox());
    return box;
}

RayIntersection Object::intersect(Ray ray) const {
    return tree.intersect(ray);
}

std::vector<Thing*> Object::primitivesToThings(const std::vector<Primitive*>& primitives) {
    std::vector<Thing*> things;
    for (auto primitive : primitives)
        things.push_back(primitive);
    return things;
}