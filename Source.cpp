#include <GL/glut.h>
#include <iostream>
#include <new>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <tuple>
#include "ObjetoComFaces.h"
#include "Cluster.h"
#include "Cilindro.h"
#include "Cone.h"
#include "Esfera.h"

using namespace std;

vector<float> p0_c = { -2.0, 1.5, 4.0 };
vector<float> at = { 0.0, 1.5, 0.0 };
vector<float> up = { -2.0, 2.0, 4.0 };

int Width = 150;
int Height = 150;
GLfloat janela[150][150][3];

//FUN��ES COM MATRIZES E VETORES


vector<vector<float>> multiplyMatrix(vector<vector<float>> A, vector<vector<float>> B) {
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


vector<float> multiplyVectorByMatrix(vector<float> V, vector<vector<float>> M) {
	vector<float> V2(4, 0);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			V2[i] += M[i][j] * V[j];
		}
		V2[i] += M[i][3];
	}

	return V2;
}




vector<vector<float>> multiplyByMatrix(int n, vector<vector<float>> points, vector<vector<float>> M) {
	vector<vector<float>> points2(n, vector<float>(3, 0));
	for (int i = 0; i < n; i++) {
		points2[i] = multiplyVectorByMatrix(points[i], M);
	}
	return points2;
}

vector<vector<float>> translationMatrix(float dx, float dy, float dz) {
	vector<vector<float>> M(4, vector<float>(4, 0));
	M[0][0] = M[1][1] = M[2][2] = M[3][3] = 1;
	M[0][3] = dx;
	M[1][3] = dy;
	M[2][3] = dz;
	return M;
}

vector<vector<float>> translationToOriginMatrix(vector<float> point) {
	vector<vector<float>> M(4, vector<float>(4, 0));
	M[0][0] = M[1][1] = M[2][2] = M[3][3] = 1;
	M[0][3] = -point[0];
	M[1][3] = -point[1];
	M[2][3] = -point[2];
	return M;
}

vector<vector<float>> translationFromOriginMatrix(vector<float> point) {
	vector<vector<float>> M(4, vector<float>(4, 0));
	M[0][0] = M[1][1] = M[2][2] = M[3][3] = 1;
	M[0][3] = point[0];
	M[1][3] = point[1];
	M[2][3] = point[2];
	return M;
}

vector<vector<float>> scaleMatrix(float sx, float sy, float sz) {
	vector<vector<float>> M(4, vector<float>(4, 0));
	M[0][0] = sx;
	M[1][1] = sy;
	M[2][2] = sz;
	M[3][3] = 1;
	return M;
}

vector<vector<float>> rotationMatrixX(float ax, vector<float> pivot = { 0, 0, 0 }) {
	vector<vector<float>> M(4, vector<float>(4, 0));
	float theta = ax * 3.141592653589793238462643383279502884 / 180;
	M[3][3] = 1;
	M[0][0] = 1;
	M[1][1] = cos(theta); M[1][2] = -sin(theta); M[2][1] = sin(theta); M[2][2] = cos(theta);
	return multiplyMatrix(translationFromOriginMatrix(pivot), multiplyMatrix(M, translationToOriginMatrix(pivot)));
}

vector<vector<float>> rotationMatrixY(float ay, vector<float> pivot = { 0, 0, 0 }) {
	vector<vector<float>> M(4, vector<float>(4, 0));
	float theta = ay * 3.141592653589793238462643383279502884 / 180;
	M[3][3] = 1;
	M[1][1] = 1;
	M[0][0] = cos(theta); M[2][0] = -sin(theta); M[0][2] = sin(theta); M[2][2] = cos(theta);
	return multiplyMatrix(translationFromOriginMatrix(pivot), multiplyMatrix(M, translationToOriginMatrix(pivot)));
}

vector<vector<float>> rotationMatrixZ(float az, vector<float> pivot = { 0, 0, 0 }) {
	vector<vector<float>> M(4, vector<float>(4, 0));
	float theta = az * 3.141592653589793238462643383279502884 / 180;
	M[3][3] = 1;
	M[2][2] = 1;
	M[0][0] = cos(theta); M[0][1] = -sin(theta); M[1][0] = sin(theta); M[1][1] = cos(theta);
	return multiplyMatrix(translationFromOriginMatrix(pivot), multiplyMatrix(M, translationToOriginMatrix(pivot)));
}

//FUN��ES PARA PINTAR POL�GONOS


void triangle(vector<vector<float>> poly, int a, int b, int c, float color[3]) {
	glBegin(GL_TRIANGLES);
	glColor3fv(color);
	glVertex3fv(poly[a].data());
	glVertex3fv(poly[b].data());
	glVertex3fv(poly[c].data());
	glEnd();
}

void quad(vector<vector<float>> poly, int a, int b, int c, int d, GLfloat color[3])
{
	glBegin(GL_QUADS);
	glColor3fv(color);
	glVertex3fv(poly[a].data());
	glVertex3fv(poly[b].data());
	glVertex3fv(poly[c].data());
	glVertex3fv(poly[d].data());
	glEnd();

}


//FORMAS B�SICAS


vector<vector<float>> basicTriangularPyramid{
	{ 0.0f,0.0f,0.0f },
	{ 0.5f,0.0f,1.0f },
	{ 1.0f,0.0f,0.0f },
	{ 0.5f,1.0f,0.33f }
};

void colorTriangularPyramid(vector<vector<float>> triangularPyramid, GLfloat color[3]) {
	triangle(triangularPyramid, 1, 2, 3, color);
	triangle(triangularPyramid, 2, 0, 3, color);
	triangle(triangularPyramid, 0, 1, 3, color);
	triangle(triangularPyramid, 0, 2, 1, color);
}

vector<vector<float>> basicSquarePyramid{
	{ 0.0f,0.0f,0.0f },
	{ 0.0f,0.0f,1.0f },
	{ 1.0f,0.0f,1.0f },
	{ 1.0f,0.0f,0.0f },
	{ 0.5f,1.0f,0.5f },
};

