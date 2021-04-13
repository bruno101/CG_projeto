#include "ObjetoComFaces.h"
#include <stdlib.h>
#include <vector>
#include <tuple>
#include <GL/glut.h>
#include <iostream>

ObjetoComFaces::ObjetoComFaces(vector<vector<float>> vertices, vector<vector<int>> faces, vector<vector<float>> material)
{
	this->tipoObjeto = "Objeto com Faces";
	this->vertices = vertices;
	this->faces = faces;

	vector<float> somaVetoresVertices = { 0.0, 0.0, 0.0 };
	for (int v = 0; v < size(vertices); v++) {
		somaVetoresVertices = somaVetores(somaVetoresVertices, this->vertices[v]);
	}
	this->centro = multiplicaVetorPorEscalar(somaVetoresVertices, 1.0 / size(vertices));

	float raio = 0.0;
	float w;
	for (int v = 0; v < size(vertices); v++) {
		w = normaVetor(diferencaVetores(this->vertices[v], this->centro));
		if (w > raio) {
			raio = w;
		}
	}
	this->raio = raio;

	vector<vector<float>> normaisFaces(size(faces), vector<float>(3));
	for (int f = 0; f < size(faces); f++) {
		normaisFaces[f] = getNormalFace(f);
	}
	this->normaisFaces = normaisFaces;
	this->material = material;
}

