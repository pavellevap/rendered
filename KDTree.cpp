//
// Created by pavel on 12.04.16.
//

#include "KDTree.h"

const size_t KDTree::NULL_NODE_INDEX = size_t(-1);

KDTree::KDTree(const std::vector<Thing*>& things) : data(things) {
    std::vector<size_t> objectIndices(things.size());
    for (size_t i = 0; i < things.size(); i++)
        objectIndices[i] = i;
    rootIndex = buildTree(objectIndices, Dimension::X);
}

RayIntersection KDTree::intersect(Ray ray) const {
    RayIntersection inter;
    if (rootIndex != NULL_NODE_INDEX)
        findIntersection(rootIndex, ray, nodes[rootIndex].boundingBox.intersect(ray), inter);
    return inter;
}

size_t KDTree::buildTree(std::vector<size_t>& objectIndices, Dimension dim) {
    if (objectIndices.size() == 0)
        return NULL_NODE_INDEX;

    Node newNode;

    newNode.boundingBox = data[objectIndices[0]]->getBoundingBox();
    for (size_t i = 1; i < objectIndices.size(); i++)
        newNode.boundingBox = Box::unite(newNode.boundingBox, data[objectIndices[i]]->getBoundingBox());

    newNode.dividingPlane.dim = dim;
    newNode.dividingPlane.coordinate = (newNode.boundingBox.min[dim] + newNode.boundingBox.max[dim]) / 2;

    std::vector<size_t> leftObjectIndices;
    std::vector<size_t> rightObjectIndices;

    for (size_t index : objectIndices) {
        Box box = data[index]->getBoundingBox();
        if (box.max[dim] < newNode.dividingPlane.coordinate)
            leftObjectIndices.push_back(index);
        else if (box.min[dim] > newNode.dividingPlane.coordinate)
            rightObjectIndices.push_back(index);
        else
            newNode.objectIndices.push_back(index);
    }

    Dimension nextDim;
    if (dim == Dimension::X)
        nextDim = Dimension::Y;
    else if (dim == Dimension::Y)
        nextDim = Dimension::Z;
    else
        nextDim = Dimension::X;

    newNode.leftNodeIndex = buildTree(leftObjectIndices, nextDim);
    newNode.rightNodeIndex = buildTree(rightObjectIndices, nextDim);

    size_t newNodeIndex = nodes.size();
    nodes.push_back(newNode);
    return newNodeIndex;
}

void KDTree::findIntersection(size_t nodeIndex, Ray ray, RayIntersection boxInter, RayIntersection& curInter) const {
    const Node& curNode = nodes[nodeIndex];

    if (!boxInter.isValid || curInter < boxInter)
        return;

    for (size_t index : curNode.objectIndices)
        curInter = std::min(curInter, data[index]->intersect(ray));

    if (nodes[nodeIndex].leftNodeIndex == NULL_NODE_INDEX && nodes[nodeIndex].rightNodeIndex == NULL_NODE_INDEX)
        return;
    else if (nodes[nodeIndex].leftNodeIndex == NULL_NODE_INDEX)
        findIntersection(curNode.rightNodeIndex, ray, nodes[curNode.rightNodeIndex].boundingBox.intersect(ray), curInter);
    else if (nodes[nodeIndex].rightNodeIndex == NULL_NODE_INDEX)
        findIntersection(curNode.leftNodeIndex, ray, nodes[curNode.leftNodeIndex].boundingBox.intersect(ray), curInter);
    else {
        const Node& leftNode = nodes[nodes[nodeIndex].leftNodeIndex];
        const Node& rightNode = nodes[nodes[nodeIndex].rightNodeIndex];
        RayIntersection leftBoxInter = leftNode.boundingBox.intersect(ray);
        RayIntersection rightBoxInter = rightNode.boundingBox.intersect(ray);

        if (leftBoxInter < rightBoxInter) {
            findIntersection(curNode.leftNodeIndex, ray, leftBoxInter, curInter);
            findIntersection(curNode.rightNodeIndex, ray, rightBoxInter, curInter);
        }
        else {
            findIntersection(curNode.rightNodeIndex, ray, rightBoxInter, curInter);
            findIntersection(curNode.leftNodeIndex, ray, leftBoxInter, curInter);
        }
    }
}