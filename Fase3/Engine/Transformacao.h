#ifndef TRANSFORMACAO_H
#define TRANSFORMACAO_H

#include <vector>
#include "Modelo.h"

class Transformacao {
public:
    int type;
    float angle;
    float x;
    float y;
    float z;
    float time = 0;
	bool align = true;
    std::vector<Point3D> catmullRomPoints;


    Transformacao(); // Default constructor
    Transformacao(float type, float a, float b, float c, float angle = 0); // Parameterized constructor
};

#endif /* TRANSFORMACAO_H */
