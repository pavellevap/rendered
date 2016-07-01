//
// Created by pavel on 02.05.16.
//

#ifndef RENDERER_COMPACT_H
#define RENDERER_COMPACT_H

#include "Linalg/Point.h"

class Closed {
    virtual bool isInside(Point3D point) const = 0;
};


#endif //RENDERER_COMPACT_H
