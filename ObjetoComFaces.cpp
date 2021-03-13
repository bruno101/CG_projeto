#include "ObjetoComFaces.h"
#include <stdlib.h>
#include <vector>
#include <tuple>
#include <GL/glut.h>
#include <iostream>


ObjetoComFaces::ObjetoComFaces(vector<vector<float>> vertices, vector<vector<int>> faces, GLfloat color[3])
{
	this->vertices = vertices;
	this->faces = faces;

	vector<float> somaVetoresVertices = { 0.0, 0.0, 0.0 };
	for (int v = 0; v < size(vertices); v++) {
		somaVetoresVertices = somaVetores(somaVetoresVertices, this->vertices[v]);
	}
	this->centro = multiplicaVetorPorEscalar(somaVetoresVertices, 1.0/size(vertices));

	float raio = 0.0;
	float w;
	for (int v = 0; v < size(vertices); v++) {
		w = normaVetor(diferencaVetores(this->vertices[v], this->centro));
		if (w > raio) {
			raio = w;
		}
	}
	this->raio = raio;

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

bool ObjetoComFaces::podeTerIntersecao(float p0x, float p0y, float p0z, float dx, float dy, float dz) {
	vector<float> v = { p0x-this->centro[0], p0y-this->centro[1], p0z-this->centro[2] };
	float A = v[0]*v[0] + v[1]*v[1] + v[2]*v[2] - this->raio * this->raio;
	float B = 2*v[0]*dx + 2*v[1]*dy + 2*v[2]*dz;
	float C = dx*dx + dy*dy + dz*dz;
	float delta = B*B - 4*A*C;
	if (delta >= 0) {
		return true;
	}	
	return false;
}

tuple<bool, float> ObjetoComFaces::hasIntersection(float p0x, float p0y, float p0z, float dx, float dy, float dz)
{

	bool intersecta;
	float t = 100000;

	if (!(ObjetoComFaces::podeTerIntersecao(p0x, p0y, p0z, dx, dy, dz))) {
		return make_tuple(false, t);
	}

	bool x;
	float y;

	for (int i = 0; i < size(this->faces); i++) {
		tie(x, y) = this->hasIntersectionFace(p0x, p0y, p0z, dx, dy, dz, i);
		if (x == true) {
			intersecta = true;
			if (y < t) {
				t = y;
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