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
int alpha = 0, beta = 0, r = 20;      // ângulos e raio da câmera
int xInicial, yInicial, modoRato = 0;   // posições anteriores da câmera e modo da mesma
double lookX;
double lookY;
double lookZ;
double upX;
double upY;
double upZ;
int fov;
int near;
double far;

int windowWidth ;
int windowHeight ;

bool eixos = true;   // eixos
int tipo = GL_LINE;   // tipo de desenho: linhas, pontos ou fill
float v = 0.0f, g = 1.0f, b = 0.0f; // cores do desenho

// Classe ponto que contém as coordenadas x, y, z de cada ponto junto com os getters e setters
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
    gluPerspective(40.0f, ratio, 1.0f, 1000.0f);
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

void drawTriangles() {
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 1.0f, 1.0f);
    for (size_t i = 0; i < globalPoints.size(); i += 3) {
        glVertex3f(globalPoints[i].x, globalPoints[i].y, globalPoints[i].z);
        glVertex3f(globalPoints[i + 1].x, globalPoints[i + 1].y, globalPoints[i + 1].z);
        glVertex3f(globalPoints[i + 2].x, globalPoints[i + 2].y, globalPoints[i + 2].z);
    }
    glEnd();
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




// Função que processa as teclas do rato e muda as coordenadas, angulos ou raio da camara com base nisso
void processMouseButtons(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        xInicial = x;
        yInicial = y;
        if (button == GLUT_LEFT_BUTTON) {
            modoRato = 1; // Modo rodar camara
        }
        else if (button == GLUT_RIGHT_BUTTON) {
            modoRato = 2; // Modo aproximar ou afastar camara
        }
        else {
            modoRato = 0;
        }
    }
    else if (state == GLUT_UP) {
        if (modoRato == 1) { // Muda alpha e beta pois este modo muda a posicao
            alpha += (x - xInicial);
            beta += (y - yInicial);
        }
        else if (modoRato == 2) { // Muda raio pois este modo muda o zoom
            r -= y - yInicial;
            if (r < 3) r = 3.0;
        }
        modoRato = 0;
    }
}

// Função que faz a movimentacao das variaveis da camara
void processMouseMotion(int x, int y) {
    int xAux, yAux;
    int alphaAux, betaAux;
    int rAux;

    if (!modoRato) return;

    xAux = x - xInicial;
    yAux = y - yInicial;

    if (modoRato == 1) {
        alphaAux = alpha + xAux;
        betaAux = beta + yAux;
        if (betaAux > 85.0) betaAux = 85.0;
        else if (betaAux < -85.0) betaAux = -85.0;
        rAux = r;
    }
    else if (modoRato == 2) {
        alphaAux = alpha;
        betaAux = beta;
        rAux = r - yAux;
        if (rAux < 3) rAux = 3;
    }

    // Nova posicao x, y, z da camara
    camX = rAux * sin(-alphaAux * M_PI / 180.0) * cos(betaAux * M_PI / 180.0);
    camZ = rAux * cos(-alphaAux * M_PI / 180.0) * cos(betaAux * M_PI / 180.0);
    camY = rAux * sin(betaAux * M_PI / 180.0);
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

            xInicial = camX;
            yInicial = camY;

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

void keyboard(unsigned char key, int x, int y) {
    if (key == 'e') {
        eixos = !eixos;
    }
    else if (key == 'f') {
        tipo = GL_FILL;
    }
    else if (key == 'l') {
        tipo = GL_LINE;
    }
    else if (key == 'p') {
        tipo = GL_POINT;
    }
    else if (key == 'r') {
        v = 1.0f;
        g = 0.0f;
        b = 0.0f;
    }
    else if (key == 'g') {
        v = 0.0f;
        g = 1.0f;
        b = 0.0f;
    }
    else if (key == 'b') {
        v = 0.0f;
        g = 0.0f;
        b = 1.0f;
    }
    else if (key == 'w') {
        v = 1.0f;
        g = 1.0f;
        b = 1.0f;
    }
    glutPostRedisplay();
}

int main(int argc, char* argv[]) {
    if (argc == 2) {
        readXML(argv[1]);
    }
    else {
        readXML("test_1_2.xml");
    }

    // Inicialização
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(windowWidth, windowHeight); // Alterado para as dimensões desejadas
    glutCreateWindow("Phase 1");

    // Callback registration
    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutReshapeFunc(changeSize);

    // Mouse callbacks
    //glutMouseFunc(processMouseButtons);
    //glutMotionFunc(processMouseMotion);
    //glutKeyboardFunc(keyboard);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glutMainLoop();

    return 1;
}