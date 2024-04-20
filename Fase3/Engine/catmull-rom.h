#ifndef CATMULL_ROM_H
#define CATMULL_ROM_H

#include <vector>
#include "Modelo.h"

void buildRotMatrix(float *x, float *y, float *z, float *m);

void cross(float *a, float *b, float *res);

void normalize(float *a);

float length(float *v);

void multMatrixVector(float *m, float *v, float *res);

void getCatmullRomPoint(float t, Point3D p0, Point3D p1, Point3D p2, Point3D p3, float* pos, float* deriv);

void getGlobalCatmullRomPoint(float gt, float* pos, float* deriv, std::vector<Point3D> pontos);

#endif // CATMULL_ROM_H
