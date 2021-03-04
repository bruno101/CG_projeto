#include "ObjetoComFaces.h"
#include <stdlib.h>
#include <vector>
#include <tuple>
#include <GL/glut.h>


ObjetoComFaces::ObjetoComFaces(vector<vector<float>> vertices, vector<vector<int>> faces, GLfloat color[3])
{
	this->vertices = vertices;
	this->faces = faces;
	copy(color, color+3, this->color);
}

vector<float> ObjetoComFaces::getNormalFace(int numFace) {

	vector<float> p0 = this->vertices[this->faces[numFace][0]];
	vector<float> p1 = this->vertices[this->faces[numFace][1]];
	vector<float> p2 = this->vertices[this->faces[numFace][2]];

	vector<float> v0 = this->diferencaVetores(p1, p0);
	vector<float> v1 = this->diferencaVetores(p2, p0);

	vector<float> vPerpendicular = this->produtoVetorial(v0, v1);

	return this->multiplicaVetorPorEscalar(vPerpendicular, 1/this->normaVetor(vPerpendicular));
}

tuple<bool, float> ObjetoComFaces::hasIntersectionFace(float p0x, float p0y, float p0z, float dx, float dy, float dz, int numFace) {

	float intersecta;
	float t = 100000;

	vector<float> n = this->getNormalFace(numFace);
	vector<float> v = this->diferencaVetores( { p0x, p0y, p0z }, this->vertices[this->faces[numFace][0]] );

	float t0;

	//temos A+Bx=0

	float A = v[0]*n[0]+v[1]*n[1]+v[2]*n[2];
	float B = dx*n[0]+dy*n[1]+dz*n[2];

	if (B == 0) {
		if (A == 0) {
			//INFINITAS SOLUCOES - CHECAR SE UMA DELAS É VÁLIDA E ENCONTRAR A MENOR
		}
		else {
			intersecta = false;
		}
	}
	else {
		t0 = -A / B;
		//CHECAR SE PONTO GERADO POR T0 ESTÁ DENTRO DO LIMITE DA FACE
	}


	return make_tuple(intersecta, t);

}

tuple<bool, float> ObjetoComFaces::hasIntersection(float p0x, float p0y, float p0z, float dx, float dy, float dz)
{

	bool intersecta;
	float t = 100000;

	bool x;
	float y;

	for (int i = 0; i < size(this->faces); i++) {
		tie(x, y) = this->hasIntersectionFace(p0x, p0y, p0z, dx, dy, dz, i);
		if (x == true) {
			intersecta == true;
			if (x < t) {
				t = x;
			}
		}
	}

	return make_tuple(intersecta, t);

}

void ObjetoComFaces::paint() {

	for (int i = 0; i < size(this->faces); i++) {

		if (size(this->faces[i]) == 3) {

			glBegin(GL_TRIANGLES);
			glColor3fv(this->color);
			glVertex3fv(this->vertices[this->faces[i][0]].data());
			glVertex3fv(this->vertices[this->faces[i][1]].data());
			glVertex3fv(this->vertices[this->faces[i][2]].data());
			glEnd();

		}

		if (size(this->faces[i]) == 4) {

			glBegin(GL_QUADS);
			glColor3fv(this->color);
			glVertex3fv(this->vertices[this->faces[i][0]].data());
			glVertex3fv(this->vertices[this->faces[i][1]].data());
			glVertex3fv(this->vertices[this->faces[i][2]].data());
			glVertex3fv(this->vertices[this->faces[i][3]].data());
			glEnd();

		}

	}

}