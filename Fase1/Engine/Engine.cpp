#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
using namespace std;

float cameraRadius = 7.0f;
float cameraTheta = M_PI / 4;  // Ângulo inicial
float cameraPhi = M_PI / 4;    // Ângulo inicial
GLenum drawMode = GL_LINE;

struct Point3D {
    float x, y, z;
    Point3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

std::vector<Point3D> globalPoints;

void changeSize(int w, int h) {
    // Evitar uma divisão por zero, quando a janela é muito curta
    if (h == 0)
        h = 1;

    // Calcular a proporção da janela
    float ratio = w * 1.0 / h;

    // Definir a matriz de projeção como a atual
    glMatrixMode(GL_PROJECTION);
    // Carregar a matriz identidade
    glLoadIdentity();

    // Definir o viewport como a janela inteira
    glViewport(0, 0, w, h);

    // Definir a perspectiva
    gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

    // Voltar para o modo de matriz de visualização do modelo
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


void renderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLoadIdentity();

    float camX = cameraRadius * sin(cameraTheta) * sin(cameraPhi);
    float camY = cameraRadius * cos(cameraPhi);
    float camZ = cameraRadius * cos(cameraTheta) * sin(cameraPhi);

    gluLookAt(camX, camY, camZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // Linhas dos eixos
    glBegin(GL_LINES);
    // Eixo X em vermelho
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-100.0f, 0.0f, 0.0f);
    glVertex3f(100.0f, 0.0f, 0.0f);
    // Eixo Y em verde
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -100.0f, 0.0f);
    glVertex3f(0.0f, 100.0f, 0.0f);
    // Eixo Z em azul
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -100.0f);
    glVertex3f(0.0f, 0.0f, 100.0f);
    glEnd();

    drawTriangles();

    glutSwapBuffers();
}

void processSpecialKeys(int key, int xx, int yy) {
    // Manipular teclas especiais
    switch (key) {
    case GLUT_KEY_UP:
        cameraRadius -= 0.1f;
        break;
    case GLUT_KEY_DOWN:
        cameraRadius += 0.1f;
        break;
    }

    glutPostRedisplay();
}

void processKeys(unsigned char c, int xx, int yy) {
    // Manipular teclas regulares
    switch (c) {
    case 'w':
        cameraPhi -= 0.1f;
        break;
    case 's':
        cameraPhi += 0.1f;
        break;
    case 'a':
        cameraTheta -= 0.1f;
        break;
    case 'd':
        cameraTheta += 0.1f;
        break;
    }

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("CG@DI-UM");

    //readPointsFromFile("C:\\Users\\João Rodrigues\\Desktop\\Universidade\\3 Ano 2 Semestre\\CG\\TrabalhoCG-main\\Fase1\\Ficheiros3D\\plane.3d");
    //readPointsFromFile("C:\\Users\\João Rodrigues\\Desktop\\Universidade\\3 Ano 2 Semestre\\CG\\TrabalhoCG-main\\Fase1\\Ficheiros3D\\cone.3d");
    //readPointsFromFile("C:\\Users\\João Rodrigues\\Desktop\\Universidade\\3 Ano 2 Semestre\\CG\\TrabalhoCG-main\\Fase1\\Ficheiros3D\\sphere.3d");
    readPointsFromFile("C:\\Users\\João Rodrigues\\Desktop\\Universidade\\3 Ano 2 Semestre\\CG\\TrabalhoCG-main\\Fase1\\Ficheiros3D\\box.3d");


    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

    glutKeyboardFunc(processKeys);
    glutSpecialFunc(processSpecialKeys);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glutMainLoop();

    return 1;
}