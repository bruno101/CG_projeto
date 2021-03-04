#include <GL/glut.h>
#include <iostream>
#include <new>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include "ObjetoComFaces.h"
using namespace std;

//FUNÇÕES COM MATRIZES E VETORES


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

vector<vector<float>> rotationMatrix(float ax, float ay, float az, vector<float> pivot = { 0,0,0 }) {
	vector<vector<float>> M = multiplyMatrix(multiplyMatrix(rotationMatrixX(ax, pivot), rotationMatrixY(ay, pivot)), rotationMatrixZ(az, pivot));
	return M;
}

//FUNÇÕES PARA PINTAR POLÍGONOS


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


//FORMAS BÁSICAS


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



//vector<vector<float>> triangularPyramid;


/*
float triangularPyramid[4][3] = {
{-0.3,0.0,0.0},
{0.0,0.0,-0.4},
{0.3,0.0,0.0},
{0.0,1.0,-0.2},
};
*/


/*
void triangle(int a, int b, int c) {
glBegin(GL_TRIANGLES);
glColor3f(0.004, 0.475, 0.435);
glVertex3fv(triangularPyramid[a].data());
glVertex3fv(triangularPyramid[b].data());
glVertex3fv(triangularPyramid[c].data());
glEnd();
}
void colorPyramid() {
triangle(0, 2, 3);
triangle(3, 2, 1);
triangle(3, 1, 0);
triangle(1, 2, 0);
}
*/

/*
float boxVer[8][3] =
{
{ -0.1,-1.0,0.1 },
{ -0.1,1.0,0.1 },
{ 0.1,1.0,0.1 },
{ 0.1,-1.0,0.1 },
{ -0.1,-1.0,-0.1 },
{ -0.1,1.0,-0.1 },
{ 0.1,1.0,-0.1 },
{ 0.1,-1.0,-0.1 },
};
GLfloat boxColor[8][3] =
{
{ 119.0 / 256.0, 69.0 / 256.0, 19.0 / 256.0 },
{ 119.0 / 256.0, 69.0 / 256.0, 19.0 / 256.0 },
{ 119.0 / 256.0, 69.0 / 256.0, 19.0 / 256.0 },
{ 119.0 / 256.0, 69.0 / 256.0, 19.0 / 256.0 },
{ 119.0 / 256.0, 69.0 / 256.0, 19.0 / 256.0 },
{ 119.0 / 256.0, 69.0 / 256.0, 19.0 / 256.0 },
{ 119.0 / 256.0, 69.0 / 256.0, 19.0 / 256.0 },
{ 119.0 / 256.0, 69.0 / 256.0, 19.0 / 256.0 },
};
*/

/*
void quad(int a, int b, int c, int d)
{
glBegin(GL_QUADS);
glColor3fv(boxColor[a]);
glVertex3fv(boxVer[a]);
glColor3fv(boxColor[b]);
glVertex3fv(boxVer[b]);
glColor3fv(boxColor[c]);
glVertex3fv(boxVer[c]);
glColor3fv(boxColor[d]);
glVertex3fv(boxVer[d]);
glEnd();
}
void colorBox()
{
quad(0, 3, 2, 1);
quad(2, 3, 7, 6);
quad(0, 4, 7, 3);
quad(1, 2, 6, 5);
quad(4, 5, 6, 7);
quad(0, 1, 5, 4);
}
*/

// FUNÇÕES PRINCIPAIS

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

