#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h> 
#include <stdio.h>
using namespace std;
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <vector>

/**
 * @brief Escreve as coordenadas de três vértices em uma linha no arquivo de saída.
 *
 * @param outFile Objeto std::ofstream representando o arquivo de saída.
 * @param x1, y1, z1 Coordenadas do primeiro vértice.
 * @param x2, y2, z2 Coordenadas do segundo vértice.
 * @param x3, y3, z3 Coordenadas do terceiro vértice.
 */
void writeVertex(std::ofstream& outFile, float x1, float y1, float z1, 
                                    float x2, float y2, float z2,
                                    float x3, float y3, float z3) {
    // Escreve as coordenadas dos vértices separadas por vírgulas e termina com ponto e vírgula
    outFile << x1 << ", " << y1 << ", " << z1 << "; ";
    outFile << x2 << ", " << y2 << ", " << z2 << "; ";
    outFile << x3 << ", " << y3 << ", " << z3 << std::endl;
}


/**
 * @brief Gera um plano no plano XZ e grava os vértices num ficheiro .3d
 *
 * @param length Comprimento do lado do plano
 * @param divisions Número de divisões ao longo de cada lado do plano
 * @param outFile  arquivo de saída
 * @param tx, ty, tz Translação do plano nos eixos X, Y e Z, respetivamente
 * @param side Indica qual é o lado do plano que será gerado (1 para superior, 0 para inferior)
 */
void generatePlaneXZ(float length, int divisions, std::ofstream& outFile, float tx, float ty, float tz, int side) {
    // Calcula o espaço entre os vértices com base no número de divisões
    float spacing = length / divisions;

    if (side == 1) {
        for (int i = 0; i < divisions; ++i) {
            for (int j = 0; j < divisions; ++j) {
                // Vértices do primeiro triângulo
                float x0 = -length / 2.0f + j * spacing + tx;
                float y0 = 0 + ty;
                float z0 = -length / 2.0f + i * spacing + tz;
                float x1 = -length / 2.0f + (j + 1) * spacing + tx;
                float y1 = 0 + ty;
                float z1 = -length / 2.0f + (i + 1) * spacing + tz;
                float x2 = -length / 2.0f + (j + 1) * spacing + tx;
                float y2 = 0 + ty;
                float z2 = -length / 2.0f + i * spacing + tz;
                
                writeVertex(outFile, x0, y0, z0, x1, y1, z1, x2, y2, z2);

                // Vértices do segundo triângulo
                float x3 = -length / 2.0f + j * spacing + tx;
                float y3 = 0 + ty;
                float z3 = -length / 2.0f + i * spacing + tz;
                float x4 = -length / 2.0f + j * spacing + tx;
                float y4 = 0 + ty;
                float z4 = -length / 2.0f + (i + 1) * spacing + tz;
                float x5 = -length / 2.0f + (j + 1) * spacing + tx;
                float y5 = 0 + ty;
                float z5 = -length / 2.0f + (i + 1) * spacing + tz;
                
                writeVertex(outFile, x3, y3, z3, x4, y4, z4, x5, y5, z5);
            }
        }
    }
    else {
        for (int i = 0; i < divisions; ++i) {
            for (int j = 0; j < divisions; ++j) {
                // Vértices do primeiro triângulo
                float z0 = -length / 2.0f + j * spacing + tz;
                float y0 = 0 + ty;
                float x0 = -length / 2.0f + i * spacing + tx;
                float z1 = -length / 2.0f + (j + 1) * spacing + tz;
                float y1 = 0 + ty;
                float x1 = -length / 2.0f + (i + 1) * spacing + tx;
                float z2 = -length / 2.0f + (j + 1) * spacing + tz;
                float y2 = 0 + ty;
                float x2 = -length / 2.0f + i * spacing + tx;
                
                writeVertex(outFile, x0, y0, z0, x1, y1, z1, x2, y2, z2);

                // Vértices do segundo triângulo
                float z3 = -length / 2.0f + j * spacing + tz;
                float y3 = 0 + ty;
                float x3 = -length / 2.0f + i * spacing + tx;
                float z4 = -length / 2.0f + j * spacing + tz;
                float y4 = 0 + ty;
                float x4 = -length / 2.0f + (i + 1) * spacing + tx;
                float z5 = -length / 2.0f + (j + 1) * spacing + tz;
                float y5 = 0 + ty;
                float x5 = -length / 2.0f + (i + 1) * spacing + tx;

                writeVertex(outFile, x3, y3, z3, x4, y4, z4, x5, y5, z5);                
            }
        }
    }
}


