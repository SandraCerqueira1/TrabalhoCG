#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <list>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "tinyxml2.h"

using namespace tinyxml2;
using namespace std;

float camX, camY, camZ;  // posição x, y, z da câmera
float alpha, beta, r;      // ângulos e raio da câmera
int xInicial, yInicial, modoRato = 0;   // posições anteriores da câmera e modo da mesma
double lookX;
double lookY;
double lookZ;
double upX;
double upY;
double upZ;
float fov;
float near;
float far;

int windowWidth;
int windowHeight;

bool eixos = true;   // eixos
int tipo = GL_LINE;   // tipo de desenho: linhas, pontos ou fill
float v = 0.0f, g = 1.0f, b = 0.0f; // cores do desenho

// Classe ponto que contém as coordenadas x, y, z de cada ponto 
struct Point3D {
    float x, y, z;
    Point3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

std::vector<Point3D> globalPoints;

void changeSize(int w, int h) {
    if (h == 0)
        h = 1;
    float ratio = w * 1.0 / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);

    std::cout << "far value antes: " << far << std::endl;
    gluPerspective(fov, ratio, near, far);
    std::cout << "far value : " << far << std::endl;
    glMatrixMode(GL_MODELVIEW);
}

void readPointsFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        float x1, y1, z1, x2, y2, z2, x3, y3, z3;
        char delimiter;
        iss >> x1 >> delimiter >> y1 >> delimiter >> z1 >> delimiter
            >> x2 >> delimiter >> y2 >> delimiter >> z2 >> delimiter
            >> x3 >> delimiter >> y3 >> delimiter >> z3;
        globalPoints.emplace_back(x1, y1, z1);
        globalPoints.emplace_back(x2, y2, z2);
        globalPoints.emplace_back(x3, y3, z3);
    }
}


//void drawTriangles() {
//    glBegin(GL_TRIANGLES);
//    for (size_t i = 0; i < globalPoints.size(); i += 3) {
//        if (tipo == GL_FILL) {
//            // Cores para os vértices do triângulo (azul para roxo)
//            glColor3f(0.0f, 0.0f, 1.0f);              // Vértice 1 (azul)
//            glVertex3f(globalPoints[i].x, globalPoints[i].y, globalPoints[i].z);
//
//            glColor3f(0.6f, 0.0f, 0.6f);              // Vértice 2 (roxo)
//            glVertex3f(globalPoints[i + 1].x, globalPoints[i + 1].y, globalPoints[i + 1].z);
//
//            glColor3f(0.5f, 0.0f, 0.5f);              // Vértice 3 (roxo)
//            glVertex3f(globalPoints[i + 2].x, globalPoints[i + 2].y, globalPoints[i + 2].z);
//        }
//        else {
//            // Caso contrário (GL_LINE ou GL_POINT)
//            glColor3f(1.0f, 1.0f, 1.0f);             
//            glVertex3f(globalPoints[i].x, globalPoints[i].y, globalPoints[i].z);
//            glVertex3f(globalPoints[i + 1].x, globalPoints[i + 1].y, globalPoints[i + 1].z);
//            glVertex3f(globalPoints[i + 2].x, globalPoints[i + 2].y, globalPoints[i + 2].z);
//        }
//    }
//    glEnd();
//}

void drawTriangles() {
    // Configurar a espessura dos pontos
    if (tipo == GL_POINT) {
        glPointSize(5.0f); // Ajuste a espessura conforme necessário
    }

    glBegin(GL_TRIANGLES);
    for (size_t i = 0; i < globalPoints.size(); i += 3) {
        if (tipo == GL_FILL) {
            // Cores para os vértices do triângulo (azul para roxo)
            glColor3f(0.0f, 0.0f, 1.0f);              // Vértice 1 (azul)
            glVertex3f(globalPoints[i].x, globalPoints[i].y, globalPoints[i].z);

            glColor3f(0.5f, 0.0f, 0.5f);              // Vértice 2 (roxo intermediário)
            glVertex3f(globalPoints[i + 1].x, globalPoints[i + 1].y, globalPoints[i + 1].z);

            glColor3f(0.5f, 0.0f, 0.5f);              // Vértice 3 (roxo intermediário)
            glVertex3f(globalPoints[i + 2].x, globalPoints[i + 2].y, globalPoints[i + 2].z);
        }
        else {
            // Caso contrário (GL_LINE ou GL_POINT)
            glColor3f(1.0f, 1.0f, 1.0f);
            glVertex3f(globalPoints[i].x, globalPoints[i].y, globalPoints[i].z);
            glVertex3f(globalPoints[i + 1].x, globalPoints[i + 1].y, globalPoints[i + 1].z);
            glVertex3f(globalPoints[i + 2].x, globalPoints[i + 2].y, globalPoints[i + 2].z);
        }
    }

    glEnd();

    // Restaurar a espessura padrão
    if (tipo == GL_POINT) {
        glPointSize(1.0f); // Ajuste conforme necessário ou retorne à espessura padrão
    }
}


// Função que desenha os eixos
void eixo() {
    glBegin(GL_LINES);
    // X axis in red
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-100.0f, 0.0f, 0.0f);
    glVertex3f(100.0f, 0.0f, 0.0f);
    // Y Axis in Green
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -100.0f, 0.0f);
    glVertex3f(0.0f, 100.0f, 0.0f);
    // Z Axis in Blue
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -100.0f);
    glVertex3f(0.0f, 0.0f, 100.0f);
    glEnd();
}

