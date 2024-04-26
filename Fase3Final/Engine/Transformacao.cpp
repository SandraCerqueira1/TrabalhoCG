#include "Transformacao.h"

Transformacao::Transformacao() {
    this->type = 0;
    this->angle = 0;
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

Transformacao::Transformacao(float type, float a, float b, float c, float angle) {
    this->type = type;
    this->angle = angle;
    this->x = a;
    this->y = b;
    this->z = c;
}