/**
 * @brief Gera um plano no plano XY e grava os vértices num ficheiro .3d
 *
 * @param length Comprimento do lado do plano
 * @param divisions Número de divisões ao longo de cada lado do plano
 * @param outFile arquivo de saída.
 * @param tx, ty, tz Translação do plano nos eixos X, Y e Z, respetivamente.
 * @param side Indica qual é o lado do plano que será gerado (1 para superior, 0 para inferior)
 */
void generatePlaneXY(float length, int divisions, std::ofstream& outFile, float tx, float ty, float tz, int side) {
    // Calcula o número total de triângulos do plano
    int numTriangles = 2 * divisions * divisions;
    // Calcula o espaço entre os vértices com base no número de divisões
    float spacing = length / divisions;
    // Verifica  qual é o lado do plano que será gerado
    if (side == 0) {
        for (int i = 0; i < divisions; ++i) {
            for (int j = 0; j < divisions; ++j) {
                // Vértices do primeiro triângulo
                float x0 = -length / 2.0f + j * spacing + tx;
                float y0 = -length / 2.0f + i * spacing + ty;
                float z0 = 0 + tz;
                float x1 = -length / 2.0f + (j + 1) * spacing + tx;
                float y1 = -length / 2.0f + i * spacing + ty;
                float z1 = 0 + tz;
                float x2 = -length / 2.0f + (j + 1) * spacing + tx;
                float y2 = -length / 2.0f + (i + 1) * spacing + ty;
                float z2 = 0 + tz;
                
                writeVertex(outFile, x0, y0, z0, x2, y2, z2, x1, y1, z1);

                // Vértices do segundo triângulo
                float x3 = -length / 2.0f + j * spacing + tx;
                float y3 = -length / 2.0f + i * spacing + ty;
                float z3 = 0 + tz;
                float x4 = -length / 2.0f + (j + 1) * spacing + tx;
                float y4 = -length / 2.0f + (i + 1) * spacing + ty;
                float z4 = 0 + tz;
                float x5 = -length / 2.0f + j * spacing + tx;
                float y5 = -length / 2.0f + (i + 1) * spacing + ty;
                float z5 = 0 + tz;
                
                writeVertex(outFile, x3, y3, z3, x5, y5, z5, x4, y4, z4);            
            }
        }
    }
    else {
        for (int i = 0; i < divisions; ++i) {
            for (int j = 0; j < divisions; ++j) {
                // Vértices do primeiro triângulo
                float y0 = -length / 2.0f + j * spacing + ty;
                float x0 = -length / 2.0f + i * spacing + tx;
                float z0 = 0 + tz;
                float y1 = -length / 2.0f + (j + 1) * spacing + ty;
                float x1 = -length / 2.0f + i * spacing + tx;
                float z1 = 0 + tz;
                float y2 = -length / 2.0f + (j + 1) * spacing + ty;
                float x2 = -length / 2.0f + (i + 1) * spacing + tx;
                float z2 = 0 + tz;

                writeVertex(outFile, x0, y0, z0, x2, y2, z2, x1, y1, z1);

                // Vértices do segundo triângulo
                float y3 = -length / 2.0f + j * spacing + ty;
                float x3 = -length / 2.0f + i * spacing + tx;
                float z3 = 0 + tz;
                float y4 = -length / 2.0f + (j + 1) * spacing + ty;
                float x4 = -length / 2.0f + (i + 1) * spacing + tx;
                float z4 = 0 + tz;
                float y5 = -length / 2.0f + j * spacing + ty;
                float x5 = -length / 2.0f + (i + 1) * spacing + tx;
                float z5 = 0 + tz;

                writeVertex(outFile, x3, y3, z3, x5, y5, z5, x4, y4, z4);            
            }
        }
    }
}