void colorSquarePyramid(vector<vector<float>> squarePyramid, GLfloat color[3]) {
	quad(squarePyramid, 0, 1, 2, 3, color);
	triangle(squarePyramid, 1, 2, 4, color);
	triangle(squarePyramid, 2, 3, 4, color);
	triangle(squarePyramid, 3, 0, 4, color);
	triangle(squarePyramid, 0, 1, 4, color);
}

vector<vector<float>> roof{
	{ 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 0.0f },
	{ 0.5f, 1.0f, 0.0f },
	{ 0.5f, 1.0f, 1.0f },
};

void colorRoof(vector<vector<float>> roof, GLfloat colorHouse[3], GLfloat colorRoof[3]) {
	quad(roof, 0, 1, 2, 3, colorHouse);
	triangle(roof, 5, 1, 2, colorHouse);
	triangle(roof, 0, 4, 3, colorHouse);
	quad(roof, 5, 2, 3, 4, colorRoof);
	quad(roof, 0, 1, 5, 4, colorRoof);

}

vector<vector<float>> basicCube{
	{ 0.0f,0.0f,0.0f },
	{ 0.0f,0.0f,1.0f },
	{ 1.0f,0.0f,1.0f },
	{ 1.0f,0.0f,0.0f },
	{ 0.0f,1.0f,0.0f },
	{ 0.0f,1.0f,1.0f },
	{ 1.0f,1.0f,1.0f },
	{ 1.0f,1.0f,0.0f },
};

void colorBox(vector<vector<float>> box, GLfloat color[3]) {
	quad(box, 4, 0, 1, 5, color);
	quad(box, 5, 1, 2, 6, color);
	quad(box, 6, 2, 3, 7, color);
	quad(box, 7, 3, 0, 4, color);
	quad(box, 5, 6, 7, 4, color);
	quad(box, 1, 0, 3, 2, color);
}

vector<vector<float>> ship{
	{ 0.0f,0.0f,0.0f },
	{ 0.0f,0.0f,1.0f },
	{ 1.0f,0.0f,1.0f },
	{ 1.0f,0.0f,0.0f },
	{ -0.2f,1.0f,0.0f },
	{ -0.2f,1.0f,1.0f },
	{ 1.2f,1.0f,1.0f },
	{ 1.2f,1.0f,0.0f },
};

// FUN��ES PRINCIPAIS

double rotate_y = 0;
double rotate_x = 0;
void specialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_RIGHT)
		rotate_y += 5;
	else if (key == GLUT_KEY_LEFT)
		rotate_y -= 5;
	else if (key == GLUT_KEY_UP)
		rotate_x += 5;
	else if (key == GLUT_KEY_DOWN)
		rotate_x -= 5;
	glutPostRedisplay();
}

//vector<vector<float>> intensidadeLuzDistante = { {0.0, 0.0, 0.0},{ 0.0, 0.0, 0.0 },{ 0.0, 0.0, 0.0 } };
vector<vector<float>> intensidadeLuzDistante = { { 0.2f, 0.2f, 0.2f },{ 0.4f, 0.4f, 0.4f },{ 0.4f, 0.4f, 0.4f } };
vector<vector<float>> intensidadeLuzPontual = { { 0.3f, 0.3f, 0.3f },{ 0.6f, 0.6f, 0.6f },{ 0.6f, 0.6f, 0.6f } }; float a = 1.0; float b = 0.005; float c = 0.001; float raioFonteDeLuzPontual = 0.3;


