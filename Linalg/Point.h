//
// Created by pavel on 08.04.16.
//

#ifndef RENDERER_POINT_H
#define RENDERER_POINT_H

enum class Dimension {X, Y, Z, W};

class Point2D {
public:
    Point2D() {}
    Point2D(double x, double y) : x(x), y(y) {}
    double x, y;
};

class Point3D {
public:
    Point3D() {}
    Point3D(double x, double y, double z) : x(x), y(y), z(z) {}
    double x, y, z;

    double& operator [] (Dimension dim) {
        if (dim == Dimension::X)
            return x;
        if (dim == Dimension::Y)
            return y;
        if (dim == Dimension::Z)
            return z;
        else
            throw "There is no such dimension\n";
    }

    const double& operator [] (Dimension dim) const {
        if (dim == Dimension::X)
            return x;
        if (dim == Dimension::Y)
            return y;
        if (dim == Dimension::Z)
            return z;
        else
            throw "There is no such dimension\n";
    }
};

class Point4D { // this is not used.
    double x, y, z, w;
};


#endif //RENDERER_POINT_H