/**
 * @brief Gera um plano no plano YZ e grava os vértices num ficheiro .3d
 *
 * @param length Comprimento do lado do plano
 * @param divisions Número de divisões ao longo de cada lado do plano
 * @param outFile  arquivo de saída
 * @param tx, ty, tz Translação do plano nos eixos X, Y e Z, respetivamente.
 * @param side Indica qual é o lado do plano que será gerado (1 para superior, 0 para inferior)
 */
void generatePlaneYZ(float length, int divisions, std::ofstream& outFile, float tx, float ty, float tz, int side) {
    // Calcula o número total de vértices no plano
    int numVertices = (divisions + 1) * (divisions + 1);
    // Calcula o espaço entre os vértices com base no número de divisões
    float spacing = length / divisions;
    // Verifica  qual é o lado do plano que será gerado
    if (side == 1){
        for (int i = 0; i < divisions; ++i) {
            for (int j = 0; j < divisions; ++j) {
                // Vértices do primeiro triângulo
                float x0 = 0 + tx;
                float y0 = j * spacing + ty;
                float z0 = -length / 2.0f + i * spacing + tz;
                float x1 = 0 + tx;
                float y1 = (j + 1) * spacing + ty;
                float z1 = -length / 2.0f + (i + 1) * spacing + tz;
                // Grava os vértices no arquivo
                writeVertex(outFile, x0, y0, z0, x0, y1, z0, x1, y1, z1);
                writeVertex(outFile, x0, y0, z0, x0, y1, z1, x1, y0, z1);
            }
        }
    }
    else {
        for (int i = 0; i < divisions; ++i) {
            for (int j = 0; j < divisions; ++j) {
                // Vértices do primeiro triângulo
                float x0 = 0 + tx;
                float y0 = j * spacing + ty;
                float z0 = -length / 2.0f + i * spacing + tz;
                float x1 = 0 + tx;
                float y1 = (j + 1) * spacing + ty;
                float z1 = -length / 2.0f + (i + 1) * spacing + tz;
                // Grava os vértices no arquivo
                writeVertex(outFile, x0, y0, z0, x1, y1, z1, x0, y1, z0);
                writeVertex(outFile, x0, y0, z0, x1, y0, z1, x1, y1, z1);            
            }
        }
    }
}

/**
 * @brief Gera um cubo e grava os vértices num ficheiro .3d
 *
 * @param length Comprimento do lado do cubo
 * @param divisions Número de divisões ao longo de cada face do cubo
 * @param outFile  arquivo de saída.
 */
void generateBox(float length, int divisions, std::ofstream& outFile) {
    float tx = 0.0f;
    float ty = 0.0f;
    float tz = 0.0f;

    // Gera as faces do cubo utilizando a função generatePlane de cada plano para cada face
    generatePlaneXY(length, divisions, outFile, tx, ty, tz - length / 2.0f, 0); // Back face
    generatePlaneXY(length, divisions, outFile, tx, ty, tz + length / 2.0f, 1); // Front face
    generatePlaneXZ(length, divisions, outFile, tx, ty - length / 2.0f, tz, 0); // Bottom face
    generatePlaneXZ(length, divisions, outFile, tx, ty + length / 2.0f, tz, 1); // Top face
    generatePlaneYZ(length, divisions, outFile, tx + length / 2.0f, ty - length / 2.0f, tz, 1); // Right face
    generatePlaneYZ(length, divisions, outFile, tx - length / 2.0f, ty - length / 2.0f, tz, 0); // Left face
}


/**
 * @brief Desenha um cone e grava os vértices num ficheiro .3d
 *
 * @param radius Raio da base do cone
 * @param height Altura do cone
 * @param slices Número de fatias do cone
 * @param stacks Número de camadas  do cone
 * @param filename Nome do arquivo onde os vértices serão guardados
 */
