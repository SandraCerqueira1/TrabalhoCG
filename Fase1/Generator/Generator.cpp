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

void writeVertex(std::ofstream& outFile, float x1, float y1, float z1, 
                                    float x2, float y2, float z2,
                                    float x3, float y3, float z3) {
    outFile << x1 << ", " << y1 << ", " << z1 << "; ";
    outFile << x2 << ", " << y2 << ", " << z2 << "; ";
    outFile << x3 << ", " << y3 << ", " << z3 << std::endl;
}


void generatePlaneXZ(float length, int divisions, const std::string& filename, float tx, float ty, float tz) {
    std::ofstream outFile("../Ficheiros3D/" + filename);
    if (!outFile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo " << filename << std::endl;
        return;
    }

    float spacing = length / divisions;

    for (int i = 0; i < divisions; ++i) {
        for (int j = 0; j < divisions; ++j) {
            float x0 = -length / 2.0f + j * spacing + tx;
            float z0 = -length / 2.0f + i * spacing + tz;
            float x1 = -length / 2.0f + (j + 1) * spacing + tx;
            float z1 = -length / 2.0f + (i + 1) * spacing + tz;
            float x2 = -length / 2.0f + (j + 1) * spacing + tx;
            float z2 = -length / 2.0f + i * spacing + tz;
            outFile << x0 << ", " << ty << ", " << z0 << "; " << x1 << ", " << ty << ", " << z1 << "; " << x2 << ", " << ty << ", " << z2 << std::endl;

            float x3 = -length / 2.0f + j * spacing + tx;
            float z3 = -length / 2.0f + i * spacing + tz;
            float x4 = -length / 2.0f + j * spacing + tx;
            float z4 = -length / 2.0f + (i + 1) * spacing + tz;
            float x5 = -length / 2.0f + (j + 1) * spacing + tx;
            float z5 = -length / 2.0f + (i + 1) * spacing + tz;
            outFile << x3 << ", " << ty << ", " << z3 << "; " << x4 << ", " << ty << ", " << z4 << "; " << x5 << ", " << ty << ", " << z5 << std::endl;
        }
    }

    outFile.close();
}

void generatePlaneXY(float length, int divisions, const std::string& filename, float tx, float ty, float tz) {
    std::ofstream outFile("../Ficheiros3D/" + filename);
    if (!outFile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo " << filename << std::endl;
        return;
    }

    int numVertices = (divisions + 1) * (divisions + 1);
    float spacing = length / divisions;

    outFile << numVertices << std::endl;

    for (int i = 0; i <= divisions; ++i) {
        for (int j = 0; j <= divisions; ++j) {
            float x = -length / 2.0f + j * spacing + tx;
            float y = -length / 2.0f + i * spacing + ty;
            outFile << x << ", " << y << ", " << tz << "; ";
        }
    }

    outFile.close();
}


void generatePlaneYZ(float length, int divisions, const std::string& filename, float tx, float ty, float tz) {
    std::ofstream outFile("../Ficheiros3D/" + filename);
    if (!outFile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo " << filename << std::endl;
        return;
    }

    int numVertices = (divisions + 1) * (divisions + 1);
    float spacing = length / divisions;

    outFile << numVertices * 2 << std::endl;

    for (int i = 0; i < divisions; ++i) {
        for (int j = 0; j < divisions; ++j) {
            float y0 = j * spacing + ty;
            float z0 = -length / 2.0f + i * spacing + tz;
            float y1 = (j + 1) * spacing + ty;
            float z1 = -length / 2.0f + (i + 1) * spacing + tz;
            outFile << tx << ", " << y0 << ", " << z0 << "; " << tx << ", " << y1 << ", " << z0 << "; " << tx << ", " << y1 << ", " << z1 << std::endl;
            outFile << tx << ", " << y0 << ", " << z0 << "; " << tx << ", " << y1 << ", " << z1 << "; " << tx << ", " << y0 << ", " << z1 << std::endl;
        }
    }

    outFile.close();
}

void generateBox(float length, int divisions, const std::string& filename) {
    float tx = 0.0f;
    float ty = 0.0f;
    float tz = 0.0f;

    generatePlaneXY(length, divisions, filename, tx, ty, tz + length / 2.0f); // Front face
    generatePlaneXY(length, divisions, filename, tx, ty, tz - length / 2.0f); // Back face
    generatePlaneXZ(length, divisions, filename, tx, ty + length / 2.0f, tz); // Top face
    generatePlaneXZ(length, divisions, filename, tx, ty - length / 2.0f, tz); // Bottom face
    generatePlaneYZ(length, divisions, filename, tx + length / 2.0f, ty, tz); // Right face
    generatePlaneYZ(length, divisions, filename, tx - length / 2.0f, ty, tz); // Left face
}


