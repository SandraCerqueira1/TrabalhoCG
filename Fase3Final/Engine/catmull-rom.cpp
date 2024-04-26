#include "catmull-rom.h"
#include "Modelo.h"

/**
 * Constrói uma matriz de rotação 4x4 a partir de três vetores ortogonais.
 *
 * @param x vetor unitário na direção do eixo X.
 * @param y vetor unitário na direção do eixo Y.
 * @param z vetor unitário na direção do eixo Z.
 * @param m matriz de rotação resultante.
 */
void buildRotMatrix(float *x, float *y, float *z, float *m) {

	m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
	m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
	m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}


/**
 * Calcula o produto vetorial entre dois vetores.
 *
 * @param a primeiro vetor.
 * @param b segundo vetor.
 * @param res vetor resultante que é ortogonal a 'a' e 'b'.
 */
void cross(float *a, float *b, float *res) {

	res[0] = a[1]*b[2] - a[2]*b[1];
	res[1] = a[2]*b[0] - a[0]*b[2];
	res[2] = a[0]*b[1] - a[1]*b[0];
}


/**
 * Normaliza um vetor tridimensional.
 *
 * @param a vetor para normalizar.
 */
void normalize(float *a) {

	float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0]/l;
	a[1] = a[1]/l;
	a[2] = a[2]/l;
}


/**
 * Calcula o comprimento (magnitude) de um vetor 3D.
 *
 * @param v vetor para calcular o comprimento.
 * @return comprimento do vetor.
 */
float length(float *v) {

	float res = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	return res;

}

/**
 * Multiplica uma matriz 4x4 por um vetor 4D.
 *
 * @param m matriz 4x4.
 * @param v vetor 4D.
 * @param res vetor resultante da multiplicação.
 */
void multMatrixVector(float *m, float *v, float *res) {

	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j * 4 + k];
		}
	}

}

/**
 * Calcula o ponto e a derivada de um ponto em uma curva de Catmull-Rom.
 *
 * @param t parâmetro que indica o ponto a ser calculado na curva [0, 1].
 * @param p0 primeiro ponto de controle.
 * @param p1 segundo ponto de controle.
 * @param p2 terceiro ponto de controle.
 * @param p3 quarto ponto de controle.
 * @param pos array onde será armazenado o ponto calculado na curva.
 * @param deriv array onde será armazenada a derivada no ponto.
 */
void getCatmullRomPoint(float t, Point3D p0, Point3D p1, Point3D p2, Point3D p3, float* pos, float* deriv) {

    float M[4][4] = { {-0.5f,  1.5f, -1.5f,  0.5f},
                    { 1.0f, -2.5f,  2.0f, -0.5f},
                    {-0.5f,  0.0f,  0.5f,  0.0f},
                    { 0.0f,  1.0f,  0.0f,  0.0f} };

    float P[4][3] = { {p0.x,p0.y, p0.z},
                    {p1.x, p1.y, p1.z},
                    {p2.x, p2.y, p2.z},
                    {p3.x, p3.y, p3.z} };


    float A[4][3] = { 0 };

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 3; j++)
            for (int k = 0; k < 4; k++) A[i][j] += M[i][k] * P[k][j];

    float T[4] = { t * t * t, t * t, t, 1 };


    pos[0] = 0;
    pos[1] = 0;
    pos[2] = 0;

    for (int j = 0; j < 3; j++)
        for (int k = 0; k < 4; k++) pos[j] += T[k] * A[k][j];

    deriv[0] = 0;
    deriv[1] = 0;
    deriv[2] = 0;

    float Tderiv[4] = { 3 * t * t, 2 * t, 1, 0 };

    for (int j = 0; j < 3; j++)
        for (int k = 0; k < 4; k++) deriv[j] += Tderiv[k] * A[k][j];

}



/**
 * Dado um valor de tempo global, calcula o ponto correspondente na curva de Catmull-Rom.
 *
 * @param gt o tempo global ao longo da curva [0, 1].
 * @param pos array onde será armazenado o ponto calculado na curva.
 * @param deriv array onde será armazenada a derivada no ponto.
 * @param pontos vetor de pontos de controle que definem a curva.
 */
void getGlobalCatmullRomPoint(float gt, float* pos, float* deriv, std::vector<Point3D> pontos) {

    int tamLoop = pontos.size(); // Points that make up the loop for catmull-rom interpolation
    float t = gt * tamLoop; // this is the real global t
    int index = floor(t);  // which segment
    t = t - index; // where within  the segment

    // indices store the points
    int indices[4];
    indices[0] = (index + tamLoop - 1) % tamLoop;
    indices[1] = (indices[0] + 1) % tamLoop;
    indices[2] = (indices[1] + 1) % tamLoop;
    indices[3] = (indices[2] + 1) % tamLoop;

    getCatmullRomPoint(t, pontos[indices[0]], pontos[indices[1]], pontos[indices[2]], pontos[indices[3]], pos, deriv);
}
