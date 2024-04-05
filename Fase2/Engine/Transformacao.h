#ifndef TRANSFORMACAO_H
#define TRANSFORMACAO_H

#include <vector>

class Transformacao {
public:
    int type;
    float angle;
    float x;
    float y;
    float z;


    Transformacao(); // Default constructor
    Transformacao(float type, float a, float b, float c, float angle = 0); // Parameterized constructor
};

#endif /* TRANSFORMACAO_H */