void drawCone(float radius, float height, float slices, float stacks, const std::string& filename) {
    
    std::ofstream outFile("../Ficheiros3D/" + filename);
    if (!outFile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo " << filename << std::endl;
        return;
    }

    // Calcula o ângulo entre as fatias
    float angle = 2 * M_PI / slices;
    // Calcula a altura de cada camada
    float stackHeight = height / stacks;
    // Variáveis para armazenar os ângulos, alturas e raios em iterações anteriores
    float alpha2, alpha1;
    float topHeight, bottomHeight;
    float radius2 = radius;
    float prevRadius;

    // Loop para percorrer as camadas do cone
    for (int j = 1; j <= stacks; j++) {
        // Calcula a altura do topo da camada atual
        topHeight = stackHeight * j;
        // Atualiza o raio para a camada atual
        radius2 = radius2 - (radius / stacks);

        // Loop para percorrer as fatias do cone
        for (int i = 1; i <= slices; i++) {
            alpha1 = angle * i;
            alpha2 = alpha1 + angle;

            // Verifica se é a primeira camada e desenha o primeiro quadrilátero
            if (j == 1) {
                //base
                writeVertex(outFile, 0.0f, 0.0f, 0.0f, 
                                    radius * std::sin(alpha2), 0, radius * std::cos(alpha2), 
                                    radius * std::sin(alpha1), 0, radius * std::cos(alpha1));

                //triangulo 1
                writeVertex(outFile, radius * std::sin(alpha2), 0, radius * std::cos(alpha2),
                                        radius2 * std::sin(alpha2), topHeight, radius2 * std::cos(alpha2), 
                                        radius2 * std::sin(alpha1), topHeight, radius2 * std::cos(alpha1));
                                        
            //triangulo 2
                writeVertex(outFile,  radius2 * std::sin(alpha1), topHeight, radius2 * std::cos(alpha1),
                                        radius * std::sin(alpha1), 0, radius * std::cos(alpha1),
                                        radius * std::sin(alpha2), 0, radius * std::cos(alpha2));
            }
            // Verifica se não é a última camada e escreve os vértices entre as camadas
            else if (j != stacks) {
                writeVertex(outFile, prevRadius * std::sin(alpha2), bottomHeight, prevRadius * std::cos(alpha2), 
                                        radius2 * std::sin(alpha2), topHeight, radius2 * std::cos(alpha2), 
                                        radius2 * std::sin(alpha1), topHeight, radius2 * std::cos(alpha1));
                                        

                writeVertex(outFile, radius2 * std::sin(alpha1), topHeight, radius2 * std::cos(alpha1), 
                                        prevRadius * std::sin(alpha1), bottomHeight, prevRadius * std::cos(alpha1), 
                                        prevRadius * std::sin(alpha2), bottomHeight, prevRadius * std::cos(alpha2));
            }
            // Se for a última camada, escreve os vértices do topo do cone
            else {
                writeVertex(outFile, prevRadius * std::sin(alpha2), bottomHeight, prevRadius * std::cos(alpha2), 
                                        0, topHeight, 0, 
                                        prevRadius * std::sin(alpha1), bottomHeight, prevRadius * std::cos(alpha1));
            }
        }

        // Atualiza a altura da camada inferior e o raio anterior
        bottomHeight = topHeight;
        prevRadius = radius2;
    }

    outFile.close();
}


/**
 * @brief Gera uma esfera e grava os vértices num ficheiros .3d
 *
 * @param radius Raio da esfera.
 * @param slices Número de fatias  da esfera.
 * @param stacks Número de camadas  da esfera.
 * @param filename Nome do arquivo onde os vértices serão guardados
 */
