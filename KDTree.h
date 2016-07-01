//
// Created by pavel on 12.04.16.
//

#ifndef RENDERER_KDTREE_H
#define RENDERER_KDTREE_H

#include "Thing.h"

class KDTree : public Intersectable {
public:
    KDTree(const std::vector<Thing*>& things);

    RayIntersection intersect(Ray ray) const;

private:
    class Plane {
    public:
        Dimension dim;
        double coordinate;
    };

    class Node {
    public:
        std::vector<size_t> objectIndices;
        Plane dividingPlane;
        Box boundingBox;
        size_t leftNodeIndex;
        size_t rightNodeIndex;
    };

    std::vector<Node> nodes;
    static const size_t NULL_NODE_INDEX;
    size_t rootIndex;

    std::vector<Thing*> data;

    size_t buildTree(std::vector<size_t>& objectIndices, Dimension dim);

    void findIntersection(size_t nodeIndex, Ray ray, RayIntersection boxInter, RayIntersection& curInter) const;
};


#endif //RENDERER_KDTREE_H
