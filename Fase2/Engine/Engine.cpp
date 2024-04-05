#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "tinyxml2.h"
#include "Transformacao.h"
#include "Modelo.h"
#include "Grupo.h"
#include <list>


using namespace tinyxml2;
using namespace std;

// Variáveis globais para a câmara
float camX, camY, camZ;
float alpha, beta, r;
double lookX, lookY, lookZ;
double upX, upY, upZ;
float fov;
float nearPlane;
float farPlane;

int grID = 0;

int windowWidth;
int windowHeight;

bool showAxes = true;         // Mostrar ou não os eixos
int tipo = GL_LINE;       // GL_LINE,  GL_FILL ou GL_Point
float red = 0.0f, green = 1.0f, blue = 0.0f;


// Vetor para armazenar os pontos lidos dos ficheiros .3d
std::vector<Point3D> globalPoints;

// lista de transformacoes onde vao ser guardados em memoria todas as transformacoes do desenho
std::list<Transformacao> transformacoesLista;

//list<Grupo> gruposLista;

std::vector<Grupo> gruposLista;


// Função para atualizar o tamanho da janela
void changeSize(int w, int h) {
	if (h == 0)
		h = 1;
	float ratio = w * 1.0 / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(fov, ratio, nearPlane, farPlane);
	glMatrixMode(GL_MODELVIEW);
}


void applyTransformations(Transformacao t) {
	switch (t.type) {
	case(0): // 0 - Translate
		glTranslatef(t.x, t.y, t.z);
		break;
	case(1): // 1 - Rotate
		glRotatef(t.angle, t.x, t.y, t.z);
		break;
	case(2): // 2 - Scale
		cout << t.x << endl;
		cout << t.y << endl;
		cout << t.z << endl;
		glScalef(t.x, t.y, t.z);
		break;
	default:
		std::cout << "Unknown transformation type\n";
		break;
	}
}


// Lê as coordenadas dos pontos de um arquivo e os armazena os num vetor global
void readPointsFromFile(const std::string& filename) {
	std::string filepath = "../../Ficheiros3D/" + filename;
	std::ifstream file(filepath);
	std::string line;

	while (std::getline(file, line)) { // Lê cada linha do arquivo
		std::istringstream iss(line);
		float x1, y1, z1, x2, y2, z2, x3, y3, z3;
		char delimiter;  //vírgula que separa as coordenadas
		iss >> x1 >> delimiter >> y1 >> delimiter >> z1 >> delimiter
			>> x2 >> delimiter >> y2 >> delimiter >> z2 >> delimiter
			>> x3 >> delimiter >> y3 >> delimiter >> z3;
		//guarda os pontos no vetor
		globalPoints.emplace_back(x1, y1, z1);
		globalPoints.emplace_back(x2, y2, z2);
		globalPoints.emplace_back(x3, y3, z3);
	}
}


void drawTriangles(vector<Point3D> pontos, float r, float g, float b) {
	glBegin(GL_TRIANGLES);
	for (size_t i = 0; i < pontos.size(); i += 3) {
		glColor3f(r, g, b);  // Usar a cor do modelo para o preenchimento e as linhas
		glVertex3f(pontos[i].x, pontos[i].y, pontos[i].z);
		glVertex3f(pontos[i + 1].x, pontos[i + 1].y, pontos[i + 1].z);
		glVertex3f(pontos[i + 2].x, pontos[i + 2].y, pontos[i + 2].z);
	}
	glEnd();
}





void drawGrupo(Grupo gR) {


	std::vector<Grupo> filhos = gR.filhos;
	std::vector<Modelo> m = gR.modelos;
	std::vector<Transformacao> t = gR.transforms;

	glPushMatrix();

	for (int i = 0; i < t.size(); i++) {
		applyTransformations(t[i]);
	}


	for (int i = 0; i < m.size(); i++) {
		drawTriangles(m[i].pontos, m[i].r, m[i].g, m[i].b);
	}

	for (int i = 0; i < filhos.size(); i++) {
		drawGrupo(filhos[i]);
	}

	glPopMatrix();

}

void drawGrupos() {
	for (int i = 0; i < gruposLista.size(); i++) {
		drawGrupo(gruposLista[i]);
	}
}



// Função para desenhar os eixos
void drawAxes() {
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
}