tuple<GLfloat, GLfloat, GLfloat> corPixel(vector<Objeto*> objetos, vector<vector<float>> M_CW, vector<float> direcaoLuzD, float fatorLuzDCeuGramaMar, vector<float> pLuzPontual, float p0x, float p0y, float p0z, float dx, float dy, float dz)
{

	GLfloat R;
	GLfloat G;
	GLfloat B;

	bool intersecta = false;
	float t = 100000;

	bool x;
	float y;
	vector<vector<float>> z, material;
	vector<float> w, normal;

	for (int i = 0; i < size(objetos); i++) {
		tie(x, y, z, w) = (*objetos[i]).hasIntersection(p0x, p0y, p0z, dx, dy, dz);
		if (x == true) {
			if (y < t) {
				intersecta = true;
				t = y;
				material = z;
				normal = w;
			}
		}
	}

	if (intersecta == true) {

		//OBJETO
		float fatorAnguloD = fmax( normal[0] * direcaoLuzD[0] + normal[1] * direcaoLuzD[1] + normal[2] * direcaoLuzD[2], 0 );
		vector<float> direcaoLuzRD = Objeto::diferencaVetores(Objeto::multiplicaVetorPorEscalar(normal, 2 * Objeto::produtoEscalar(direcaoLuzD, normal)), direcaoLuzD);
		float fatorAnguloRD = fmax( pow( direcaoLuzRD[0]*(-dx) - direcaoLuzRD[1]*(-dy) - direcaoLuzRD[2]*(-dz), 1.0 ), 0);
		R = material[0][0] * intensidadeLuzDistante[0][0] + material[1][0] * intensidadeLuzDistante[1][0] * fatorAnguloD + material[2][0] * intensidadeLuzDistante[2][0] * fatorAnguloRD;
		G = material[0][1] * intensidadeLuzDistante[0][1] + material[1][1] * intensidadeLuzDistante[1][1] * fatorAnguloD + material[2][1] * intensidadeLuzDistante[2][1] * fatorAnguloRD;
		B = material[0][2] * intensidadeLuzDistante[0][2] + material[1][2] * intensidadeLuzDistante[1][2] * fatorAnguloD + material[2][2] * intensidadeLuzDistante[2][2] * fatorAnguloRD;

		vector<float> pInt = { p0x + dx*t, p0y + dy*t, p0z + dz*t };
		vector<float> l0 = Objeto::diferencaVetores(pLuzPontual, pInt);
		float d = pow(Objeto::normaVetor(l0), 2);
		float fatorD; float fatorAnguloP; float fatorAnguloRP;

		if (d < (raioFonteDeLuzPontual+0.01)) {
			fatorD = 1.0 / a;
			fatorAnguloP = 1.0;
			fatorAnguloRP = 1.0;
		}
		else {
			vector<float> l = Objeto::normalizaVetor(l0);
			vector<float> r = Objeto::diferencaVetores(Objeto::multiplicaVetorPorEscalar(normal, 2 * Objeto::produtoEscalar(l, normal)), l);
			fatorD = 1.0 / (a + b*d + c*d*d);
			fatorAnguloP = fmax(Objeto::produtoEscalar(normal, l), 0);
			fatorAnguloRP = fmax(pow(Objeto::produtoEscalar(r, { -dx,-dy, -dz }), 1.0), 0);
		}


		R += material[0][0] * intensidadeLuzPontual[0][0] * fatorD + material[1][0] * intensidadeLuzPontual[1][0] * fatorD * fatorAnguloP + material[2][0] * intensidadeLuzPontual[2][0] * fatorD * fatorAnguloRP;
		G += material[0][1] * intensidadeLuzPontual[0][1] * fatorD + material[1][1] * intensidadeLuzPontual[1][1] * fatorD * fatorAnguloP + material[2][1] * intensidadeLuzPontual[2][1] * fatorD * fatorAnguloRP;
		B += material[0][2] * intensidadeLuzPontual[0][2] * fatorD + material[1][2] * intensidadeLuzPontual[1][2] * fatorD * fatorAnguloP + material[2][2] * intensidadeLuzPontual[2][2] * fatorD * fatorAnguloRP;
		
	}
	else {
		vector<float> p0_W = multiplyVectorByMatrix({ p0x, p0y, p0z }, M_CW);
		vector<float> d_W = multiplyVectorByMatrix({ p0x + dx, p0y + dy, p0z + dz }, M_CW);
		p0x = p0_W[0]; p0y = p0_W[1]; p0z = p0_W[2];
		dx = d_W[0]-p0x; dy = d_W[1]-p0y; dz = d_W[2]-p0z;
		if (dy <= 0) {
			float zIntChao = p0z + dz*(-p0y / dy);
			if (zIntChao > 0) {
				//MAR
				R = 0.0;
				G = 0.412*intensidadeLuzDistante[0][1] + 0.412*intensidadeLuzDistante[1][1]*fatorLuzDCeuGramaMar;
				B = 0.58*intensidadeLuzDistante[0][2] + 0.58*intensidadeLuzDistante[1][2]*fatorLuzDCeuGramaMar;
			}
			else {
				//GRAMA
				R = 0.0;
				G = 0.602*intensidadeLuzDistante[0][1] + 0.602*intensidadeLuzDistante[1][1]*fatorLuzDCeuGramaMar;
				B = 0.09*intensidadeLuzDistante[0][2] + 0.09*intensidadeLuzDistante[1][2]*fatorLuzDCeuGramaMar;
			}
		}
		else {
			//CEU
			R = 0.529*intensidadeLuzDistante[0][0] + 0.529*intensidadeLuzDistante[1][0]*fatorLuzDCeuGramaMar;
			G = 0.808*intensidadeLuzDistante[0][1] + 0.808*intensidadeLuzDistante[1][1]*fatorLuzDCeuGramaMar;
			B = 0.922*intensidadeLuzDistante[0][2] + 0.922*intensidadeLuzDistante[1][2]*fatorLuzDCeuGramaMar;
		}
	}

	return make_tuple(R, G, B);

}