void renderScene(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, tipo);
    glLoadIdentity();
    gluLookAt(camX, camY, camZ,
        lookX, lookY, lookZ,
        upX, upY, upZ);
    if (eixos) {
        eixo();
    }
    glColor3f(v, g, b);
    drawTriangles();
    glutSwapBuffers();
}

// Atualizar a posição da câmera com base nos valores de alpha, beta e r
void updateCameraPosition() {
    camX = r * cos(beta) * sin(alpha);
    camY = r * sin(beta);
    camZ = r * cos(beta) * cos(alpha);
}

// Função para calcular os valores iniciais de alpha, beta e r
void calculateInitialCameraValues() {
    alpha = atan2(camZ, camX);
    beta = atan2(camY, sqrt(camX * camX + camZ * camZ));
    r = sqrt(camX * camX + camY * camY + camZ * camZ);
}

// Função que lê o ficheiro XML da pasta ../xml/
void readXML(string file) {
    XMLDocument xmlDoc;
    if (xmlDoc.LoadFile(("../../FicheirosXML/" + file).c_str()) == XML_SUCCESS) {
        cout << "Ficheiro lido com sucesso" << endl;

        XMLElement* pWorld = xmlDoc.FirstChildElement("world");
        if (!pWorld) {
            std::cerr << "Erro: Nenhuma tag 'world' encontrada no arquivo XML." << std::endl;
            return;
        }

        // Extrair informações da janela
        XMLElement* pWindow = pWorld->FirstChildElement("window");
        if (pWindow) {
            windowWidth = atoi(pWindow->Attribute("width"));
            windowHeight = atoi(pWindow->Attribute("height"));
            cout << "Window width: " << windowWidth << ", height: " << windowHeight << endl;
        }


        // Extrair informações da câmera
        XMLElement* pCamera = pWorld->FirstChildElement("camera");
        if (pCamera) {
            XMLElement* pPosition = pCamera->FirstChildElement("position");
            XMLElement* pLookAt = pCamera->FirstChildElement("lookAt");
            XMLElement* pUp = pCamera->FirstChildElement("up");
            XMLElement* pProjection = pCamera->FirstChildElement("projection");

            camX = atof(pPosition->Attribute("x"));
            camY = atof(pPosition->Attribute("y"));
            camZ = atof(pPosition->Attribute("z"));

            calculateInitialCameraValues(); // Calcular os valores iniciais de alpha, beta e r

            

            lookX = atof(pLookAt->Attribute("x"));
            lookY = atof(pLookAt->Attribute("y"));
            lookZ = atof(pLookAt->Attribute("z"));

            upX = atof(pUp->Attribute("x"));
            upY = atof(pUp->Attribute("y"));
            upZ = atof(pUp->Attribute("z"));

            fov = atof(pProjection->Attribute("fov"));
            near = atof(pProjection->Attribute("near"));
            far = atof(pProjection->Attribute("far"));

            cout << "Camera position: (" << camX << ", " << camY << ", " << camZ << ")" << endl;
            cout << "Look at: (" << lookX << ", " << lookY << ", " << lookZ << ")" << endl;
            cout << "Up vector: (" << upX << ", " << upY << ", " << upZ << ")" << endl;
            cout << "Projection: fov = " << fov << ", near = " << near << ", far = " << far << endl;
        }

        // Extrair informações do grupo de modelos
        XMLElement* pGroup = pWorld->FirstChildElement("group");
        if (pGroup) {
            XMLElement* pModels = pGroup->FirstChildElement("models");
            if (pModels) {
                XMLElement* pModel = pModels->FirstChildElement("model");
                while (pModel) {
                    const char* modelFile = pModel->Attribute("file");
                    if (modelFile) {
                        // Construir o caminho completo para o arquivo do modelo dentro de "Ficheiros3d"
                        std::string modelPath = "../../Ficheiros3d/";
                        modelPath += modelFile;
                        readPointsFromFile(modelPath);
                    }
                    pModel = pModel->NextSiblingElement("model");
                }
            }
        }
    }
    else {
        cout << "Erro ao ler o xml" << endl;
    }
}

void processKeys(int key, int xx, int yy) {
    switch (key) {
    case GLUT_KEY_DOWN:
        beta -= 0.1f;
        break;
    case GLUT_KEY_UP:
        beta += 0.1f;
        break;
    case GLUT_KEY_RIGHT:
        alpha += 0.1f;
        break;
    case GLUT_KEY_LEFT:
        alpha -= 0.1f;
        break;
    }
    updateCameraPosition(); // Atualizar a posição da câmara
    glutPostRedisplay();
}

void processSpecialKeys(unsigned char key, int x, int y) {
    switch (key) {
    case '+':
        r -= 0.5f;
        break;
    case '-':
        r += 0.5f;
        break;

    case 'f':
        tipo = GL_FILL;
        break;
    case 'l':
        tipo = GL_LINE;
        break;
    case 'p':
        tipo = GL_POINT;
        break;
    }
    updateCameraPosition(); // Atualizar a posição da câmara
    glutPostRedisplay();
}





int main(int argc, char* argv[]) {
    if (argc == 2) {
        readXML(argv[1]);
    }
    else {
        readXML("test_1_5.xml");
    }
   

    // Inicialização
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(windowWidth, windowHeight); // Alterado para as dimensões desejadas
    glutCreateWindow("Phase 1");

    // Callback registration
    glutDisplayFunc(renderScene);
    //glutIdleFunc(renderScene);
    glutReshapeFunc(changeSize);

    glutSpecialFunc(processKeys);
    glutKeyboardFunc(processSpecialKeys);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glutMainLoop();

    return 1;
}
