#include "Objeto.h"
#include <tuple>
#include <vector>
#include <iostream>

float Objeto::produtoEscalar(vector<float> x, vector<float> y) {

	return (x[0] * y[0] + x[1] * y[1] + x[2] * y[2]);

}

vector<float> Objeto::produtoVetorial(vector<float> x, vector<float> y) {

	vector<float> z(3);

	z[0] = x[1]*y[2]-x[2]*y[1];
	z[1] = x[2]*y[0]-x[0]*y[2];
	z[2] = x[0]*y[1]-x[1]*y[0];

	return z;

}

vector<float> Objeto::somaVetores(vector<float> x, vector<float> y) {
	vector<float> v(3);
	v[0] = x[0] + y[0];
	v[1] = x[1] + y[1];
	v[2] = x[2] + y[2];
	return v;
}

vector<float> Objeto::diferencaVetores(vector<float> x, vector<float> y) {
	vector<float> v(3);
	v[0] = x[0] - y[0];
	v[1] = x[1] - y[1];
	v[2] = x[2] - y[2];
	return v;
}

vector<float> Objeto::multiplicaVetorPorEscalar(vector<float> x, float k) {
	vector<float> v(3);
	v[0] = x[0] * k;
	v[1] = x[1] * k;
	v[2] = x[2] * k;
	return v;
}

float Objeto::normaVetor(vector<float> x) {
	return (sqrt(x[0]*x[0]+x[1]*x[1]+x[2]*x[2]));
}

vector<float> Objeto::normalizaVetor(vector<float> v) {
	return multiplicaVetorPorEscalar(v, 1.0 / normaVetor(v));
}

vector<vector<float>> Objeto::multiplyMatrix(vector<vector<float>> A, vector<vector<float>> B) {
	vector<vector<float>> C(4, vector<float>(4, 0));
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	return C;
}


vector<float> Objeto::multiplyVectorByMatrix(vector<float> V, vector<vector<float>> M) {
	vector<float> V2(3, 0);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			V2[i] += M[i][j] * V[j];
		}
		V2[i] += M[i][3];
	}

	return V2;
}

vector<vector<float>> Objeto::multiplyByMatrix(vector<vector<float>> points, vector<vector<float>> M) {
	int n = size(points);
	vector<vector<float>> points2(n, vector<float>(3, 0));
	for (int i = 0; i < n; i++) {
		points2[i] = multiplyVectorByMatrix(points[i], M);
	}
	return points2;
}

vector<vector<float>> Objeto::translationMatrix(float dx, float dy, float dz) {
	vector<vector<float>> M(4, vector<float>(4, 0));
	M[0][0] = M[1][1] = M[2][2] = M[3][3] = 1;
	M[0][3] = dx;
	M[1][3] = dy;
	M[2][3] = dz;
	return M;
}

vector<vector<float>> Objeto::translationToOriginMatrix(vector<float> point) {
	vector<vector<float>> M(4, vector<float>(4, 0));
	M[0][0] = M[1][1] = M[2][2] = M[3][3] = 1;
	M[0][3] = -point[0];
	M[1][3] = -point[1];
	M[2][3] = -point[2];
	return M;
}

vector<vector<float>> Objeto::translationFromOriginMatrix(vector<float> point) {
	vector<vector<float>> M(4, vector<float>(4, 0));
	M[0][0] = M[1][1] = M[2][2] = M[3][3] = 1;
	M[0][3] = point[0];
	M[1][3] = point[1];
	M[2][3] = point[2];
	return M;
}

vector<vector<float>> Objeto::scaleMatrix(float sx, float sy, float sz) {
	vector<vector<float>> M(4, vector<float>(4, 0));
	M[0][0] = sx;
	M[1][1] = sy;
	M[2][2] = sz;
	M[3][3] = 1;
	return M;
}

vector<vector<float>> Objeto::scaleMatrixP(float sx, float sy, float sz, vector<float> p) {
	vector<vector<float>> M(4, vector<float>(4, 0));
	M[0][0] = sx;
	M[1][1] = sy;
	M[2][2] = sz;
	M[3][3] = 1;
	M[3][0] = (1 - sx)*p[0];
	M[3][1] = (1 - sy)*p[1];
	M[3][2] = (1 - sz)*p[2];
	return M;
}

vector<vector<float>> Objeto::rotationMatrixX(float ax, vector<float> pivot) {
	vector<vector<float>> M(4, vector<float>(4, 0));
	float theta = ax * 3.141592653589793238462643383279502884 / 180;
	M[3][3] = 1;
	M[0][0] = 1;
	M[1][1] = cos(theta); M[1][2] = -sin(theta); M[2][1] = sin(theta); M[2][2] = cos(theta);
	return multiplyMatrix(translationFromOriginMatrix(pivot), multiplyMatrix(M, translationToOriginMatrix(pivot)));
}