// Função para renderizar 
void renderScene(void) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, tipo);
	glLoadIdentity();
	gluLookAt(camX, camY, camZ, lookX, lookY, lookZ, upX, upY, upZ); //Posição inicial da câmara
	if (showAxes) {
		drawAxes();
	}
	glColor3f(red, green, blue);

	drawGrupos();

	glutSwapBuffers();
}

// Função para atualizar a posição da câmara com base nos valores de alpha, beta e r
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

void readGroup(XMLElement* group, Grupo* grupo) {

	XMLElement* models = group->FirstChildElement("models");
	if (models == nullptr) {
		cout << "No models Found" << endl;
	}
	else {
		XMLElement* model = models->FirstChildElement("model");
		if (model == nullptr) {
			cout << "No model Found" << endl;
		}

		// LER PONTOS
		while (model != nullptr) {
			const char* fileName = model->Attribute("file");
			if (fileName != nullptr) {
				Modelo modelo = *new Modelo();
				readPointsFromFile(fileName);
				modelo.pontos = globalPoints;
				// Lê a cor do modelo do XML
				XMLElement* colorElement = model->FirstChildElement("color");
				if (colorElement != nullptr) {
					modelo.r = atof(colorElement->Attribute("r"));
					modelo.g = atof(colorElement->Attribute("g"));
					modelo.b = atof(colorElement->Attribute("b"));
				}
				else {
					// Se não houver cor especificada mete cor padrão
					modelo.r = 1.0f;
					modelo.g = 1.0f;
					modelo.b = 1.0f;
				}
				grupo->modelos.push_back(modelo);
				globalPoints.clear();
			}
			model = model->NextSiblingElement("model");
		}
	}


	// LER TRANSFORMACOES
	XMLElement* test = group->FirstChildElement("transform");
	if (test != nullptr) {
		for (XMLElement* trf = group->FirstChildElement("transform")->FirstChildElement(); trf != nullptr; trf = trf->NextSiblingElement()) {
			if (strcmp(trf->Value(), "translate") == 0) {
				float x = 0, y = 0, z = 0;
				if (trf->Attribute("x") != nullptr) x = stof(trf->Attribute("x"));
				if (trf->Attribute("y") != nullptr) y = stof(trf->Attribute("y"));
				if (trf->Attribute("z") != nullptr) z = stof(trf->Attribute("z"));
				Transformacao t = *new Transformacao(0, x, y, z);
				transformacoesLista.push_back(t);
				grupo->transforms.push_back(t);
			}
			if (strcmp(trf->Value(), "rotate") == 0) {
				float x = 0, y = 0, z = 0, angle = 0;
				if (trf->Attribute("x") != nullptr) x = stof(trf->Attribute("x"));
				if (trf->Attribute("y") != nullptr) y = stof(trf->Attribute("y"));
				if (trf->Attribute("z") != nullptr) z = stof(trf->Attribute("z"));
				if (trf->Attribute("angle") != nullptr) angle = stof(trf->Attribute("angle"));
				Transformacao t = *new Transformacao(1, x, y, z, angle);
				transformacoesLista.push_back(t);
				grupo->transforms.push_back(t);
			}
			if (strcmp(trf->Value(), "scale") == 0) {
				float x = 0, y = 0, z = 0;
				if (trf->Attribute("x") != nullptr) x = stof(trf->Attribute("x"));
				if (trf->Attribute("y") != nullptr) y = stof(trf->Attribute("y"));
				if (trf->Attribute("z") != nullptr) z = stof(trf->Attribute("z"));
				Transformacao t = *new Transformacao(2, x, y, z);
				transformacoesLista.push_back(t);
				grupo->transforms.push_back(t);
			}
		}
	}


	XMLElement* filhos = group->FirstChildElement("group");

	while (filhos != nullptr) {
		Grupo filho = *new Grupo();
		filho.id = grID;
		grID++;
		readGroup(filhos, &filho);
		(*grupo).addFilho(filho);
		filhos = filhos->NextSiblingElement();
	}
}

/**
 * Lê um arquivo XML que contém as informações sobre o que deve ser renderizadoo
 *
 * @param file - nome do arquivo XML a ser lido
 */
