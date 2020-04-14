//
// Created by pavel on 09.04.16.
//

#ifndef RENDERER_OBJECT_H
#define RENDERER_OBJECT_H

#include "Primitives/Primitive.h"
#include "KDTree.h"

// Set of primitives related to the overall texture
class Object : public Thing {
public:
    explicit Object(const std::vector<Primitive*>& primitives) : primitives(primitives),
            tree(primitivesToThings(primitives)) {}

    RayIntersection intersect(Ray ray) const override;

    Box getBoundingBox() const override;

    void setTexture(const RGBImage* texture);

    void setMaterial(const Material* material);

private:
    std::vector<Primitive*> primitives;
    KDTree tree;

    static std::vector<Thing*> primitivesToThings(const std::vector<Primitive*>& primitives);
};


#endif //RENDERER_OBJECT_H