vector<vector<float>> Objeto::rotationMatrixY(float ay, vector<float> pivot) {
	vector<vector<float>> M(4, vector<float>(4, 0));
	float theta = ay * 3.141592653589793238462643383279502884 / 180;
	M[3][3] = 1;
	M[1][1] = 1;
	M[0][0] = cos(theta); M[2][0] = -sin(theta); M[0][2] = sin(theta); M[2][2] = cos(theta);
	return multiplyMatrix(translationFromOriginMatrix(pivot), multiplyMatrix(M, translationToOriginMatrix(pivot)));
}

vector<vector<float>> Objeto::rotationMatrixZ(float az, vector<float> pivot) {
	vector<vector<float>> M(4, vector<float>(4, 0));
	float theta = az * 3.141592653589793238462643383279502884 / 180;
	M[3][3] = 1;
	M[2][2] = 1;
	M[0][0] = cos(theta); M[0][1] = -sin(theta); M[1][0] = sin(theta); M[1][1] = cos(theta);
	return multiplyMatrix(translationFromOriginMatrix(pivot), multiplyMatrix(M, translationToOriginMatrix(pivot)));
}

vector<vector<float>> Objeto::shearMatrixX_XY(float shx) {
	vector<vector<float>> M(4, vector<float>(4, 0));
	M[0][0] = 1; M[1][1] = 1; M[2][2] = 1; M[3][3] = 1;
	M[0][1] = shx;
	return M;
}

vector<vector<float>> Objeto::shearMatrixY_XY(float shy) {
	vector<vector<float>> M(4, vector<float>(4, 0));
	M[0][0] = 1; M[1][1] = 1; M[2][2] = 1; M[3][3] = 1;
	M[1][0] = shy;
	return M;
}

vector<vector<float>> Objeto::shearMatrixX_XZ(float shx) {
	vector<vector<float>> M(4, vector<float>(4, 0));
	M[0][0] = 1; M[1][1] = 1; M[2][2] = 1; M[3][3] = 1;
	M[0][2] = shx;
	return M;
}

vector<vector<float>> Objeto::shearMatrixZ_XZ(float shz) {
	vector<vector<float>> M(4, vector<float>(4, 0));
	M[0][0] = 1; M[1][1] = 1; M[2][2] = 1; M[3][3] = 1;
	M[2][0] = shz;
	return M;
}

vector<vector<float>> Objeto::shearMatrixY_YZ(float shy) {
	vector<vector<float>> M(4, vector<float>(4, 0));
	M[0][0] = 1; M[1][1] = 1; M[2][2] = 1; M[3][3] = 1;
	M[1][2] = shy;
	return M;
}

vector<vector<float>> Objeto::shearMatrixZ_YZ(float shz) {
	vector<vector<float>> M(4, vector<float>(4, 0));
	M[0][0] = 1; M[1][1] = 1; M[2][2] = 1; M[3][3] = 1;
	M[2][1] = shz;
	return M;
}

vector<vector<float>> Objeto::reflectionMatrixXZ() {
	vector<vector<float>> M(4, vector<float>(4, 0));
	M[0][0] = 1; M[1][1] = -1;  M[2][2] = 1; M[3][3] = 1;
	return M;
}

vector<vector<float>> Objeto::reflectionMatrixYZ() {
	vector<vector<float>> M(4, vector<float>(4, 0));
	M[0][0] = -1; M[1][1] = 1;  M[2][2] = 1; M[3][3] = 1;
	return M;
}

vector<vector<float>> Objeto::reflectionMatrixXY() {
	vector<vector<float>> M(4, vector<float>(4, 0));
	M[0][0] = 1; M[1][1] = 1;  M[2][2] = -1; M[3][3] = 1;
	return M;
}



void Objeto::translate(float dx, float dy, float dz) {
	setListOfPoints(multiplyByMatrix(getListOfPoints(), translationMatrix(dx, dy, dz)));
}

void Objeto::translateToOrigin(vector<float> point) {
	setListOfPoints(multiplyByMatrix(getListOfPoints(), translationToOriginMatrix(point)));
}

void Objeto::translateFromOrigin(vector<float> point) {
	setListOfPoints(multiplyByMatrix(getListOfPoints(), translationFromOriginMatrix(point)));
}

void Objeto::rotateX(float ax, vector<float> pivot) {
	setListOfPoints(multiplyByMatrix(getListOfPoints(), rotationMatrixX(ax, pivot)));
}

void Objeto::rotateY(float ay, vector<float> pivot) {
	setListOfPoints(multiplyByMatrix(getListOfPoints(), rotationMatrixY(ay, pivot)));
}

void Objeto::rotateZ(float az, vector<float> pivot) {
	setListOfPoints(multiplyByMatrix(getListOfPoints(), rotationMatrixZ(az, pivot)));
}

void Objeto::reflectionXZ() {
	setListOfPoints(multiplyByMatrix(getListOfPoints(), reflectionMatrixXZ()));
}

void Objeto::reflectionYZ() {
	setListOfPoints(multiplyByMatrix(getListOfPoints(), reflectionMatrixYZ()));
}

void Objeto::reflectionXY() {
	setListOfPoints(multiplyByMatrix(getListOfPoints(), reflectionMatrixXY()));
}

void Objeto::transform(vector<vector<float>> M) {
	setListOfPoints(multiplyByMatrix(getListOfPoints(), M));
}