void generateSphere(float radius, int slices, int stacks, const std::string& filename) {
    std::ofstream outFile("../Ficheiros3D/" + filename);
    if (!outFile.is_open()) {
        std::cerr << "Error opening output file." << std::endl;
        return;
    }

    float deltaPhi = M_PI / stacks; //variação de phi em relação às stacks
    float deltaTheta = 2 * M_PI / slices; // variação de theta em relação às slices

    for (int i = 0; i < stacks; ++i) { // ciclo que itera sobre as diferentes stacks
        
        float phi1 = i * deltaPhi; //alteração de phi em relação à stack em que se encontra
        float phi2 = (i + 1) * deltaPhi; //alteração de phi em relação à stack em que se encontra, para os pontos superiores

        for (int j = 0; j < slices; ++j) { // ciclo que itera sobre as diferentes slices

            float deltaThetaAdjusted = deltaTheta / 2.0f; //permite ajustar a posição da esfera
            float theta1 = (j * deltaTheta) - deltaThetaAdjusted; //alteração de theta em relação à slice em que se encontra
            float theta2 = ((j + 1) * deltaTheta) - deltaThetaAdjusted; //alteração de theta em relação à slice em que se encontra , para os pontos mais à esquerda

            // Vértices dos triângulos
            float x1 = radius * sin(phi1) * cos(theta1);
            float y1 = -radius * cos(phi1);
            float z1 = radius * sin(phi1) * sin(theta1);

            float x2 = radius * sin(phi1) * cos(theta2);
            float y2 = -radius * cos(phi1);
            float z2 = radius * sin(phi1) * sin(theta2);

            float x3 = radius * sin(phi2) * cos(theta1);
            float y3 = -radius * cos(phi2);
            float z3 = radius * sin(phi2) * sin(theta1);

            float x4 = radius * sin(phi2) * cos(theta2);
            float y4 = -radius * cos(phi2);
            float z4 = radius * sin(phi2) * sin(theta2);

            // Escreve os triângulos formados pelos vértices
            writeVertex(outFile, x1, y1, z1, x3, y3, z3, x4, y4, z4);
            writeVertex(outFile, x1, y1, z1, x4, y4, z4, x2, y2, z2);
        }
    }

    outFile.close();
}




int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Uso: " << argv[0] << " <figura> <parâmetros da figura> <arquivo_saida>" << std::endl;
        return 1;
    }

    std::string figura = argv[1];
    std::string filename = argv[argc - 1];
    std::ofstream outFile("../Ficheiros3D/" + filename);

    try {
        if (figura == "plane") {
            if (argc != 5) {
                throw std::invalid_argument("Número incorreto de argumentos para gerar um plano.");
            }
            float length = std::stof(argv[2]);
            int divisions = std::stoi(argv[3]);
            generatePlaneXZ(length, divisions, outFile, 0, 0, 0, 1);
            std::cout << "Plano gerado com sucesso e salvo em " << filename << std::endl;
        }
        else if (figura == "planeXY") {
            if (argc != 5) {
                throw std::invalid_argument("Número incorreto de argumentos para gerar um plano.");
            }
            float length = std::stof(argv[2]);
            int divisions = std::stoi(argv[3]);
            generatePlaneXY(length, divisions, outFile, 0, 0 , 0, 0);
            std::cout << "Plano gerado com sucesso e salvo em " << filename << std::endl;
        }
        else if (figura == "planeYZ") {
            if (argc != 5) {
                throw std::invalid_argument("Número incorreto de argumentos para gerar um plano.");
            }
            float length = std::stof(argv[2]);
            int divisions = std::stoi(argv[3]);
            generatePlaneYZ(length, divisions, outFile, 0, 0, 0, 0);
            std::cout << "Plano gerado com sucesso e salvo em " << filename << std::endl;
        }
        else if (figura == "cone") {
            if (argc != 7) {
                throw std::invalid_argument("Número incorreto de argumentos para gerar um cone.");
            }
            float radius = std::stof(argv[2]);
            float height = std::stof(argv[3]);
            float slices = std::stof(argv[4]);
            float stacks = std::stof(argv[5]);
            drawCone(radius, height, slices, stacks, filename);
            std::cout << "Cone gerado com sucesso e salvo em " << filename << std::endl;
        }
        else if (figura == "sphere") {
            double radius = std::stof(argv[2]);
            int slices = std::stoi(argv[3]);
            int stacks = std::stoi(argv[4]);

            std::string filename = argv[5];
            generateSphere(radius, slices, stacks, filename);
            std::cout << "Esfera gerada com sucesso e salvo em " << filename << std::endl;
        }
        else if (figura == "box") {
            if (argc != 5) {
                throw std::invalid_argument("Número incorreto de argumentos para gerar um cubo.");
            }
            float length = std::stof(argv[2]);
            int divisions = std::stoi(argv[3]);
            generateBox(length, divisions, outFile);
            std::cout << "Cubo gerado com sucesso e salvo em " << filename << std::endl;
        }
        else {
            throw std::invalid_argument("Figura não reconhecida.");
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
