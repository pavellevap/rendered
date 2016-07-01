//
// Created by pavel on 08.04.16.
//

#ifndef RENDERER_RGBCOLOR_H
#define RENDERER_RGBCOLOR_H

enum class RGB {R, G, B};

class RGBColor {
public:
    RGBColor(double r = 0, double g = 0, double b = 0) : r(r), g(g), b(b) {}

    double r, g, b;

    double& operator [] (RGB channel) {
        if (channel == RGB::R)
            return r;
        else if (channel == RGB::G)
            return g;
        else if (channel == RGB::B)
            return b;
        else
            throw "there is no such channel\n";
    }

    const double& operator [] (RGB channel) const {
        if (channel == RGB::R)
            return r;
        else if (channel == RGB::G)
            return g;
        else if (channel == RGB::B)
            return b;
        else
            throw "there is no such channel\n";
    }

    static const RGBColor WHITE;
    static const RGBColor BLACK;
    static const RGBColor SILVER;
    static const RGBColor RED;
    static const RGBColor GREEN;
    static const RGBColor BLUE;
    static const RGBColor YELLOW;
    static const RGBColor ORANGE;
    static const RGBColor VIOLET;
    static const RGBColor PINK;
};

#endif //RENDERER_RGBCOLOR_H
