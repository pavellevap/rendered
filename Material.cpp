//
// Created by pavel on 14.04.16.
//

#include "Material.h"


const Material* Material::BLACK_HOLE     = new Material(RGBColor::BLACK,  RGBColor::BLACK,  RGBColor::BLACK, RGBColor::BLACK, 0, 0, 0, 0);
const Material* Material::WHITE_HOLE     = new Material(RGBColor::WHITE,  RGBColor::WHITE,  RGBColor::WHITE, RGBColor::WHITE, 0, 0, 0, 0);
const Material* Material::METAL          = new Material(RGBColor::SILVER, RGBColor::SILVER, RGBColor::WHITE, RGBColor::BLACK, 20, 0, 0, 0);
const Material* Material::POLISHED_METAL = new Material(RGBColor::SILVER, RGBColor::SILVER, RGBColor::WHITE, RGBColor::BLACK, 20, 0.2f, 0, 0);
const Material* Material::MIRROW         = new Material(RGBColor::WHITE,  RGBColor::WHITE,  RGBColor::WHITE, RGBColor::BLACK, 20, 0.9f, 0, 0);
const Material* Material::WATER          = new Material(RGBColor::WHITE,  RGBColor::WHITE,  RGBColor::WHITE, RGBColor::BLACK, 20, 0.5f, 0.9, 2.0f);