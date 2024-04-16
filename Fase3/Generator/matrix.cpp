#include "matrix.hpp"

void multiplyMatrices(int la, int ca, const float* A, // matriz A, dimensões la x ca
                      int lb, int cb, const float* B, // matriz B, dimensões lb x cb
                      float* R, int* lr = NULL, int* cr = NULL){ // matriz R, dimensões ca x lb
    if (ca == lb) {
		if (lr) *lr = ca; // Se for NULL, então é porque não se pretende guardar as dimensões da matriz resultado
		if (cr) *cr = lb; // Se for NULL, então é porque não se pretende guardar as dimensões da matriz resultado
		for (int i = 0; i < la; i++) {
			for (int j = 0; j < cb; j++) {
				R[i*cb+j] = 0;
				for (int k = 0; k < ca; k++) {
					R[i*cb+j] += A[i*ca+k] * B[k*cb+j];
				}
			}
		}
    }
} 

void buildRotMatrix(const float *x, const float *y, const float *z, float *m) {
	m[0]  = x[0]; m[1]  = x[1]; m[2]  = x[2]; m[3]  = 0;
	m[4]  = y[0]; m[5]  = y[1]; m[6]  = y[2]; m[7]  = 0;
	m[8]  = z[0]; m[9]  = z[1]; m[10] = z[2]; m[11] = 0;
	m[12] =    0; m[13] =    0; m[14] =    0; m[15] = 1;
}

void cross(const float *a, const float *b, float *res) {
	res[0] = a[1]*b[2] - a[2]*b[1];
	res[1] = a[2]*b[0] - a[0]*b[2];
	res[2] = a[0]*b[1] - a[1]*b[0];
}

void normalize(float *a) {
	float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0]/l;
	a[1] = a[1]/l;
	a[2] = a[2]/l;
}

float length(float *a){
	return sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
}

float dot(float* v1, float* v2) {
  	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

float angleVectors(float* v1, float* v2) {
  float dotProduct = dot(v1, v2);
  float l1 = length(v1);
  float l2 = length(v2);
  return acos(dotProduct / (l1 * l2));
}


void getCatmullRomPoint(float t, vector<float> p0, vector<float> p1, vector<float> p2, vector<float> p3, float *pos, float *deriv) {
	// Matriz catmull-rom
	float m[16] = {-0.5f,  1.5f, -1.5f,  0.5f,
				    1.0f, -2.5f,  2.0f, -0.5f,
				   -0.5f,  0.0f,  0.5f,  0.0f,
				    0.0f,  1.0f,  0.0f,  0.0f}; // 4x4
	// Matriz P -> contém os pontos de controlo da curva
	float P[12] = {p0[0],p0[1],p0[2],
				   p1[0],p1[1],p1[2], 
				   p2[0],p2[1],p2[2],
				   p3[0],p3[1],p3[2]}; //4x3
	float A[12]; // 4x3
    multiplyMatrices(4,4,m,4,3,P,A);
	float T[4] = {t*t*t,t*t,t,1}, DERT[4] = {3*t*t,2*t,1,0}; // T-> 1x4, DERT -> 1x4
    if(pos) multiplyMatrices(1,4,T,4,3,A,pos);
	if(deriv) multiplyMatrices(1,4,DERT,4,3,A,deriv);
}

void getGlobalCatmullRomPoint(float gt, vector<vector<float>> controlPoints, float *pos, float *deriv) {
	size_t POINT_COUNT = controlPoints.size();
	float t = gt * POINT_COUNT; // this is the real global t
	int index = floor(t);  // which segment
	t = t - index; // where within  the segment

	// indices store the points
	int indices[4]; 
	indices[0] = (index + POINT_COUNT-1)%POINT_COUNT;	
	indices[1] = (indices[0]+1)%POINT_COUNT;
	indices[2] = (indices[1]+1)%POINT_COUNT; 
	indices[3] = (indices[2]+1)%POINT_COUNT;

	getCatmullRomPoint(t, controlPoints[indices[0]], controlPoints[indices[1]], controlPoints[indices[2]], controlPoints[indices[3]], pos, deriv);
}

void surfacePoint(float u, float v, vector<vector<float>> patch, float* res){
	float M[16] = {-1.0f,  3.0f, -3.0f, 1.0f,
                   3.0f, -6.0f,  3.0f, 0.0f,
                  -3.0f,  3.0f,  0.0f, 0.0f,
                   1.0f,  0.0f,  0.0f, 0.0f}; // 4x4
	float U[4] = {u*u*u,u*u,u,1.0f}, V[4] = {v*v*v,v*v,v,1.0f}; // U: 1x4; V: 4x1
	float UM[4]; multiplyMatrices(1,4,U,4,4,M,UM); // UM: 1x4
	float MV[4]; multiplyMatrices(4,4,M,4,1,V,MV); // MV: 4x1
	float P[3][16] = {{patch[0][0],patch[1][0],patch[2][0],patch[3][0],
					   patch[4][0],patch[5][0],patch[6][0],patch[7][0],
					   patch[8][0],patch[9][0],patch[10][0],patch[11][0],
					   patch[12][0],patch[13][0],patch[14][0],patch[15][0]},
					  {patch[0][1],patch[1][1],patch[2][1],patch[3][1],
					   patch[4][1],patch[5][1],patch[6][1],patch[7][1],
					   patch[8][1],patch[9][1],patch[10][1],patch[11][1],
					   patch[12][1],patch[13][1],patch[14][1],patch[15][1]},
					  {patch[0][2],patch[1][2],patch[2][2],patch[3][2],
					   patch[4][2],patch[5][2],patch[6][2],patch[7][2],
					   patch[8][2],patch[9][2],patch[10][2],patch[11][2],
					   patch[12][2],patch[13][2],patch[14][2],patch[15][2]}}; // Três matrizes 4x4, uma para cada componente X, Y e Z
	for(int i = 0; i < 3; i++){ // Vamos calcular as componentes X, Y e Z do resultado.
		float UMP[4]; // UMP: 1x4
		multiplyMatrices(1,4,UM,4,4,P[i],UMP);
		multiplyMatrices(1,4,UMP,4,1,MV,&res[i]);
	}

}