#ifndef MODELO_H
#define MODELO_H

#include <vector>
#include <GL/glut.h>

struct Point3D {
    float x, y, z;
    Point3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

class Modelo {
public:
    std::vector<Point3D> pontos;
    float r, g, b;
    GLuint buffer;
};

#endif /* MODELO_H */