void readXML(string file) {
	// Cria um novo documento XML
	XMLDocument xmlDoc;

	// Tenta carregar o arquivo XML
	if (xmlDoc.LoadFile(("../../FicheirosXML/" + file).c_str()) == XML_SUCCESS) {
		cout << "Arquivo XML lido com sucesso" << endl;

		// Obtém o elemento "world" como o elemento raiz do documento XML
		XMLElement* pWorld = xmlDoc.FirstChildElement("world");

		// Verifica se o elemento "world" foi encontrado
		if (!pWorld) {
			cerr << "Erro: Nenhuma tag 'world' encontrada no arquivo XML." << endl;
			return;
		}

		// Lê as informações da janela (width e height)
		XMLElement* pWindow = pWorld->FirstChildElement("window");
		if (pWindow) {
			windowWidth = atoi(pWindow->Attribute("width"));
			windowHeight = atoi(pWindow->Attribute("height"));
		}

		// Lê as informações da câmara (position, lookAt, up, projection)
		XMLElement* pCamera = pWorld->FirstChildElement("camera");
		if (pCamera) {
			XMLElement* pPosition = pCamera->FirstChildElement("position");
			XMLElement* pLookAt = pCamera->FirstChildElement("lookAt");
			XMLElement* pUp = pCamera->FirstChildElement("up");
			XMLElement* pProjection = pCamera->FirstChildElement("projection");

			// Lê as coordenadas da posição da câmara
			camX = atof(pPosition->Attribute("x"));
			camY = atof(pPosition->Attribute("y"));
			camZ = atof(pPosition->Attribute("z"));

			// Calcula os valores iniciais de alpha, beta e r com base na posição da câmara
			calculateInitialCameraValues();

			// Lê as coordenadas para onde a câmara está olhando (lookAt)
			lookX = atof(pLookAt->Attribute("x"));
			lookY = atof(pLookAt->Attribute("y"));
			lookZ = atof(pLookAt->Attribute("z"));

			// Lê as coordenadas do vetor de orientação da câmara (up)
			upX = atof(pUp->Attribute("x"));
			upY = atof(pUp->Attribute("y"));
			upZ = atof(pUp->Attribute("z"));

			// Lê os parâmetros da projeção (fov, near, far)
			fov = atof(pProjection->Attribute("fov"));
			nearPlane = atof(pProjection->Attribute("near"));
			farPlane = atof(pProjection->Attribute("far"));
		}

		XMLElement* group = xmlDoc.FirstChildElement("world")->FirstChildElement("group");
		if (group == nullptr) {
			cout << "No group Found\n" << endl;
		}

		while (group != nullptr) {
			Grupo g = *new Grupo();
			g.id = grID;
			grID++;
			readGroup(group, &g);
			gruposLista.push_back(g);
			group = group->NextSiblingElement();
		}
	}
	else {
		//  erro se ocorrer alguma falha ao ler o XML
		cout << "Erro ao ler o arquivo XML" << endl;
	}


	for (size_t i = 0; i < gruposLista.size(); ++i) {
		cout << "Grupo " << i << ":" << endl;
	
	}
}


// Função para processar as teclas especiais
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
	updateCameraPosition(); //chama a função para atualizar a posição da câmara com os novos valores de alpha, beta e r
	glutPostRedisplay();
}

// Função para processar as teclas normais
void processSpecialKeys(unsigned char key, int x, int y) {
	switch (key) {
	case '+':
		r -= 0.5f;//Aproxima a câmara 
		break;
	case '-':
		r += 0.5f;//Afasta a câmara
		break;
	case 'f':
		tipo = GL_FILL; //responsável por colorir a figura
		break;
	case 'l':
		tipo = GL_LINE; // desenha as linhas da figura 
		break;
	case 'p':
		tipo = GL_POINT;//dá display dos pontos usados para desenhar a figura
		break;
	}
	updateCameraPosition();
	glutPostRedisplay();
}

// Função principal
int main(int argc, char* argv[]) {
	if (argc == 2) {
		readXML(argv[1]);
	}
	else {
		readXML("test_2_4.xml");
	}

	// Inicialização do GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Phase 2");

	// Registro de callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutSpecialFunc(processKeys);
	glutKeyboardFunc(processSpecialKeys);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE); //Responsável por não renderizar as faces trazeiras da figura

	// Loop principal do GLUT
	glutMainLoop();

	return 1;
}