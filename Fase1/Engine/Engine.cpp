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

struct Vertex {
    float x, y, z;
};

class Ponto {
    float x;
    float y;
    float z;

public:

    Ponto() {
        x = 0;
        y = 0;
        z = 0;
    }

    Ponto(float a, float b, float c) {
        x = a;
        y = b;
        z = c;
    }

    float getX() const {
        return x;
    }

    float getY() const {
        return y;
    }

    float getZ() const {
        return z;
    }

    void setX(float a) {
        x = a;
    }

    void setY(float b) {
        y = b;
    }

    void setZ(float c) {
        z = c;
    }
};

list<Ponto> pontosLista;

void readFile(string caminho3d) {
    string linha;
    vector<string> coordenadas;

    // Abrir o arquivo
    ifstream file(caminho3d);
    if (file.is_open()) {
        // Pegar na primeira linha que é o número de vértices, ou seja, número de linhas a ler (1 vértice por linha)
        getline(file, linha);
        int nLinhas = atoi(linha.c_str());
        for (int i = 1; i <= nLinhas; i++) {
            getline(file, linha);     // Pegar na linha atual
            stringstream ss(linha);
            vector<string> result{
                istream_iterator<string>(ss), {}    // Separar a linha nos espaços e guardar como array de strings em result
            };

            pontosLista.push_back(Ponto(stof(result[0]), stof(result[1]), stof(result[2]))); // Adicionar o Ponto lido à lista de pontos
        }
    }
    else {
        cout << "Erro ao ler o arquivo .3d" << endl;
    }
}

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

void drawFiguras() {
    glPointSize(5.0); // Ajustar o tamanho do ponto conforme necessário

    // Desenhar os pontos lidos do arquivo
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 1.0f); // Cor dos pontos (branco)
    for (const Ponto& ponto : pontosLista) {
        glVertex3f(ponto.getX(), ponto.getY(), ponto.getZ());
    }
    glEnd();
}

void renderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

    drawFiguras(); // Desenhar os pontos lidos

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

    // Adicionar esta linha para ler o arquivo plano.3d
   //readFile("C:\\Users\\sandr\\Downloads\\Fase1\\Fase1\\Ficheiros3D\\plane.3d");
    //readFile("C:\\Users\\sandr\\Downloads\\Fase1\\Fase1\\Ficheiros3D\\cone.3d");
    readFile("C:\\Users\\sandr\\Downloads\\Fase1\\Fase1\\Ficheiros3D\\esfera.3d");

    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

    glutKeyboardFunc(processKeys);
    glutSpecialFunc(processSpecialKeys);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glutMainLoop();

    return 1;
}