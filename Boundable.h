//
// Created by pavel on 29.04.16.
//

#ifndef RENDERER_BOUNDABLE_H
#define RENDERER_BOUNDABLE_H


#include "Box.h"

class Boundable {
    virtual Box getBoundingBox() const = 0;
};


#endif //RENDERER_BOUNDABLE_H
