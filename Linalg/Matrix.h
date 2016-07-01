//
// Created by pavel on 08.04.16.
//

#ifndef RENDERER_MATRIX_H
#define RENDERER_MATRIX_H

#include <vector>
#include <cstddef>
#include "Vector.h"

class Matrix {
public:
    Matrix(size_t width, size_t height, double value = 0) :
            data(std::vector< std::vector<double> >(height, std::vector<double>(width, value))) {}

    size_t width() const {
        return data[0].size();
    }

    size_t height() const {
        return data.size();
    }

    double get(size_t i, size_t j) const {
        return data[i][j];
    }

    void set(size_t i, size_t j, double value) {
        data[i][j] = value;
    }
private:
    std::vector< std::vector<double> > data;
};


class Matrix3x3 {
public:
    Matrix3x3() {}
    Matrix3x3(Vector3D col1, Vector3D col2, Vector3D col3) : cols{col1, col2, col3} { }
    Matrix3x3 transpose() const {
        Matrix3x3 res = *this;
        std::swap(res.cols[1].x, res.cols[0].y);
        std::swap(res.cols[2].x, res.cols[0].z);
        std::swap(res.cols[2].y, res.cols[1].z);
        return res;
    }
    double determinant() const {
        double det = 0;
        for (size_t colIndex = 0; colIndex < 3; colIndex++)
            det += cofactor(colIndex, Dimension::X) * cols[colIndex][Dimension::X];
        return det;
    }
    Matrix3x3 inverse() const {
        Matrix3x3 res;
        double det = determinant();
        for (Dimension rowIndex : {Dimension::X, Dimension::Y, Dimension::Z})
            for (size_t colIndex = 0; colIndex < 3; colIndex++)
                res.cols[colIndex][rowIndex] = cofactor(colIndex, rowIndex) / det;
        return res.transpose();
    }

    friend Vector3D dot(Matrix3x3 m, Vector3D v) {
        return Vector3D(m.cols[0][Dimension::X] * v[Dimension::X] + m.cols[1][Dimension::X] * v[Dimension::Y] + m.cols[2][Dimension::X] * v[Dimension::Z],
                        m.cols[0][Dimension::Y] * v[Dimension::X] + m.cols[1][Dimension::Y] * v[Dimension::Y] + m.cols[2][Dimension::Y] * v[Dimension::Z],
                        m.cols[0][Dimension::Z] * v[Dimension::X] + m.cols[1][Dimension::Z] * v[Dimension::Y] + m.cols[2][Dimension::Z] * v[Dimension::Z]);
    }

    double get(Dimension i, size_t j) const {
        return cols[j][i];
    }

    void set(Dimension i, size_t j, double value) {
        cols[j][i] = value;
    }
private:
    double cofactor(size_t colIndex, Dimension rowIndex) const {
        double minor[2][2];

        int i = 0;
        for (Dimension dim : {Dimension::X, Dimension::Y, Dimension::Z})
            if (dim != rowIndex) {
                int j = 0;
                for (size_t index = 0; index < 3; index++)
                    if (index != colIndex) {
                        minor[i][j] = cols[index][dim];
                        j++;
                    }
                i++;
            }

        double res = minor[0][0] * minor[1][1] - minor[0][1] * minor[1][0];
        if (colIndex == 1)
            res *= -1;
        if (rowIndex == Dimension::Y)
            res *= -1;

        return res;
    }

    Vector3D cols[3];
};

#endif //RENDERER_MATRIX_H