void lancarRaios(vector<Objeto*> objetos, vector<vector<float>> M_CW, vector<float> dLuzDistante, vector<float>pLuzPontual, float left, float right, float bottom, float top, float p0x, float p0y, float p0z, float z)
{

	float R, G, B;

	float d0x, d0y, d0z, tam, max = 0.0;

	float fatorLuzDCeuGramaMar = Objeto::produtoEscalar(multiplyVectorByMatrix(dLuzDistante, M_CW), { 0.0,1.0,0.0 });

	for (int i = 0; i < Height; i++) {
		for (int j = 0; j < Width; j++) {

			d0x = (left + (right - left)*(0.5 + j) / Width) - p0x;
			d0y = (bottom + (top - bottom)*(0.5 + i) / Height) - p0y;
			d0z = z - p0z;
			tam = sqrt(d0x*d0x + d0y*d0y + d0z*d0z);
			auto tup = corPixel(objetos, M_CW, dLuzDistante, fatorLuzDCeuGramaMar, pLuzPontual, p0x, p0y, p0z, d0x/tam, d0y/tam, d0z/tam);

			R = get<0>(tup), G = get<1>(tup), B = get<2>(tup);
			janela[i][j][0] = R;
			janela[i][j][1] = G;
			janela[i][j][2] = B;
			if (R > max) {
				max = R;
			}
			if (G > max) {
				max = G;
			}
			if (B > max) {
				max = B;
			}
		}
	}

	if (max > 1.0) {

		for (int i = 0; i < Height; i++) {
			for (int j = 0; j < Width; j++) {
				janela[i][j][0] /= max;
				janela[i][j][1] /= max;
				janela[i][j][2] /= max;
			}
		}

	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawPixels(Width, Height, GL_RGB, GL_FLOAT, janela);
	glutSwapBuffers();

}

tuple<GLfloat, GLfloat, GLfloat> corPixelOrtho(vector<Objeto*> objetos, vector<vector<float>> M_CW, vector<float> direcaoLuzD, float fatorLuzDCeuGramaMar, vector<float> pLuzPontual, float p0x, float p0y, float p0z, float dx, float dy, float dz)
{

	GLfloat R;
	GLfloat G;
	GLfloat B;

	bool intersecta = false;
	float t = 100000;

	bool x;
	float y;
	vector<vector<float>> z, material;
	vector<float> w, normal;

	for (int i = 0; i < size(objetos); i++) {
		tie(x, y, z, w) = (*objetos[i]).hasIntersection(p0x, p0y, p0z, dx, dy, dz);
		if (x == true) {
			if (y < t) {
				intersecta = true;
				t = y;
				material = z;
				normal = w;
			}
		}
	}

	if (intersecta == true) {

		//OBJETO
		float fatorAnguloD = fmax(normal[0] * direcaoLuzD[0] + normal[1] * direcaoLuzD[1] + normal[2] * direcaoLuzD[2], 0);
		vector<float> direcaoLuzRD = Objeto::diferencaVetores(Objeto::multiplicaVetorPorEscalar(normal, 2 * Objeto::produtoEscalar(direcaoLuzD, normal)), direcaoLuzD);
		float fatorAnguloRD = fmax(pow(direcaoLuzRD[0] * (-dx) - direcaoLuzRD[1] * (-dy) - direcaoLuzRD[2] * (-dz), 1.0), 0);
		R = material[0][0] * intensidadeLuzDistante[0][0] + material[1][0] * intensidadeLuzDistante[1][0] * fatorAnguloD + material[2][0] * intensidadeLuzDistante[2][0] * fatorAnguloRD;
		G = material[0][1] * intensidadeLuzDistante[0][1] + material[1][1] * intensidadeLuzDistante[1][1] * fatorAnguloD + material[2][1] * intensidadeLuzDistante[2][1] * fatorAnguloRD;
		B = material[0][2] * intensidadeLuzDistante[0][2] + material[1][2] * intensidadeLuzDistante[1][2] * fatorAnguloD + material[2][2] * intensidadeLuzDistante[2][2] * fatorAnguloRD;

		vector<float> pInt = { p0x + dx*t, p0y + dy*t, p0z + dz*t };
		vector<float> l0 = Objeto::diferencaVetores(pLuzPontual, pInt);
		float d = pow(Objeto::normaVetor(l0), 2);
		float fatorD; float fatorAnguloP; float fatorAnguloRP;

		if (d < (raioFonteDeLuzPontual + 0.01)) {
			fatorD = 1.0 / a;
			fatorAnguloP = 1.0;
			fatorAnguloRP = 1.0;
		}
		else {
			vector<float> l = Objeto::normalizaVetor(l0);
			vector<float> r = Objeto::diferencaVetores(Objeto::multiplicaVetorPorEscalar(normal, 2 * Objeto::produtoEscalar(l, normal)), l);
			fatorD = 1.0 / (a + b*d + c*d*d);
			fatorAnguloP = fmax(Objeto::produtoEscalar(normal, l), 0);
			fatorAnguloRP = fmax(pow(Objeto::produtoEscalar(r, { -dx,-dy, -dz }), 1.0), 0);
		}


		R += material[0][0] * intensidadeLuzPontual[0][0] * fatorD + material[1][0] * intensidadeLuzPontual[1][0] * fatorD * fatorAnguloP + material[2][0] * intensidadeLuzPontual[2][0] * fatorD * fatorAnguloRP;
		G += material[0][1] * intensidadeLuzPontual[0][1] * fatorD + material[1][1] * intensidadeLuzPontual[1][1] * fatorD * fatorAnguloP + material[2][1] * intensidadeLuzPontual[2][1] * fatorD * fatorAnguloRP;
		B += material[0][2] * intensidadeLuzPontual[0][2] * fatorD + material[1][2] * intensidadeLuzPontual[1][2] * fatorD * fatorAnguloP + material[2][2] * intensidadeLuzPontual[2][2] * fatorD * fatorAnguloRP;

	}
	else {
		vector<float> p0_W = multiplyVectorByMatrix({ p0x, p0y, p0z }, M_CW);
		vector<float> d_W = multiplyVectorByMatrix({ p0x + dx, p0y + dy, p0z + dz }, M_CW);
		p0x = p0_W[0]; p0y = p0_W[1]; p0z = p0_W[2];
		dx = d_W[0] - p0x; dy = d_W[1] - p0y; dz = d_W[2] - p0z;

		if (p0y <= 0) {
			if (p0z > 0) {
				//MAR
				R = 0.0;
				G = 0.412*intensidadeLuzDistante[0][1] + 0.412*intensidadeLuzDistante[1][1] * fatorLuzDCeuGramaMar;
				B = 0.58*intensidadeLuzDistante[0][2] + 0.58*intensidadeLuzDistante[1][2] * fatorLuzDCeuGramaMar;
			}
			else {
				//GRAMA
				R = 0.0;
				G = 0.602*intensidadeLuzDistante[0][1] + 0.602*intensidadeLuzDistante[1][1] * fatorLuzDCeuGramaMar;
				B = 0.09*intensidadeLuzDistante[0][2] + 0.09*intensidadeLuzDistante[1][2] * fatorLuzDCeuGramaMar;
			}
		}
		else {
			//CEU
			R = 0.529*intensidadeLuzDistante[0][0] + 0.529*intensidadeLuzDistante[1][0] * fatorLuzDCeuGramaMar;
			G = 0.808*intensidadeLuzDistante[0][1] + 0.808*intensidadeLuzDistante[1][1] * fatorLuzDCeuGramaMar;
			B = 0.922*intensidadeLuzDistante[0][2] + 0.922*intensidadeLuzDistante[1][2] * fatorLuzDCeuGramaMar;
		}
	}

	return make_tuple(R, G, B);

}

void lancarRaiosOrtho(vector<Objeto*> objetos, vector<vector<float>> M_CW, vector<float> dLuzDistante, vector<float>pLuzPontual, float left, float right, float bottom, float top, float p0x, float p0y, float p0z) {

	float R, G, B;

	float d0x, d0y, d0z, tam, max = 0.0;

	float fatorLuzDCeuGramaMar = Objeto::produtoEscalar(multiplyVectorByMatrix(dLuzDistante, M_CW), { 0.0,1.0,0.0 });

	for (int i = 0; i < Height; i++) {
		for (int j = 0; j < Width; j++) {

			float p0xR = (left + (right - left)*(0.5 + j) / Width);
			float p0yR = (bottom + (top - bottom)*(0.5 + i) / Height);
			float p0zR = 0;
			vector<float> dR = { 0.0,0.0,-1.0 };
			auto tup = corPixelOrtho(objetos, M_CW, dLuzDistante, fatorLuzDCeuGramaMar, pLuzPontual, p0xR, p0yR, p0zR, dR[0], dR[1], dR[2]);
			
			R = get<0>(tup), G = get<1>(tup), B = get<2>(tup);
			janela[i][j][0] = R;
			janela[i][j][1] = G;
			janela[i][j][2] = B;
			if (R > max) {
				max = R;
			}
			if (G > max) {
				max = G;
			}
			if (B > max) {
				max = B;
			}
		}
	}

	if (max > 1.0) {

		for (int i = 0; i < Height; i++) {
			for (int j = 0; j < Width; j++) {
				janela[i][j][0] /= max;
				janela[i][j][1] /= max;
				janela[i][j][2] /= max;
			}
		}

	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawPixels(Width, Height, GL_RGB, GL_FLOAT, janela);
	glutSwapBuffers();

}

void display()
{

	//definindo materiais usados
	
	vector<vector<float>> pineGreen = { { 0.004f, 0.475f, 0.318f },{ 0.004f, 0.475f, 0.318f },{ 0.0f, 0.0f, 0.0f } };
	vector<vector<float>> lightYellow = { {255.0f / 255.0f, 255.0f / 255.0f, 153.0f / 255.0f},{ 255.0f / 255.0f, 255.0f / 255.0f, 153.0f / 255.0f }, {0.0f, 0.0f, 0.0f} };
	vector<vector<float>> treeBrown = { { 119.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f },{ 119.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f },{ 0.0f, 0.0f, 0.0f } };
	vector<vector<float>> crimson = { { 220.0f / 255.0f, 20.0f / 255.0f, 60.0f / 255.0f },{ 220.0f / 255.0f, 20.0f / 255.0f, 60.0f / 255.0f },{ 0.0f, 0.0f, 0.0f } };
	vector<vector<float>> brown = { { 210.0f / 255.0f, 105.0f / 255.0f, 30.0f / 255.0f },{ 210.0f / 255.0f, 105.0f / 255.0f, 30.0f / 255.0f },{ 0.0f, 0.0f, 0.0f } };
	vector<vector<float>> lightBrown = { { 0.71f,0.396f,0.114f },{ 0.71f,0.396f,0.114f },{ 0.0f, 0.0f, 0.0f } };
	vector<vector<float>> blueGlass = { { 140.0f / 255.0f,210.0f / 255.0f,255.0f / 255.0f },{ 70.0f / 255.0f,105.0f / 255.0f,128.0f / 255.0f }, { 70.0f / 128.0f,210.0f / 255.0f,128.0f / 255.0f } };
	vector<vector<float>> darkBlue = { { 0.0f, 0.0f, 128.0f / 256.0f },{ 0.0f, 0.0f, 128.0f / 256.0f },{ 0.0f, 0.0f, 0.0f } };
	vector<vector<float>> greenBrown = { { 74.0f / 256.0f, 67.0f / 256.0f, 0.0f },{ 74.0f / 256.0f, 67.0f / 256.0f, 0.0f },{ 0.0f, 0.0f, 0.0f } };
	vector<vector<float>> red = { { 0.659f, 0.0f, 0.0f },{ 0.659f, 0.0f, 0.0f },{ 0.0f, 0.0f, 0.0f } };
	vector<vector<float>> mountainBlue = { { 0.6314f, 0.2392f, 0.1765f },{ 0.6314f, 0.2392f, 0.1765f }, { 0.0f, 0.0f, 0.0f } };
	vector<vector<float>> black = { { 0.05f, 0.05f, 0.05f },{ 0.05f, 0.05f, 0.05f },{ 0.0f, 0.0f, 0.0f } };
	vector<vector<float>> white = { { 1.0f, 1.0f, 1.0f },{ 0.9f, 0.9f, 0.9f },{ 0.1f, 0.1f, 0.1f } };

	//faces para figuras basicas

	vector<vector<int>> boxFaces = { { 4,0,1,5 },{ 5,1,2,6 },{ 6,2,3,7 },{ 7,3,0,4 },{ 5,6,7,4 },{ 1,0,3,2 } };
	vector<vector<int>> triangularPyramidFaces = { { 1,2,3 },{ 2,0,3 },{ 0,1,3 },{ 0,2,1 } };
	vector<vector<int>> squarePyramidFaces = { {0,1,2,3},{1,2,4},{2,3,4},{3,0,4},{0,1,4} };
	vector<vector<int>> roofFaces = { {0,1,2,3},{5,1,2},{0,4,3},{5,2,3,4},{0,1,5,4} };

	//figuras base para pinheiros

	vector<vector<float>> topoArvore = multiplyByMatrix(4, basicTriangularPyramid, multiplyMatrix(translationMatrix(-0.25, 1.0, -0.25), scaleMatrix(0.6, 0.8, 0.6)));
	vector<vector<float>> troncoArvore = multiplyByMatrix(8, basicCube, scaleMatrix(0.1, 1.0, 0.1));

	//desenhando arvores

	ObjetoComFaces a1Tronco = ObjetoComFaces(multiplyByMatrix(8, troncoArvore, translationMatrix(-3.0, 0.0, -7.0)), boxFaces, treeBrown);
	ObjetoComFaces a1Folhas1 = ObjetoComFaces(multiplyByMatrix(4, topoArvore, translationMatrix(-3.0, 0.0, -7.0)), triangularPyramidFaces, pineGreen);
	ObjetoComFaces a1Folhas2 = ObjetoComFaces(multiplyByMatrix(4, topoArvore, translationMatrix(-3.0, 0.2, -7.0)), triangularPyramidFaces, pineGreen);
	ObjetoComFaces a1Folhas3 = ObjetoComFaces(multiplyByMatrix(4, topoArvore, translationMatrix(-3.0, 0.4, -7.0)), triangularPyramidFaces, pineGreen);
	ObjetoComFaces a1Folhas4 = ObjetoComFaces(multiplyByMatrix(4, topoArvore, translationMatrix(-3.0, 0.6, -7.0)), triangularPyramidFaces, pineGreen);

	a1Tronco.rotateZ(10, a1Tronco.centro);
	a1Folhas1.rotateZ(10, a1Tronco.centro);
	a1Folhas2.rotateZ(10, a1Tronco.centro);
	a1Folhas3.rotateZ(10, a1Tronco.centro);
	a1Folhas4.rotateZ(10, a1Tronco.centro);

	ObjetoComFaces a2Tronco = ObjetoComFaces(multiplyByMatrix(8, troncoArvore, translationMatrix(0.7, 0.0, -7.0)), boxFaces, treeBrown);
	ObjetoComFaces a2Folhas1 = ObjetoComFaces(multiplyByMatrix(4, topoArvore, translationMatrix(0.7, 0.0, -7.0)), triangularPyramidFaces, pineGreen);
	ObjetoComFaces a2Folhas2 = ObjetoComFaces(multiplyByMatrix(4, topoArvore, translationMatrix(0.7, 0.2, -7.0)), triangularPyramidFaces, pineGreen);
	ObjetoComFaces a2Folhas3 = ObjetoComFaces(multiplyByMatrix(4, topoArvore, translationMatrix(0.7, 0.4, -7.0)), triangularPyramidFaces, pineGreen);
	ObjetoComFaces a2Folhas4 = ObjetoComFaces(multiplyByMatrix(4, topoArvore, translationMatrix(0.7, 0.6, -7.0)), triangularPyramidFaces, pineGreen);

	ObjetoComFaces a3Tronco = ObjetoComFaces(a2Tronco);
	ObjetoComFaces a3Folhas1 = ObjetoComFaces(a2Folhas1);
	ObjetoComFaces a3Folhas2 = ObjetoComFaces(a2Folhas2);
	ObjetoComFaces a3Folhas3 = ObjetoComFaces(a2Folhas3);
	ObjetoComFaces a3Folhas4 = ObjetoComFaces(a2Folhas4);
	a3Tronco.reflectionYZ();
	a3Folhas1.reflectionYZ();
	a3Folhas2.reflectionYZ();
	a3Folhas3.reflectionYZ();
	a3Folhas4.reflectionYZ();

	Cilindro tronco1 = Cilindro({ 0.5, 0.0, -4.0 }, { 0.0, 1.0, 0.0 }, 1.0, 0.1, brown);
	Cone cone1 = Cone({ 0.5f, 1.0f, -4.0f }, { 0.0f, 1.0f, 0.0f }, 1.2f, 0.3f, pineGreen);
	Cilindro tronco2 = Cilindro(tronco1);
	Cone cone2 = Cone(cone1);
	tronco2.reflectionYZ();
	cone2.reflectionYZ();

	ObjetoComFaces tronco3 = ObjetoComFaces(multiplyByMatrix(8, troncoArvore, translationMatrix(7.0, 0.0, -2.0)), boxFaces, treeBrown);
	Esfera topoEsfericoArvore = Esfera({7.05f, 1.45f, -1.95f}, 0.8, pineGreen);

	//desenhando igreja

	ObjetoComFaces baseTorre = ObjetoComFaces(multiplyByMatrix(8, basicCube, multiplyMatrix(translationMatrix(2.0, 0.0, -7.2), scaleMatrix(0.8, 5.0, 0.8))), boxFaces, lightYellow);
	ObjetoComFaces topoTorre = ObjetoComFaces(multiplyByMatrix(5, basicSquarePyramid, multiplyMatrix(translationMatrix(2.05, 5.0, -7.15), scaleMatrix(0.7, 1.7, 0.7))), squarePyramidFaces, crimson);
	ObjetoComFaces cruzParteVertical = ObjetoComFaces(multiplyByMatrix(8, basicCube, multiplyMatrix(translationMatrix(2.385, 6.7, -6.815), scaleMatrix(0.03, 1.0, 0.03))), boxFaces, brown);
	ObjetoComFaces cruzParteHorizontal = ObjetoComFaces(multiplyByMatrix(8, basicCube, multiplyMatrix(translationMatrix(2.15, 7.3, -6.95), scaleMatrix(0.5, 0.03, 0.03))), boxFaces, brown);
	ObjetoComFaces baseIgreja = ObjetoComFaces(multiplyByMatrix(8, basicCube, multiplyMatrix(translationMatrix(2.0, 0.0, -10.2), scaleMatrix(2.0, 2.5, 3.0))), boxFaces , lightYellow);
	ObjetoComFaces topoIgreja = ObjetoComFaces(multiplyByMatrix(6, roof, multiplyMatrix(translationMatrix(2.0, 2.5, -10.2), scaleMatrix(2.0, 2.5, 3.0))), roofFaces, {lightYellow, lightYellow, lightYellow, crimson, crimson});

	//desenhando extremidades do telhado

	ObjetoComFaces telhado = ObjetoComFaces({ { 4.0f, 2.5f, -7.19f },{ 3.0f, 5.0f, -7.19f },{ 2.9f, 5.0f, -7.19f },{ 3.9f, 2.5f, -7.19f } }, { { 0, 1, 2, 3 } }, crimson);

	//desenhando janelas

	ObjetoComFaces janelaTorreFrenteAlto = ObjetoComFaces({ { 2.6f, 3.6f, -6.39f },{ 2.6f, 4.4f, -6.39f },{ 2.2f, 4.4f, -6.39f },{ 2.2f, 3.6f, -6.39f } }, { { 0, 1, 2, 3 } }, blueGlass);
	ObjetoComFaces janelaTorreEsquerdaAlto = ObjetoComFaces({ { 1.99f, 3.6f, -6.6f },{ 1.99f, 4.4f, -6.6f },{ 1.99f, 4.4f, -7.0f },{ 1.99f, 3.6f, -7.0f } }, { { 0, 1, 2, 3 } }, blueGlass);
	ObjetoComFaces janelaTorreFrenteBaixo = ObjetoComFaces({ { 2.6f, 1.2f, -6.39f },{ 2.6f, 2.0f, -6.39f },{ 2.2f, 2.0f, -6.39f },{ 2.2f, 1.2f, -6.39f } }, { { 0, 1, 2, 3 } }, blueGlass);
	ObjetoComFaces janelaTorreEsquerdaBaixo = ObjetoComFaces({ { 1.99f, 1.2f, -6.6f },{ 1.99f, 2.0f, -6.6f },{ 1.99f, 2.0f, -7.0f },{ 1.99f, 1.2f, -7.0f } }, { { 0, 1, 2, 3 } }, blueGlass);
	ObjetoComFaces janelaIgrejaEsquerdaFrente = ObjetoComFaces({ { 1.99f, 1.2f, -7.4f },{ 1.99f, 2.0f, -7.4f },{ 1.99f, 2.0f, -7.8f },{ 1.99f, 1.2f, -7.8f } }, { { 0, 1, 2, 3 } }, blueGlass);
	ObjetoComFaces janelaIgrejaEsquerdaMeio = ObjetoComFaces({ { 1.99f, 1.2f, -8.4f },{ 1.99f, 2.0f, -8.4f },{ 1.99f, 2.0f, -8.8f },{ 1.99f, 1.2f, -8.8f } }, { { 0, 1, 2, 3 } }, blueGlass);
	ObjetoComFaces janelaIgrejaEsquerdaTras = ObjetoComFaces({ { 1.99f, 1.2f, -9.4f },{ 1.99f, 2.0f, -9.4f },{ 1.99f, 2.0f, -9.8f },{ 1.99f, 1.2f, -9.8f } }, { { 0, 1, 2, 3 } }, blueGlass);
	ObjetoComFaces janelaIgrejaFrente = ObjetoComFaces({ { 3.6f, 1.2f, -7.19f },{ 3.6f, 2.0f, -7.19f },{ 3.2f, 2.0f, -7.19f },{ 3.2f, 1.2f, -7.19f } }, { { 0, 1, 2, 3 } }, blueGlass);
	ObjetoComFaces portaIgreja = ObjetoComFaces({ { 3.6f, 0.0f, -7.19f },{ 3.6f, 0.8f, -7.19f },{ 3.2f, 0.8f, -7.19f },{ 3.2f, 0.0f, -7.19f } }, { { 0, 1, 2, 3 } }, lightBrown);

	//desenhando navio
	ObjetoComFaces ship1 = ObjetoComFaces(multiplyByMatrix(8, ship, multiplyMatrix(translationMatrix(-2.5, 0.0, 2.0), scaleMatrix(1.5, 0.5, 0.5))), boxFaces, darkBlue);
	ObjetoComFaces ship1pole1 = ObjetoComFaces(multiplyByMatrix(8, basicCube, multiplyMatrix(translationMatrix(-1.3, 0.5, 2.23), scaleMatrix(0.04, 0.8, 0.04))), boxFaces, greenBrown);
	ObjetoComFaces ship1pole2 = ObjetoComFaces(multiplyByMatrix(8, basicCube, multiplyMatrix(translationMatrix(-1.5, 0.5, 2.23), scaleMatrix(0.04, 0.8, 0.04))), boxFaces, greenBrown);
	ObjetoComFaces ship1pole3 = ObjetoComFaces(multiplyByMatrix(8, basicCube, multiplyMatrix(translationMatrix(-1.7, 0.5, 2.24), scaleMatrix(0.02, 0.8, 0.02))), boxFaces, greenBrown);
    ObjetoComFaces flag = ObjetoComFaces({ {-2.7f,0.6f,2.25f}, {-1.7f,0.6f,2.25f}, {-1.7f,1.3f,2.25f} }, { {0,1,2}, {0,2,1} }, red);
	ship1.rotateY(45, ship1.centro);
	ship1pole1.rotateY(45, ship1.centro);
	ship1pole2.rotateY(45, ship1.centro);
	ship1pole3.rotateY(45, ship1.centro);
	flag.rotateY(45, ship1.centro);

	ObjetoComFaces montanha1 = ObjetoComFaces(basicTriangularPyramid, triangularPyramidFaces, mountainBlue);
	ObjetoComFaces montanha2 = ObjetoComFaces(montanha1);
	ObjetoComFaces montanha3 = ObjetoComFaces(montanha1);
	ObjetoComFaces montanha4 = ObjetoComFaces(montanha1);
	montanha1.scale(28.0, 9.0, 15.0);
	montanha1.translate(40.0, 0.0, -70.0);
	montanha2.scale(33.0, 10.0, 18.0);
	montanha2.translate(49.0, 0.0, -50.0);
	montanha3.scale(19.0, 11.0, 12.0);
	montanha3.translate(32.0, 0.0, -81.0);
	montanha4.scale(24.0, 10.0, 7.0);
	montanha4.translate(14.0, 0.0, -82.0);

	Cilindro poste1 = Cilindro({ 3.5f, 0.0f, -4.0f }, { 0.0f, 1.0f, 0.0f }, 1.5, 0.1, black);
	Esfera topoPoste1 = Esfera({ 3.5f, 1.65f, -4.0f }, 0.3, white);



	vector<Objeto*> listaTodosObjetos = { &portaIgreja, &tronco3, &topoEsfericoArvore, &a1Tronco, &a1Folhas1, &a1Folhas2, &a1Folhas3, &a1Folhas4, &a2Tronco, &a2Folhas1, &a2Folhas2, &a2Folhas3, &a2Folhas4, &a3Tronco, &a3Folhas1, &a3Folhas2, &a3Folhas3, &a3Folhas4, &montanha1, &montanha2, &montanha3, &montanha4, &ship1, &ship1pole1, &ship1pole2, &ship1pole3, &flag, &topoTorre, &cruzParteVertical, &cruzParteHorizontal, &baseTorre, &baseIgreja, &topoIgreja, &telhado, &janelaTorreFrenteAlto, &janelaTorreEsquerdaAlto, &janelaTorreFrenteBaixo, &janelaTorreEsquerdaBaixo, &janelaIgrejaEsquerdaFrente, &janelaIgrejaEsquerdaMeio, &janelaIgrejaEsquerdaTras, &janelaIgrejaFrente, &tronco1, &cone1, &tronco2, &cone2, &poste1, &topoPoste1 };

	//APLICA��O DE MAIS TRANSFORMA��ES

	//TRANSFORMA��ES DE C�MERA

	vector<float> vUp = Objeto::normalizaVetor(Objeto::diferencaVetores(up, p0_c));

	vector<float> k_c = Objeto::normalizaVetor(Objeto::diferencaVetores(p0_c, at));
	vector<float> i_c = Objeto::normalizaVetor(Objeto::produtoVetorial(vUp, k_c));
	vector<float> j_c = Objeto::produtoVetorial(k_c, i_c);

	vector<vector<float>> M_CW(4, vector<float>(4));
	M_CW[0][0] = i_c[0];
	M_CW[0][1] = j_c[0];
	M_CW[0][2] = k_c[0];
	M_CW[0][3] = p0_c[0];
	M_CW[1][0] = i_c[1];
	M_CW[1][1] = j_c[1];
	M_CW[1][2] = k_c[1];
	M_CW[1][3] = p0_c[1];
	M_CW[2][0] = i_c[2];
	M_CW[2][1] = j_c[2];
	M_CW[2][2] = k_c[2];
	M_CW[2][3] = p0_c[2];
	M_CW[3][3] = 1;


	vector<vector<float>> M_WC(4, vector<float>(4));

	M_WC[0][0] = i_c[0];
	M_WC[0][1] = i_c[1];
	M_WC[0][2] = i_c[2];
	M_WC[1][0] = j_c[0];
	M_WC[1][1] = j_c[1];
	M_WC[1][2] = j_c[2];
	M_WC[2][0] = k_c[0];
	M_WC[2][1] = k_c[1];
	M_WC[2][2] = k_c[2];
	M_WC[0][3] = -(i_c[0] * p0_c[0] + i_c[1] * p0_c[1] + i_c[2] * p0_c[2]);
	M_WC[1][3] = -(j_c[0] * p0_c[0] + j_c[1] * p0_c[1] + j_c[2] * p0_c[2]);
	M_WC[2][3] = -(k_c[0] * p0_c[0] + k_c[1] * p0_c[1] + k_c[2] * p0_c[2]);
	M_WC[3][3] = 1;

	for (int i = 0; i < size(listaTodosObjetos); i++) {
		(*listaTodosObjetos[i]).transform(M_WC);
	}

	vector<float> direcaoLuzD = { -1.0f / sqrt(2.0f), 1.0f / sqrt(2.0f), 0.0 };
	vector<float> posicaoLuzP = { 3.5f, 1.65f, -4.0f };

	vector<float> novaOrigem = multiplyVectorByMatrix({ 0.0, 0.0, 0.0 }, M_WC);

	direcaoLuzD = Objeto::diferencaVetores(multiplyVectorByMatrix(direcaoLuzD, M_WC), novaOrigem);
	posicaoLuzP = multiplyVectorByMatrix(posicaoLuzP, M_WC);

	//DEFINI��O DOS CLUSTERS

	Cluster navio = Cluster({ &ship1, &ship1pole1, &ship1pole2, &ship1pole3, &flag });
	Cluster topoTorreCluster = Cluster({ &topoTorre, &cruzParteVertical, &cruzParteHorizontal });
	Cluster igreja = Cluster({ &baseTorre, &baseIgreja, &topoIgreja, &telhado, &janelaTorreFrenteAlto, &janelaTorreEsquerdaAlto, &janelaTorreFrenteBaixo, &janelaTorreEsquerdaBaixo, &janelaIgrejaEsquerdaFrente, &janelaIgrejaEsquerdaMeio, &janelaIgrejaEsquerdaTras, &janelaIgrejaFrente, &portaIgreja });
	Cluster arvore1 = Cluster({ &a1Tronco, &a1Folhas1, &a1Folhas2, &a1Folhas3, &a1Folhas4 });
	Cluster arvore2 = Cluster({ &a2Tronco, &a2Folhas1, &a2Folhas2, &a2Folhas3, &a2Folhas4 });
	Cluster arvore3 = Cluster({ &a3Tronco, &a3Folhas1, &a3Folhas2, &a3Folhas3, &a3Folhas4 });

	vector<Objeto*> objetos = { &igreja, &topoTorreCluster, &navio, &montanha1, &montanha2, &montanha3, &montanha4, &arvore1, &arvore2, &arvore3, &tronco1, &cone1, &tronco2, &cone2, &tronco3, &topoEsfericoArvore, &poste1, &topoPoste1 };
	//vector<Objeto*> objetos = { /*&baseIgreja, &navio, &arvore3*/ /*&topoCilindroArvore,*/&topoCilindroArvore, &topoConeArvore, &esfera1, &igreja ,/*&arvore1, &arvore2,*/ &arvore3, &montanha1/*&arvore4, &arvore5, &topoTorreCluster, &navio, &montanha1, &montanha2, &montanha3, &montanha4*/ };

	//lancarRaiosOrtho(objetos, M_CW, direcaoLuzD, posicaoLuzP, -10.0, 10.0, -10.0, 10.0, 0.0, 0.0, 0.0);
	lancarRaios(objetos, M_CW, direcaoLuzD, posicaoLuzP, -0.5, 0.5, -0.5, 0.5, 0.0, 0.0, 0.0, -0.5);

}

int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(Width, Height);
	glutCreateWindow("Projeto CG");

	glutDisplayFunc(display);
	//glutSpecialFunc(specialKeys);
	glEnable(GL_DEPTH_TEST);

	glutMainLoop();
	return 0;

}