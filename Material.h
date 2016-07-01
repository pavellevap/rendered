//
// Created by pavel on 14.04.16.
//

#ifndef RENDERER_MATERIAL_H
#define RENDERER_MATERIAL_H


#include "RGB/RGBColor.h"

class Material {
public:
    Material() : ambient(RGBColor::BLACK), diffuse(RGBColor::BLACK),
                 specular(RGBColor::BLACK), emission(RGBColor::BLACK),
                 shininess(0), reflectivity(0) {}
    Material(RGBColor ambient, RGBColor diffuse, RGBColor specular, RGBColor emission,
             double shininess, double reflectivity, double refractivity, double refractiveIndex) :
                                    ambient(ambient), diffuse(diffuse), specular(specular), emission(emission),
                                    shininess(shininess), reflectivity(reflectivity), refractivity(refractivity),
                                    refractiveIndex(refractiveIndex) {}

    static Material* changeColor(const Material* material, RGBColor color) {
        return new Material(color, color, material->specular, material->emission,
                            material->shininess, material->reflectivity, material->refractivity,
                            material->refractiveIndex);
    }

    static Material* otherSide(const Material* material) {
        return new Material(material->ambient, material->diffuse, material->specular, material->emission,
                            material->shininess, material->reflectivity, material->refractivity,
                            1 / material->refractiveIndex);
    }

    static const Material* BLACK_HOLE;
    static const Material* WHITE_HOLE;
    static const Material* METAL;
    static const Material* POLISHED_METAL;
    static const Material* MIRROW;
    static const Material* WATER;

//private:
    RGBColor ambient;
    RGBColor diffuse;
    RGBColor specular;
    RGBColor emission;
    double shininess;
    double reflectivity;

    double refractivity;
    double refractiveIndex; // n2/n1
};




#endif //RENDERER_MATERIAL_H
