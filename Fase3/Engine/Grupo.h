#ifndef GRUPO_H
#define GRUPO_H

#include <vector>
#include "Transformacao.h"
#include "Modelo.h"

class Grupo {
public:
    int id;
    std::vector<Grupo> filhos;
    std::vector<Modelo> modelos;
    std::vector<Transformacao> transforms;

    void addFilho(Grupo g);
};

#endif /* GRUPO_H */
