//
// Created by pavel on 09.04.16.
//

#ifndef RENDERER_POINTLIGHTSOURCE_H
#define RENDERER_POINTLIGHTSOURCE_H


#include <cstddef>
#include "../Linalg/Point.h"
#include "../RGB/RGBColor.h"

class PointLight {
public:
//private:
    Point3D position;
    RGBColor color;
    double quadricAttenuation;
};


#endif //RENDERER_POINTLIGHTSOURCE_H