void drawCone(float radius, float height, float slices, float stacks, const std::string& filename) {
    std::ofstream outFile("../Ficheiros3D/" + filename);
    if (!outFile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo " << filename << std::endl;
        return;
    }

    float angle = 2 * M_PI / slices;
    float stackHeight = height / stacks;
    float alpha2, alpha1;
    float topHeight, bottomHeight;
    float radius2 = radius;
    float prevRadius;

    int vertices = (slices * 3) + slices * (3 + 6 * (stacks - 1));
    outFile << vertices << std::endl;

    for (int j = 1; j <= stacks; j++) {
        topHeight = stackHeight * j;
        radius2 = radius2 - (radius / stacks);

        for (int i = 1; i <= slices; i++) {
            alpha1 = angle * i;
            alpha2 = alpha1 + angle;

            if (j == 1) {
                writeVertex(outFile, 0.0f, 0.0f, 0.0f, 
                                    radius * std::sin(alpha2), 0, radius * std::cos(alpha2), 
                                    radius * std::sin(alpha1), 0, radius * std::cos(alpha1));

                writeVertex(outFile, radius * std::sin(alpha2), 0, radius * std::cos(alpha2), 
                                        radius2 * std::sin(alpha2), topHeight, radius2 * std::cos(alpha2), 
                                        radius * std::sin(alpha1), 0, radius * std::cos(alpha1));

                writeVertex(outFile, radius * std::sin(alpha1), 0, radius * std::cos(alpha1), 
                                        radius2 * std::sin(alpha2), topHeight, radius2 * std::cos(alpha2), 
                                        radius2 * std::sin(alpha1), topHeight, radius2 * std::cos(alpha1));
            }
            else if (j != stacks) {
                writeVertex(outFile, prevRadius * std::sin(alpha2), bottomHeight, prevRadius * std::cos(alpha2), 
                                        radius2 * std::sin(alpha2), topHeight, radius2 * std::cos(alpha2), 
                                        prevRadius * std::sin(alpha1), bottomHeight, prevRadius * std::cos(alpha1));

                writeVertex(outFile, prevRadius * std::sin(alpha1), bottomHeight, prevRadius * std::cos(alpha1), 
                                        radius2 * std::sin(alpha2), topHeight, radius2 * std::cos(alpha2), 
                                        radius2 * std::sin(alpha1), topHeight, radius2 * std::cos(alpha1));
            }
            else {
                writeVertex(outFile, prevRadius * std::sin(alpha2), bottomHeight, prevRadius * std::cos(alpha2), 
                                        0, topHeight, 0, 
                                        prevRadius * std::sin(alpha1), bottomHeight, prevRadius * std::cos(alpha1));
            }
        }

        bottomHeight = topHeight;
        prevRadius = radius2;
    }

    outFile.close();
}



void generateSphere(float radius, int slices, int stacks, const std::string& filename) {
    std::vector<float> vertices;

    for (int i = 0; i < stacks; ++i) {
        float phi1 = static_cast<float>(M_PI) * i / stacks;
        float phi2 = static_cast<float>(M_PI) * (i + 1) / stacks;
        
        for (int j = 0; j <= slices; ++j) {
            float theta1 = 2 * static_cast<float>(M_PI) * j / slices;
            float theta2 = 2 * static_cast<float>(M_PI) * (j + 1) / slices;
            
            float x1 = radius * std::sin(phi1) * std::cos(theta1);
            float y1 = radius * std::sin(phi1) * std::sin(theta1);
            float z1 = radius * std::cos(phi1);
            
            float x2 = radius * std::sin(phi1) * std::cos(theta2);
            float y2 = radius * std::sin(phi1) * std::sin(theta2);
            float z2 = radius * std::cos(phi1);
            
            float x3 = radius * std::sin(phi2) * std::cos(theta2);
            float y3 = radius * std::sin(phi2) * std::sin(theta2);
            float z3 = radius * std::cos(phi2);
            
            vertices.push_back(x1);
            vertices.push_back(y1);
            vertices.push_back(z1);
            
            vertices.push_back(x2);
            vertices.push_back(y2);
            vertices.push_back(z2);
            
            vertices.push_back(x3);
            vertices.push_back(y3);
            vertices.push_back(z3);
        }
    }

    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        // Write the number of vertices as the first line
        outFile << vertices.size() / 3 << "\n";

        // Write vertex coordinates
        for (size_t i = 0; i < vertices.size(); i += 9) {
            for (size_t j = 0; j < 9; j += 3) {
                outFile << vertices[i + j] << ", " << vertices[i + j + 1] << ", " << vertices[i + j + 2];
                if (j < 6) outFile << "; ";
            }
            outFile << "\n";
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
            generatePlaneXZ(length, divisions, filename, 0, 0, 0);
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
        else if (figura == "box") { // Adicionado caso para gerar o cubo
            if (argc != 5) {
                throw std::invalid_argument("Número incorreto de argumentos para gerar um cubo.");
            }
            float length = std::stof(argv[2]);
            int divisions = std::stoi(argv[3]);
            generateBox(length, divisions, filename);
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