ObjetoComFaces::ObjetoComFaces(vector<vector<float>> vertices, vector<vector<int>> faces, vector<vector<vector<float>>> materiaisFaces)
{
	this->tipoObjeto = "Objeto com Faces";
	this->vertices = vertices;
	this->faces = faces;
	this->objetoComMaisDeUmMaterial = true;

	vector<float> somaVetoresVertices = { 0.0, 0.0, 0.0 };
	for (int v = 0; v < size(vertices); v++) {
		somaVetoresVertices = somaVetores(somaVetoresVertices, this->vertices[v]);
	}
	this->centro = multiplicaVetorPorEscalar(somaVetoresVertices, 1.0 / size(vertices));

	float raio = 0.0;
	float w;
	for (int v = 0; v < size(vertices); v++) {
		w = normaVetor(diferencaVetores(this->vertices[v], this->centro));
		if (w > raio) {
			raio = w;
		}
	}
	this->raio = raio;

	vector<vector<float>> normaisFaces(size(faces), vector<float>(3));
	for (int f = 0; f < size(faces); f++) {
		normaisFaces[f] = getNormalFace(f);
	}
	this->normaisFaces = normaisFaces;
	this->materiaisFaces = materiaisFaces;
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

bool ObjetoComFaces::estaDentroDaFace(vector<float> pInt, int numFace) {

	int i1;
	vector<float> n = this->normaisFaces[numFace];
	vector<float> v, w;
	v = diferencaVetores(pInt, vertices[faces[numFace][0]]);
	for (int i = 0; i < size(faces[numFace]); i++) {
		i1 = i + 1 < size(faces[numFace]) ? i + 1 : 0;
		w = diferencaVetores(pInt, vertices[faces[numFace][i1]]);
		if (produtoEscalar(n, produtoVetorial(v, w)) < 0) {
			return false;
		}
		v = w;
	}
	return true;

}


tuple<bool, float> ObjetoComFaces::hasIntersectionFace(float p0x, float p0y, float p0z, float dx, float dy, float dz, int numFace) {

	float intersecta;
	float t = 100000;

	vector<float> n = this->normaisFaces[numFace];

	if (produtoEscalar(n, { dx, dy, dz }) >= 0.0) {
		return make_tuple(false, t);
	}

	vector<float> v = this->diferencaVetores( { p0x, p0y, p0z }, this->vertices[this->faces[numFace][0]] );

	float t0;

	//temos A+Bx=0

	float A = v[0]*n[0]+v[1]*n[1]+v[2]*n[2];
	float B = dx*n[0]+dy*n[1]+dz*n[2];

	if (B == 0) {
		//PARALELO
		intersecta = false;
	}
	else {

		t0 = -A / B;
		float pIntX = p0x + t0*dx;
		float pIntY = p0y + t0*dy;
		float pIntZ = p0z + t0*dz;

		if (t0 < 0) {
			intersecta = false;
		}
		else if (this->estaDentroDaFace( { pIntX, pIntY, pIntZ }, numFace)) {
			intersecta = true;
			t = t0;
		} 
		else {
			intersecta = false;
		}

	}

	return make_tuple(intersecta, t);

}

bool ObjetoComFaces::podeTerIntersecao(float p0x, float p0y, float p0z, float dx, float dy, float dz) {
	vector<float> v = { p0x-this->centro[0], p0y-this->centro[1], p0z-this->centro[2] };
	float C = v[0]*v[0] + v[1]*v[1] + v[2]*v[2] - this->raio * this->raio;
	float B = 2*v[0]*dx + 2*v[1]*dy + 2*v[2]*dz;
	float A = dx*dx + dy*dy + dz*dz;
	float delta = B*B - 4*A*C;
	if (delta > 0) {
		return true;
	}	
	return false;
}

tuple<bool, float, vector<vector<float>>, vector<float>> ObjetoComFaces::hasIntersection(float p0x, float p0y, float p0z, float dx, float dy, float dz)
{

	bool intersecta = false;
	float t = 100000;
	vector<vector<float>> material(3, vector<float>(3));
	vector<float> normal(3);

	if (!(ObjetoComFaces::podeTerIntersecao(p0x, p0y, p0z, dx, dy, dz))) {
		return make_tuple(false, t, material, normal);
	}

	bool x;
	float y;
	int numFaceIntersecao;

	for (int i = 0; i < size(this->faces); i++) {
		tie(x, y) = this->hasIntersectionFace(p0x, p0y, p0z, dx, dy, dz, i);
		if (x == true && y < t) {
			intersecta = true;
			t = y;
			numFaceIntersecao = i;
		}
	}

	if (intersecta) {
		normal = normaisFaces[numFaceIntersecao];
		if (this->objetoComMaisDeUmMaterial == true) {
			material = this->materiaisFaces[numFaceIntersecao];
		}
		else {
			material = this->material;
		}

	}

	return make_tuple(intersecta, t, material, normal);

}

vector<vector<float>> ObjetoComFaces::getListOfPoints() {
	vector<vector<float>> points = this->vertices;
	points.push_back(this->centro);
	return points;
}

void ObjetoComFaces::setListOfPoints(vector<vector<float>> newPoints) {
	this->centro = newPoints.back();
	newPoints.pop_back();
	this->vertices = newPoints;
	vector<vector<float>> normaisFaces(size(faces), vector<float>(3));
	for (int f = 0; f < size(faces); f++) {
		normaisFaces[f] = getNormalFace(f);
	}
	this->normaisFaces = normaisFaces;
}

void ObjetoComFaces::scale(float sx, float sy, float sz) {
	setListOfPoints(multiplyByMatrix(getListOfPoints(), scaleMatrix(sx, sy, sz)));
	this->raio = this->raio * fmax(sx, fmax(sy, sz));
	vector<vector<float>> normaisFaces(size(faces), vector<float>(3));
	for (int f = 0; f < size(faces); f++) {
		normaisFaces[f] = getNormalFace(f);
	}
	this->normaisFaces = normaisFaces;
}

void ObjetoComFaces::scaleP(float sx, float sy, float sz, vector<float> p) {
	setListOfPoints(multiplyByMatrix(getListOfPoints(), scaleMatrixP(sx, sy, sz, p)));
	this->raio = this->raio * fmax(sx, fmax(sy, sz));
	vector<vector<float>> normaisFaces(size(faces), vector<float>(3));
	for (int f = 0; f < size(faces); f++) {
		normaisFaces[f] = getNormalFace(f);
	}
	this->normaisFaces = normaisFaces;
}