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


void generatePlaneXZ(float length, int divisions, const std::string& filename) {
    // std::string newFilename = filename.substr(0, filename.find_last_of('.')) + "Horizontal" + filename.substr(filename.find_last_of('.'));
    std::ofstream outFile("../Ficheiros3D/" + filename);
    if (!outFile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo " << filename << std::endl;
        return;
    }

    // Escreve o número de vértices no arquivo
    int numVertices = (divisions + 1) * (divisions + 1);

    // Calcula o espaçamento entre os vértices
    float spacing = length / divisions;

    outFile << numVertices << std::endl;

    // Escreve os vértices no arquivo
    for (int i = 0; i <= divisions; ++i) {
        for (int j = 0; j <= divisions; ++j) {
            float x = -length / 2.0f + j * spacing;
            float z = -length / 2.0f + i * spacing;
            outFile << x << ", 0, " << z << std::endl;
        }
    }

    outFile.close();
}

void generatePlaneXY(float length, int divisions, const std::string& filename) {
    std::ofstream outFile("../Ficheiros3D/" + filename);
    if (!outFile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo " << filename << std::endl;
        return;
    }

    // Escreve o número de vértices no arquivo
    int numVertices = (divisions + 1) * (divisions + 1);

    // Calcula o espaçamento entre os vértices
    float spacing = length / divisions;

    outFile << numVertices << std::endl;

    // Escreve os vértices no arquivo
    for (int i = 0; i <= divisions; ++i) {
        for (int j = 0; j <= divisions; ++j) {
            float x = -length / 2.0f + j * spacing;
            float y = -length / 2.0f + i * spacing;
            outFile << x << ", " << y << ", 0" << std::endl;
        }
    }

    outFile.close();
}


void generatePlaneYZ(float length, int divisions, const std::string& filename) {
    std::ofstream outFile("Fase1/Ficheiros3D/" + filename);
    if (!outFile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo " << filename << std::endl;
        return;
    }

    // Escreve o número de vértices no arquivo
    int numVertices = (divisions + 1) * (divisions + 1);

    // Calcula o espaçamento entre os vértices
    float spacing = length / divisions;

    outFile << numVertices << std::endl;

    // Escreve os vértices no arquivo
    for (int i = 0; i <= divisions; ++i) {
        for (int j = 0; j <= divisions; ++j) {
            float y = j * spacing;  // Use 'y' para representar a altura
            float z = -length / 2.0f + i * spacing;
            outFile << "0, " << y << ", " << z << std::endl; // Alterado para gerar o plano vertical
        }
    }

    outFile.close();
}


std::string converte(float x, float y, float z) {
    return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
}


