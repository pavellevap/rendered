//
// Created by pavel on 08.04.16.
//

#ifndef RENDERER_SCENE_H
#define RENDERER_SCENE_H

#include <string>
#include <vector>
#include "Primitives/Primitive.h"
#include "Object.h"
#include "LightSources/AmbientLight.h"
#include "LightSources/PointLight.h"
#include "Camera.h"

class Scene {
public:
    Scene() : background(RGBColor::BLACK) {}

    RGBColor trace(Ray ray, size_t maxDepth = 0) const {
        RGBColor color = background;

        RayIntersection inter;
        inter.isValid = false;
        for (auto object : things) {
            RayIntersection newInter = object->intersect(ray);
            if (newInter.isValid && (!inter.isValid || newInter.rayCoord < inter.rayCoord))
                inter = newInter;
        }

        if (inter.isValid && dot(ray.dir, inter.norm) > 0)
            return trace(Ray(inter.point, ray.dir), maxDepth);
        else if (inter.isValid) {
            if (inter.material == nullptr)
                inter.material = Material::BLACK_HOLE;

            if (inter.texture != nullptr) {
                RGBColor materialColor = inter.texture->getPixel(
                        size_t(inter.texCoords.y * (inter.texture->height() - 1.0)),
                        size_t(inter.texCoords.x * (inter.texture->width() - 1.0)));
                inter.material = Material::changeColor(inter.material, materialColor);
            }

            double refractivity = 0;
            RGBColor refractedColor = RGBColor::BLACK;
            if (inter.material->refractivity > 0 && inter.material->refractiveIndex > 0 && maxDepth > 0) {
                Vector3D alpha1_sin = ray.dir - inter.norm * dot(inter.norm, ray.dir);
                Vector3D alpha2_sin = alpha1_sin / inter.material->refractiveIndex;
                if (alpha2_sin.squaredLen() < 1) {
                    Vector3D alpha2_cos = -inter.norm * std::sqrt(1 - alpha2_sin.squaredLen());
                    refractedColor = trace(Ray(inter.point, alpha2_sin + alpha2_cos), maxDepth - 1);
                    refractivity = inter.material->refractivity * (1 - alpha2_sin.squaredLen());
                }
            }

            color = calculateLighting(inter.point, inter.norm, -ray.dir, inter.material);

            RGBColor reflectedColor = RGBColor::BLACK;
            if (inter.material->reflectivity > 0 && maxDepth > 0)
                reflectedColor = trace(Ray(inter.point, ray.dir - inter.norm * dot(inter.norm, ray.dir) * 2),
                                       maxDepth - 1);

            color = add(mul(color, (1 - inter.material->reflectivity)),
                        mul(reflectedColor, inter.material->reflectivity));
            color = add(mul(color, (1 - refractivity)),
                        mul(refractedColor, refractivity));
        }

        return color;
    }

    RGBColor calculateLighting(Point3D point, Vector3D norm, Vector3D toViewer, const Material* material) const {
        RGBColor res = material->emission;
        if (ambientLight != nullptr)
            res = combine(res, mul(material->ambient, ambientLight->color));

        for (PointLight* source : pointLights) {
            const double EPS = 1e-5;

            Ray ray(point, source->position);
            Vector3D toLight = ray.dir;
            double dist = Vector3D(point, source->position).len();

            bool inShade = false;
            for (auto object : things) {
                RayIntersection inter = object->intersect(ray);
                if (inter.isValid && inter.rayCoord > EPS && inter.rayCoord < dist - EPS &&
                        (inter.material == nullptr || inter.material->refractivity == 0) && dot(inter.norm, ray.dir) < 0)
                    inShade = true;
            }
            if (inShade)
                continue;

            double power;
            if (dist == 0 || source->quadricAttenuation == 0)  // may be bad
                power = 1.0;
            else
                power = std::min(1.0, 1.0 / source->quadricAttenuation / dist / dist);

            double cosine1 = dot(toLight, norm);
            double cosine2 = dot((toLight + toViewer).normalize(), norm);

            RGBColor diffuse = RGBColor::BLACK;
            if (cosine1 > 0)
                diffuse = mul(mul(material->diffuse, source->color), cosine1 * power);

            RGBColor specular = RGBColor::BLACK;
            if (cosine1 > 0 && cosine2 > 0)
                specular = mul(mul(material->specular, source->color), std::pow(cosine2, material->shininess) * power);

            res = combine(res, diffuse);
            res = combine(res, specular);
        }

        return res;
    }

//private:
    std::vector<Thing*> things;

    AmbientLight* ambientLight;
    std::vector<PointLight*> pointLights;

    RGBColor background;

    Camera camera;

private:
    static RGBColor add(RGBColor c1, RGBColor c2) {
        RGBColor res;
        for (RGB channel : {RGB::R, RGB::G, RGB::B})
            res[channel] = c1[channel] + c2[channel];
        return res;
    }

    static RGBColor combine(RGBColor c1, RGBColor c2) {
        RGBColor res;
        for (RGB channel : {RGB::R, RGB::G, RGB::B})
            res[channel] = 1 - (1 - c1[channel]) * (1 - c2[channel]);
        return res;
    }

    static RGBColor mul (RGBColor c1, RGBColor c2) {
        RGBColor res;
        for (RGB channel : {RGB::R, RGB::G, RGB::B})
            res[channel] = c1[channel] * c2[channel];
        return res;
    }

    static RGBColor mul (RGBColor c1, double f) {
        RGBColor res;
        for (RGB channel : {RGB::R, RGB::G, RGB::B})
            res[channel] = std::min(1.0 ,std::max(0.0, c1[channel] * f));
        return res;
    }
};


#endif //RENDERER_SCENE_H
