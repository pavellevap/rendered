//
// Created by pavel on 08.04.16.
//

#ifndef RENDERER_VECTOR_H
#define RENDERER_VECTOR_H


#include <cmath>
#include "Point.h"

class Vector3D {
public:
    Vector3D() = default;
    Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}
    Vector3D(Point3D p1, Point3D p2) : x(p2.x - p1.x), y(p2.y - p1.y), z(p2.z - p1.z) {}
    explicit Vector3D(Point3D p) : x(p.x), y(p.y), z(p.z) {}

    double x, y, z;

    double& operator [] (Dimension dim) {
        if (dim == Dimension::X)
            return x;
        if (dim == Dimension::Y)
            return y;
        if (dim == Dimension::Z)
            return z;
        else
            throw std::invalid_argument("There is no such dimension\n");
    }

    const double& operator [] (Dimension dim) const {
        if (dim == Dimension::X)
            return x;
        if (dim == Dimension::Y)
            return y;
        if (dim == Dimension::Z)
            return z;
        else
            throw std::invalid_argument("There is no such dimension\n");
    }

    Point3D toPoint3D() const {
        return Point3D(x, y, z);
    }

    double len() const {
        return sqrt(squaredLen());
    }

    double squaredLen() const {
        return x * x + y * y + z * z;
    }

    Vector3D normalize() { // mud
        *this /= len();
        return *this;
    }

    friend Vector3D cross(Vector3D v1, Vector3D v2) {
        return Vector3D{v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x};
    }

    friend double dot(Vector3D v1, Vector3D v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    friend Vector3D operator + (Vector3D a, Vector3D b) {
        return Vector3D{a.x + b.x, a.y + b.y, a.z + b.z};
    }

    friend void operator += (Vector3D& a, Vector3D b) {
        a.x += b.x;
        a.y += b.y;
        a.z += b.z;
    }

    friend Vector3D operator - (Vector3D a, Vector3D b) {
        return Vector3D{a.x - b.x, a.y - b.y, a.z - b.z};
    }

    friend void operator -= (Vector3D& a, Vector3D b) {
        a.x -= b.x;
        a.y -= b.y;
        a.z -= b.z;
    }

    friend Vector3D operator * (Vector3D v, double a) {
        return Vector3D{v.x * a, v.y * a, v.z * a};
    }

    friend void operator *= (Vector3D& v, double a) {
        v.x *= a;
        v.y *= a;
        v.z *= a;
    }

    friend Vector3D operator * (Vector3D a, Vector3D b) {
        return Vector3D{a.x * b.x, a.y * b.y, a.z * b.z};
    }

    friend void operator *= (Vector3D& a, Vector3D b) {
        a.x *= b.x;
        a.y *= b.y;
        a.z *= b.z;
    }

    friend Vector3D operator / (Vector3D v, double a) {
        return Vector3D{v.x / a, v.y / a, v.z / a};
    }

    friend void operator /= (Vector3D& v, double a) {
        v.x /= a;
        v.y /= a;
        v.z /= a;
    }

    friend Vector3D operator / (Vector3D a, Vector3D b) {
        return Vector3D{a.x / b.x, a.y / b.y, a.z / b.z};
    }

    friend void operator /= (Vector3D& a, Vector3D b) {
        a.x /= b.x;
        a.y /= b.y;
        a.z /= b.z;
    }

    friend Vector3D operator - (Vector3D v) {
        return Vector3D{-v.x, -v.y, -v.z};
    }
};


#endif //RENDERER_VECTOR_H