/**
 * Gera pontos representando um cone e os salva em um arquivo.
 *
 * @param radius Raio da base do cone.
 * @param height Altura do cone.
 * @param slices Número de fatias ao redor do cone.
 * @param stacks Número de camadas do cone.
 * @param filename Nome do arquivo onde os pontos do cone serão salvos.
 */
    void drawCone(float radius, float height, float slices, float stacks, const std::string& filename) {
    // Abre o arquivo para escrita
    
    std::ofstream outFile("../Ficheiros3D/" + filename);
  
    if (!outFile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo " << filename << std::endl;
        return;
    }

    // Ângulo entre cada fatia do cone
    float angle = 2 * M_PI / slices;
    // Altura de cada fatia do cone
    float stackHeight = height / stacks;
    // Variáveis que armazenam os ângulos de cada ponto na base da fatia
    float alpha2, alpha1;
    // Altura do topo e da base da fatia atual
    float topHeight, bottomHeight;
    // Raio da fatia atual
    float radius2 = radius;
    // Raio da fatia anterior
    float prevRadius;

    // Escrita do número de vértices no arquivo
    // Calcula o número total de vértices com base no número de fatias e camadas
    int vertices = (slices * 3) + slices * (3 + 6 * (stacks - 1));
    outFile << vertices << std::endl;

    // Loop para gerar pontos do cone
    for (int j = 1; j <= stacks; j++) {
        topHeight = stackHeight * j;
        radius2 = radius2 - (radius / stacks);

        // Loop interno para gerar pontos de cada fatia
        for (int i = 1; i <= slices; i++) {
            alpha1 = angle * i;
            alpha2 = alpha1 + angle;

            if (j == 1) {
                // Base
                outFile << converte(0.0f, 0.0f, 0.0f) << std::endl;
                outFile << converte(radius * sin(alpha2), 0, radius * cos(alpha2)) << std::endl;
                outFile << converte(radius * sin(alpha1), 0, radius * cos(alpha1)) << std::endl;

                // Lados
                outFile << converte(radius * sin(alpha2), 0, radius * cos(alpha2)) << std::endl;
                outFile << converte(radius2 * sin(alpha2), topHeight, radius2 * cos(alpha2)) << std::endl;
                outFile << converte(radius * sin(alpha1), 0, radius * cos(alpha1)) << std::endl;

                outFile << converte(radius * sin(alpha1), 0, radius * cos(alpha1)) << std::endl;
                outFile << converte(radius2 * sin(alpha2), topHeight, radius2 * cos(alpha2)) << std::endl;
                outFile << converte(radius2 * sin(alpha1), topHeight, radius2 * cos(alpha1)) << std::endl;
            }
            else if (j != stacks) {
                // Lados
                outFile << converte(prevRadius * sin(alpha2), bottomHeight, prevRadius * cos(alpha2)) << std::endl;
                outFile << converte(radius2 * sin(alpha2), topHeight, radius2 * cos(alpha2)) << std::endl;
                outFile << converte(prevRadius * sin(alpha1), bottomHeight, prevRadius * cos(alpha1)) << std::endl;

                outFile << converte(prevRadius * sin(alpha1), bottomHeight, prevRadius * cos(alpha1)) << std::endl;
                outFile << converte(radius2 * sin(alpha2), topHeight, radius2 * cos(alpha2)) << std::endl;
                outFile << converte(radius2 * sin(alpha1), topHeight, radius2 * cos(alpha1)) << std::endl;
            }
            else {
                // Topo
                outFile << converte(prevRadius * sin(alpha2), bottomHeight, prevRadius * cos(alpha2)) << std::endl;
                outFile << converte(0, topHeight, 0) << std::endl;
                outFile << converte(prevRadius * sin(alpha1), bottomHeight, prevRadius * cos(alpha1)) << std::endl;
            }
        }

        bottomHeight = topHeight;
        prevRadius = radius2;
    }

    outFile.close();
}


    void generateSphere(float radius, int slices, int stacks, const std::string& filename) {
        std::vector<float> vertices;

        for (int i = 0; i <= stacks; ++i) {
            float phi = static_cast<float>(M_PI) * i / stacks;
            for (int j = 0; j < slices; ++j) {
                float theta = 2 * static_cast<float>(M_PI) * j / slices;
                float x = radius * std::sin(phi) * std::cos(theta);
                float y = radius * std::sin(phi) * std::sin(theta);
                float z = radius * std::cos(phi);
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
            }
        }

        std::ofstream outFile("../Ficheiros3D/" + filename);
        if (outFile.is_open()) {
            // Write the number of vertices as the first line
            outFile << vertices.size() / 3 << "\n";

            // Write vertex coordinates
            for (size_t i = 0; i < vertices.size(); i += 3) {
                outFile << vertices[i] << ", " << vertices[i + 1] << ", " << vertices[i + 2] << "\n";
            }
            outFile.close();
        }
        else {
            std::cerr << "Unable to open file: " << filename << std::endl;
        }
    }




    int main(int argc, char* argv[]) {
        if (argc < 4) {
            std::cerr << "Uso: " << argv[0] << " <figura> <parâmetros da figura> <arquivo_saida>" << std::endl;
            return 1;
        }

        std::string figura = argv[1];
        std::string filename = argv[argc - 1];

        try {
            if (figura == "plane") {
                if (argc != 5) {
                    throw std::invalid_argument("Número incorreto de argumentos para gerar um plano.");
                }
                float length = std::stof(argv[2]);
                int divisions = std::stoi(argv[3]);
                generatePlaneXZ(length, divisions, filename);
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
            else if (figura == "esfera") {
                double radius = std::stof(argv[2]);
                int slices = std::stoi(argv[3]);
                int stacks = std::stoi(argv[4]);

                std::string filename = argv[5];
                generateSphere(radius, slices, stacks, filename);
                std::cout << "Esfera gerada com sucesso e salvo em " << filename << std::endl;
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