void display()
{
	glClearColor(0.529, 0.808, 0.922, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/*int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	gluPerspective(60, w / h, 0.1, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt
	(
		0, 0.3, 11,
		0, 0, 0,
		0, 1, 0
	);*/

	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	glOrtho(-8, 8, 0, 16, -10, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt
	(
		0, 2, 50,
		0, 0, 20,
		0, 1, 0
	);
	glViewport(0, 0, w, h);

	glRotatef(rotate_x, 1.0, 0.0, 0.0);
	glRotatef(rotate_y, 0.0, 1.0, 0.0);
	//colorBox();
	//colorPyramid();
	//colorSphere();

	glBegin(GL_QUADS);
	glColor3f(0.0, 154.0 / 256.0, 23.0 / 256.0);
	glVertex3f(-100.0, 0.0, 0.0);
	glVertex3f(100.0, 0.0, 0.0);
	glVertex3f(100.0, 0.0, -100.0);
	glVertex3f(-100.0, 0.0, -100.0);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.0, 0.412, 0.58);
	glVertex3f(-100.0, 0.0, 100.0);
	glVertex3f(100.0, 0.0, 100.0);
	glVertex3f(100.0, 0.0, 0.0);
	glVertex3f(-100.0, 0.0, 0.0);
	glEnd();
	
	vector<vector<float>> topoArvore = multiplyByMatrix(4, basicTriangularPyramid, multiplyMatrix(translationMatrix(-0.25, 1.0, -0.25), scaleMatrix(0.6, 0.8, 0.6)));
	/*vector<vector<float>> p1 = multiplyByMatrix(4, topoArvore, translationMatrix(-3.0, 0.0, -3.0));
	vector<vector<float>> p2 = multiplyByMatrix(4, topoArvore, translationMatrix(-1.0, 0.0, -3.0));
	vector<vector<float>> p3 = multiplyByMatrix(4, topoArvore, translationMatrix(1.0, 0.0, -3.0));
	vector<vector<float>> p4 = multiplyByMatrix(4, topoArvore, translationMatrix(-2.0, 0.0, -5.0));
	vector<vector<float>> p5 = multiplyByMatrix(4, topoArvore, translationMatrix(0.0, 0.0, -5.0));*/
	GLfloat pineGreen[3] = { 0.004, 0.475, 0.318 };
	/*colorTriangularPyramid(p1, pineGreen);
	colorTriangularPyramid(p2, pineGreen);
	colorTriangularPyramid(p3, pineGreen);
	colorTriangularPyramid(p4, pineGreen);
	colorTriangularPyramid(p5, pineGreen);*/

	vector<vector<float>> troncoArvore = multiplyByMatrix(8, basicCube, scaleMatrix(0.1, 1.0, 0.1));
	/*vector<vector<float>> b1 = multiplyByMatrix(8, troncoArvore, translationMatrix(-3.0, 0.0,-3.0));
	vector<vector<float>> b2 = multiplyByMatrix(8, troncoArvore, translationMatrix(-1.0, 0.0, -3.0));
	vector<vector<float>> b3 = multiplyByMatrix(8, troncoArvore, translationMatrix(1.0, 0.0, -3.0));
	vector<vector<float>> b4 = multiplyByMatrix(8, troncoArvore, translationMatrix(-2.0, 0.0, -5.0));
	vector<vector<float>> b5 = multiplyByMatrix(8, troncoArvore, translationMatrix(0.0, 0.0, -5.0));*/
	GLfloat treeBrown[3] = { 119.0 / 256.0, 69.0 / 256.0, 19.0 / 256.0 };
	/*colorBox(b1, treeBrown);
	colorBox(b2, treeBrown);
	colorBox(b3, treeBrown);
	colorBox(b4, treeBrown);
	colorBox(b5, treeBrown);*/

	/*void colorBox(vector<vector<float>> box, GLfloat color[3]) {
		quad(box, 4, 0, 1, 5, color);
		quad(box, 5, 1, 2, 6, color);
		quad(box, 6, 2, 3, 7, color);
		quad(box, 7, 3, 0, 4, color);
		quad(box, 5, 6, 7, 4, color);
		quad(box, 1, 0, 3, 2, color);
	}*/

	/*void colorTriangularPyramid(vector<vector<float>> triangularPyramid, GLfloat color[3]) {
		triangle(triangularPyramid, 1, 2, 3, color);
		triangle(triangularPyramid, 2, 0, 3, color);
		triangle(triangularPyramid, 0, 1, 3, color);
		triangle(triangularPyramid, 0, 2, 1, color);
	}*/

	Objeto& a1Tronco = ObjetoComFaces(multiplyByMatrix(8, troncoArvore, translationMatrix(-6.0, 0.0, -7.0)), { {4,0,1,5}, {5,1,2,6}, {6,2,3,7}, {7,3,0,4}, {5,6,7,4}, {1,0,3,2} }, treeBrown);
	a1Tronco.paint();
	Objeto& a1Folhas1 = ObjetoComFaces(multiplyByMatrix(4, topoArvore, translationMatrix(-6.0, 0.0, -7.0)), { {1,2,3}, {2,0,3}, {0,1,3}, {0,2,1} }, pineGreen);
	a1Folhas1.paint();
	Objeto& a1Folhas2 = ObjetoComFaces(multiplyByMatrix(4, topoArvore, translationMatrix(-6.0, 0.2, -7.0)), { { 1,2,3 },{ 2,0,3 },{ 0,1,3 },{ 0,2,1 } }, pineGreen);
	a1Folhas2.paint();
	Objeto& a1Folhas3 = ObjetoComFaces(multiplyByMatrix(4, topoArvore, translationMatrix(-6.0, 0.4, -7.0)), { { 1,2,3 },{ 2,0,3 },{ 0,1,3 },{ 0,2,1 } }, pineGreen);
	a1Folhas3.paint();
	Objeto& a1Folhas4 = ObjetoComFaces(multiplyByMatrix(4, topoArvore, translationMatrix(-6.0, 0.6, -7.0)), { { 1,2,3 },{ 2,0,3 },{ 0,1,3 },{ 0,2,1 } }, pineGreen);
	a1Folhas4.paint();

	//Vector<Objeto> objetos = { a1Tronco, a1Folhas1, a1Folhas2, a1Folhas3, a1Folhas4 };



	//vector<vector<float>> a1Tronco = multiplyByMatrix(8, troncoArvore, translationMatrix(-6.0, 0.0, -7.0));
	//vector<vector<float>> a1Folhas1 = multiplyByMatrix(4, topoArvore, translationMatrix(-6.0, 0.0, -7.0));
	//vector<vector<float>> a1Folhas2 = multiplyByMatrix(4, topoArvore, translationMatrix(-6.0, 0.2, -7.0));
	//vector<vector<float>> a1Folhas3 = multiplyByMatrix(4, topoArvore, translationMatrix(-6.0, 0.4, -7.0));
	//vector<vector<float>> a1Folhas4 = multiplyByMatrix(4, topoArvore, translationMatrix(-6.0, 0.6, -7.0));
	//colorBox(a1Tronco, treeBrown);
	//colorTriangularPyramid(a1Folhas1, pineGreen);
	//colorTriangularPyramid(a1Folhas2, pineGreen);
	//colorTriangularPyramid(a1Folhas3, pineGreen);
	//colorTriangularPyramid(a1Folhas4, pineGreen);

	vector<vector<float>> a2Tronco = multiplyByMatrix(8, troncoArvore, translationMatrix(-5.0, 0.0, -7.0));
	vector<vector<float>> a2Folhas1 = multiplyByMatrix(4, topoArvore, translationMatrix(-5.0, 0.0, -7.0));
	vector<vector<float>> a2Folhas2 = multiplyByMatrix(4, topoArvore, translationMatrix(-5.0, 0.2, -7.0));
	vector<vector<float>> a2Folhas3 = multiplyByMatrix(4, topoArvore, translationMatrix(-5.0, 0.4, -7.0));
	vector<vector<float>> a2Folhas4 = multiplyByMatrix(4, topoArvore, translationMatrix(-5.0, 0.6, -7.0));
	colorBox(a2Tronco, treeBrown);
	colorTriangularPyramid(a2Folhas1, pineGreen);
	colorTriangularPyramid(a2Folhas2, pineGreen);
	colorTriangularPyramid(a2Folhas3, pineGreen);
	colorTriangularPyramid(a2Folhas4, pineGreen);

	vector<vector<float>> a3Tronco = multiplyByMatrix(8, troncoArvore, translationMatrix(-4.0, 0.0, -7.0));
	vector<vector<float>> a3Folhas1 = multiplyByMatrix(4, topoArvore, translationMatrix(-4.0, 0.0, -7.0));
	vector<vector<float>> a3Folhas2 = multiplyByMatrix(4, topoArvore, translationMatrix(-4.0, 0.2, -7.0));
	vector<vector<float>> a3Folhas3 = multiplyByMatrix(4, topoArvore, translationMatrix(-4.0, 0.4, -7.0));
	vector<vector<float>> a3Folhas4 = multiplyByMatrix(4, topoArvore, translationMatrix(-4.0, 0.6, -7.0));
	colorBox(a3Tronco, treeBrown);
	colorTriangularPyramid(a3Folhas1, pineGreen);
	colorTriangularPyramid(a3Folhas2, pineGreen);
	colorTriangularPyramid(a3Folhas3, pineGreen);
	colorTriangularPyramid(a3Folhas4, pineGreen);

	vector<vector<float>> a4Tronco = multiplyByMatrix(8, troncoArvore, translationMatrix(-3.0, 0.0, -7.0));
	vector<vector<float>> a4Folhas1 = multiplyByMatrix(4, topoArvore, translationMatrix(-3.0, 0.0, -7.0));
	vector<vector<float>> a4Folhas2 = multiplyByMatrix(4, topoArvore, translationMatrix(-3.0, 0.2, -7.0));
	vector<vector<float>> a4Folhas3 = multiplyByMatrix(4, topoArvore, translationMatrix(-3.0, 0.4, -7.0));
	vector<vector<float>> a4Folhas4 = multiplyByMatrix(4, topoArvore, translationMatrix(-3.0, 0.6, -7.0));
	colorBox(a4Tronco, treeBrown);
	colorTriangularPyramid(a4Folhas1, pineGreen);
	colorTriangularPyramid(a4Folhas2, pineGreen);
	colorTriangularPyramid(a4Folhas3, pineGreen);
	colorTriangularPyramid(a4Folhas4, pineGreen);

	vector<vector<float>> a5Tronco = multiplyByMatrix(8, troncoArvore, translationMatrix(-2.0, 0.0, -7.0));
	vector<vector<float>> a5Folhas1 = multiplyByMatrix(4, topoArvore, translationMatrix(-2.0, 0.0, -7.0));
	vector<vector<float>> a5Folhas2 = multiplyByMatrix(4, topoArvore, translationMatrix(-2.0, 0.2, -7.0));
	vector<vector<float>> a5Folhas3 = multiplyByMatrix(4, topoArvore, translationMatrix(-2.0, 0.4, -7.0));
	vector<vector<float>> a5Folhas4 = multiplyByMatrix(4, topoArvore, translationMatrix(-2.0, 0.6, -7.0));
	colorBox(a5Tronco, treeBrown);
	colorTriangularPyramid(a5Folhas1, pineGreen);
	colorTriangularPyramid(a5Folhas2, pineGreen);
	colorTriangularPyramid(a5Folhas3, pineGreen);
	colorTriangularPyramid(a5Folhas4, pineGreen);


	vector<vector<float>> montanha1 = multiplyByMatrix(4, basicTriangularPyramid, multiplyMatrix(translationMatrix(-15.0, 0.0, -80.0), scaleMatrix(20.0, 10.0, 5.0)));
	GLfloat mountainBlue[3] = { 0.6314, 0.2392, 0.1765 };
	colorTriangularPyramid(montanha1, mountainBlue);
	vector<vector<float>> montanha2 = multiplyByMatrix(4, basicTriangularPyramid, multiplyMatrix(translationMatrix(0.0, 0.0, -85.0), scaleMatrix(19.0, 12.0, 7.0)));
	colorTriangularPyramid(montanha2, mountainBlue);
	vector<vector<float>> montanha3 = multiplyByMatrix(4, basicTriangularPyramid, multiplyMatrix(translationMatrix(14.0, 0.0, -82.0), scaleMatrix(24.0, 10.0, 7.0)));
	colorTriangularPyramid(montanha3, mountainBlue);
	vector<vector<float>> montanha4 = multiplyByMatrix(4, basicTriangularPyramid, multiplyMatrix(translationMatrix(32.0, 0.0, -81.0), scaleMatrix(19.0, 11.0, 12.0)));
	colorTriangularPyramid(montanha4, mountainBlue);
	vector<vector<float>> montanha5 = multiplyByMatrix(4, basicTriangularPyramid, multiplyMatrix(translationMatrix(40.0, 0.0, -70.0), scaleMatrix(28.0, 9.0, 15.0)));
	colorTriangularPyramid(montanha5, mountainBlue);
	vector<vector<float>> montanha6 = multiplyByMatrix(4, basicTriangularPyramid, multiplyMatrix(translationMatrix(49.0, 0.0, -50.0), scaleMatrix(33.0, 10.0, 18.0)));
	colorTriangularPyramid(montanha6, mountainBlue);
	vector<vector<float>> montanha7 = multiplyByMatrix(4, basicTriangularPyramid, multiplyMatrix(translationMatrix(67.0, 0.0, -35.0), scaleMatrix(29.0, 10.0, 17.0)));
	colorTriangularPyramid(montanha7, mountainBlue);
	vector<vector<float>> montanha8 = multiplyByMatrix(4, basicTriangularPyramid, multiplyMatrix(translationMatrix(80.0, 0.0, -20.0), scaleMatrix(24.0, 10.0, 20.0)));
	colorTriangularPyramid(montanha8, mountainBlue);
	vector<vector<float>> montanha9 = multiplyByMatrix(4, basicTriangularPyramid, multiplyMatrix(translationMatrix(-27.0, 0.0, -70.0), scaleMatrix(17.0, 11.0, 10.0)));
	colorTriangularPyramid(montanha9, mountainBlue);
	vector<vector<float>> montanha10 = multiplyByMatrix(4, basicTriangularPyramid, multiplyMatrix(translationMatrix(-40.0, 0.0, -65.0), scaleMatrix(24.0, 9.0, 8.0)));
	colorTriangularPyramid(montanha10, mountainBlue);
	vector<vector<float>> montanha11 = multiplyByMatrix(4, basicTriangularPyramid, multiplyMatrix(translationMatrix(-53.0, 0.0, -55.0), scaleMatrix(22.0, 8.0, 14.0)));
	colorTriangularPyramid(montanha11, mountainBlue);
	vector<vector<float>> montanha12 = multiplyByMatrix(4, basicTriangularPyramid, multiplyMatrix(translationMatrix(-67.0, 0.0, -50.0), scaleMatrix(21.0, 7.0, 13.0)));
	colorTriangularPyramid(montanha12, mountainBlue);
	vector<vector<float>> montanha13 = multiplyByMatrix(4, basicTriangularPyramid, multiplyMatrix(translationMatrix(-80.0, 0.0, -42.0), scaleMatrix(23.0, 8.0, 15.0)));
	colorTriangularPyramid(montanha13, mountainBlue);
	vector<vector<float>> montanha14 = multiplyByMatrix(4, basicTriangularPyramid, multiplyMatrix(translationMatrix(-95.0, 0.0, -35.0), scaleMatrix(23.0, 8.0, 18.0)));
	colorTriangularPyramid(montanha14, mountainBlue);

	vector<vector<float>> ship1 = multiplyByMatrix(8, ship, multiplyMatrix(translationMatrix(2.5, 0.0, 2.0), scaleMatrix(1.5, 0.5, 0.5)));
	GLfloat darkBlue[3] = { 0.0, 0.0, 128.0 / 256.0 };
	colorBox(ship1, darkBlue);
	vector<vector<float>> ship1pole1 = multiplyByMatrix(8, basicCube, multiplyMatrix(translationMatrix(3.7, 0.5, 2.23), scaleMatrix(0.04, 0.8, 0.04)));
	GLfloat greenBrown[3] = { 74.0 / 256.0, 67.0 / 256.0, 0.0 };
	colorBox(ship1pole1, greenBrown);
	vector<vector<float>> ship1pole2 = multiplyByMatrix(8, basicCube, multiplyMatrix(translationMatrix(3.5, 0.5, 2.23), scaleMatrix(0.04, 0.8, 0.04)));
	colorBox(ship1pole2, greenBrown);
	vector<vector<float>> ship1pole3 = multiplyByMatrix(8, basicCube, multiplyMatrix(translationMatrix(3.3, 0.5, 2.24), scaleMatrix(0.02, 0.8, 0.02)));
	colorBox(ship1pole3, greenBrown);
	glBegin(GL_TRIANGLES);
	GLfloat red[3] = { 0.659, 0.0, 0.0 };
	glColor3fv(red);
	glVertex3f(2.3, 0.6, 2.25);
	glVertex3f(3.3, 0.6, 2.25);
	glVertex3f(3.3, 1.3, 2.25);
	glEnd();

	vector<vector<float>> baseTorre = multiplyByMatrix(8, basicCube, multiplyMatrix(translationMatrix(2.0, 0.0, -7.2), scaleMatrix(0.8, 5.0, 0.8)));
	GLfloat lightYellow[3] = { 255.0 / 256.0, 255.0 / 256.0, 153.0 / 256.0 };
	colorBox(baseTorre, lightYellow);

	GLfloat white[3] = { 256.0, 256.0, 256.0 };

	vector<vector<float>> topoTorre = multiplyByMatrix(5, basicSquarePyramid, multiplyMatrix(translationMatrix(2.05, 5.0, -7.15), scaleMatrix(0.7, 1.7, 0.7)));
	GLfloat crimson[3] = { 220.0 / 256.0, 20.0 / 256.0, 60.0 / 256.0 };
	colorSquarePyramid(topoTorre, crimson);

	vector<vector<float>> cruzParteVertical = multiplyByMatrix(8, basicCube, multiplyMatrix(translationMatrix(2.385, 6.7, -6.815), scaleMatrix(0.03, 1.0, 0.03)));
	GLfloat brown[3] = { 210.0 / 256.0, 105.0 / 256.0, 30.0 / 256.0 };
	colorBox(cruzParteVertical, brown);

	vector<vector<float>> cruzParteHorizontal = multiplyByMatrix(8, basicCube, multiplyMatrix(translationMatrix(2.15, 7.3, -6.95), scaleMatrix(0.5, 0.03, 0.03)));
	colorBox(cruzParteHorizontal, brown);

	vector<vector<float>> baseIgreja = multiplyByMatrix(8, basicCube, multiplyMatrix(translationMatrix(2.0, 0.0, -10.2), scaleMatrix(2.0, 2.5, 3.0)));
	colorBox(baseIgreja, lightYellow);

	vector<vector<float>> topoIgreja = multiplyByMatrix(6, roof, multiplyMatrix(translationMatrix(2.0, 2.5, -10.2), scaleMatrix(2.0, 2.5, 3.0)));;
	colorRoof(topoIgreja, lightYellow, crimson);

	//desenhando porta da igreja

	glBegin(GL_QUADS);
	glColor3fv(brown);
	glVertex3f(3.0, 0.0, -7.19);
	glVertex3f(3.5, 0.0, -7.19);
	glVertex3f(3.5, 0.6, -7.19);
	glVertex3f(3.0, 0.6, -7.19);
	glEnd();

	float radius = 0.25;
	float pi = 3.1415926535;
	glBegin(GL_TRIANGLE_FAN);
	glColor3fv(brown);
	for (float i = 0; i <= pi * 2; i += 0.001) {
		glVertex3f(sin(i)*radius + 3.25, cos(i)*radius + 0.6, -7.19);
	}
	glEnd();

	GLfloat blue[3] = { 135.0 / 256.0,206.0 / 256.0,255.0 / 256.0 };

	/*radius = 0.25;
	glBegin(GL_TRIANGLE_FAN);
	glColor3fv(darkBlue);
	for (float i = 0; i <= pi * 2; i += 0.001) {
		glVertex3f(sin(i)*radius + 3.25, cos(i)*radius*1.2 + 2.2, -7.19);
	}
	glEnd();

	GLfloat green[3] = { 40.0 / 256.0, 180.0 / 256.0, 99.0 / 256.0 };

	radius = 0.2;
	glBegin(GL_TRIANGLE_FAN);
	glColor3fv(green);
	for (float i = 0; i <= pi * 2; i += 0.001) {
		glVertex3f(sin(i)*radius + 3.25, cos(i)*radius*1.2 + 2.2, -7.18);
	}
	glEnd();

	glBegin(GL_QUADS);
	glColor3fv(white);
	glVertex3f(3.25, 2.2, -7.17);
	glVertex3f(3.265, 2.2, -7.17);
	glVertex3f(3.265, 2.3, -7.17);
	glVertex3f(3.25, 2.3, -7.17);
	glEnd();

	glBegin(GL_QUADS);
	glColor3fv(white);
	glVertex3f(3.25, 2.215, -7.17);
	glVertex3f(3.4, 2.215, -7.17);
	glVertex3f(3.4, 2.185, -7.17);
	glVertex3f(3.25, 2.185, -7.17);
	glEnd();*/

	//desenhando parte da frente do telhado

	glBegin(GL_QUADS);
	glColor3fv(crimson);
	glVertex3f(4.0, 2.5, -7.19);
	glVertex3f(3.0, 5.0, -7.19);
	glVertex3f(2.9, 5.0, -7.19);
	glVertex3f(3.9, 2.5, -7.19);
	glEnd();

	glBegin(GL_QUADS);
	glColor3fv(crimson);
	glVertex3f(3.0, 5.0, -7.19);
	glVertex3f(2.5, 3.75, -7.19);
	glVertex3f(2.4, 3.75, -7.19);
	glVertex3f(2.9, 5.0, -7.19);
	glEnd();

	//desenhando janelas

	glBegin(GL_QUADS);
	glColor3fv(blue);
	glVertex3f(2.6, 3.6, -6.39);
	glVertex3f(2.6, 4.4, -6.39);
	glVertex3f(2.2, 4.4, -6.39);
	glVertex3f(2.2, 3.6, -6.39);
	glEnd();

	glBegin(GL_QUADS);
	glColor3fv(blue);
	glVertex3f(1.99, 3.6, -6.6);
	glVertex3f(1.99, 4.4, -6.6);
	glVertex3f(1.99, 4.4, -7.0);
	glVertex3f(1.99, 3.6, -7.0);
	glEnd();

	glBegin(GL_QUADS);
	glColor3fv(blue);
	glVertex3f(2.81, 3.6, -6.6);
	glVertex3f(2.81, 4.4, -6.6);
	glVertex3f(2.81, 4.4, -7.0);
	glVertex3f(2.81, 3.6, -7.0);
	glEnd();

	glBegin(GL_QUADS);
	glColor3fv(blue);
	glVertex3f(2.6, 1.2, -6.39);
	glVertex3f(2.6, 2.0, -6.39);
	glVertex3f(2.2, 2.0, -6.39);
	glVertex3f(2.2, 1.2, -6.39);
	glEnd();

	glBegin(GL_QUADS);
	glColor3fv(blue);
	glVertex3f(1.99, 1.2, -6.6);
	glVertex3f(1.99, 2.0, -6.6);
	glVertex3f(1.99, 2.0, -7.0);
	glVertex3f(1.99, 1.2, -7.0);
	glEnd();

	glBegin(GL_QUADS);
	glColor3fv(blue);
	glVertex3f(2.81, 1.2, -6.6);
	glVertex3f(2.81, 2.0, -6.6);
	glVertex3f(2.81, 2.0, -7.0);
	glVertex3f(2.81, 1.2, -7.0);
	glEnd();

	glBegin(GL_QUADS);
	glColor3fv(blue);
	glVertex3f(1.99, 1.2, -7.4);
	glVertex3f(1.99, 2.0, -7.4);
	glVertex3f(1.99, 2.0, -7.8);
	glVertex3f(1.99, 1.2, -7.8);
	glEnd();

	glBegin(GL_QUADS);
	glColor3fv(blue);
	glVertex3f(1.99, 1.2, -8.4);
	glVertex3f(1.99, 2.0, -8.4);
	glVertex3f(1.99, 2.0, -8.8);
	glVertex3f(1.99, 1.2, -8.8);
	glEnd();

	glBegin(GL_QUADS);
	glColor3fv(blue);
	glVertex3f(1.99, 1.2, -9.4);
	glVertex3f(1.99, 2.0, -9.4);
	glVertex3f(1.99, 2.0, -9.8);
	glVertex3f(1.99, 1.2, -9.8);
	glEnd();

	glBegin(GL_QUADS);
	glColor3fv(blue);
	glVertex3f(4.01, 1.2, -7.4);
	glVertex3f(4.01, 2.0, -7.4);
	glVertex3f(4.01, 2.0, -7.8);
	glVertex3f(4.01, 1.2, -7.8);
	glEnd();

	glBegin(GL_QUADS);
	glColor3fv(blue);
	glVertex3f(4.01, 1.2, -8.4);
	glVertex3f(4.01, 2.0, -8.4);
	glVertex3f(4.01, 2.0, -8.8);
	glVertex3f(4.01, 1.2, -8.8);
	glEnd();

	glBegin(GL_QUADS);
	glColor3fv(blue);
	glVertex3f(4.01, 1.2, -9.4);
	glVertex3f(4.01, 2.0, -9.4);
	glVertex3f(4.01, 2.0, -9.8);
	glVertex3f(4.01, 1.2, -9.8);
	glEnd();

	glBegin(GL_QUADS);
	glColor3fv(blue);
	glVertex3f(3.6, 1.2, -7.19);
	glVertex3f(3.6, 2.0, -7.19);
	glVertex3f(3.2, 2.0, -7.19);
	glVertex3f(3.2, 1.2, -7.19);
	glEnd();

	glutSwapBuffers();
}

int main(int argc, char **argv)
{


	/*
	vector<vector<float>> M1 = { { 0.0,1.0,2.0,0.0 },{ 0.0,1.0,2.0,0.0 },{ 0.0,1.0,2.0,0.0 },{ 0.0,0.0,0.0,1.0 }, };
	vector<vector<float>> M2 = { { 0.0,1.0,2.0,0.0 },{ 0.0,1.0,2.0,0.0 },{ 0.0,1.0,2.0,0.0 },{ 0.0,0.0,0.0,1.0 }, };
	vector<vector<float>> M3 = multiplyMatrix(M1, M2);
	std::cout << M1[0][0] << M3[0][0] << M3[0][1] << M3[1][1];
	vector<float> V1 = { 1.0,1.0,1.0 };
	vector<float> V2 = multiplyVectorByMatrix(V1, M1);
	cout << "\n" << V2[0] << V2[1] << V2[2];
	triangularPyramid = multiplyByMatrix(4, basicTriangularPyramid, M1);
	cout << "\n" << triangularPyramid[0][0];
	*/
	//triangularPyramid = basicTriangularPyramid;
	//triangularPyramid = multiplyByMatrix(4, basicTriangularPyramid, translationMatrix(0,0,0));


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(640, 480);
	glutCreateWindow("GLUT");
	glutDisplayFunc(display);
	glutSpecialFunc(specialKeys);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
	return 0;

}