#include "Cluster.h"
#include <iostream>

using namespace std;

Cluster::Cluster(vector<ObjetoComFaces*> objetos)
{
	this->objetos = objetos;

	float xMin = 10000, xMax = -10000, yMin = 10000, yMax = -10000, zMin = 10000, zMax = -10000;
	vector<vector<float>> vertices;

	for (int obj = 0; obj < size(objetos); obj++) {

		vertices = objetos[obj]->vertices;

		for (int v = 0; v < size(vertices); v++) {
			if (vertices[v][0] < xMin) {
				xMin = vertices[v][0];
			}
			if (vertices[v][0] > xMax) {
				xMax = vertices[v][0];
			}
			if (vertices[v][1] < yMin) {
				yMin = vertices[v][1];
			}
			if (vertices[v][1] > yMax) {
				yMax = vertices[v][1];
			}
			if (vertices[v][2] < zMin) {
				zMin = vertices[v][2];
			}
			if (vertices[v][2] > zMax) {
				zMax = vertices[v][2];
			}

		}
	}

	this->centro = { (xMin + xMax) / 2, (yMin + yMax) / 2, (zMin + zMax) / 2 };
	this->raio = this->normaVetor(this->diferencaVetores(this->centro, { xMin, yMin, zMin }));

}

bool Cluster::podeTerIntersecao(float p0x, float p0y, float p0z, float dx, float dy, float dz) {
	vector<float> v = { p0x - this->centro[0], p0y - this->centro[1], p0z - this->centro[2] };
	float A = v[0] * v[0] + v[1] * v[1] + v[2] * v[2] - this->raio * this->raio;
	float B = 2 * v[0] * dx + 2 * v[1] * dy + 2 * v[2] * dz;
	float C = dx*dx + dy*dy + dz*dz;
	float delta = B*B - 4 * A*C;
	if (delta > 0) {
		return true;
	}
	return false;
}


tuple<bool, float> Cluster::hasIntersection(float p0x, float p0y, float p0z, float dx, float dy, float dz) 
{

	bool intersecta = false;
	float t = 100000;

	if (this->podeTerIntersecao( p0x, p0y, p0z, dx, dy, dz)) {

		bool x;
		float y;

		for (int i = 0; i < size(objetos); i++) {
			tie(x, y) = (*objetos[i]).hasIntersection(p0x, p0y, p0z, dx, dy, dz);
			if (x == true) {
				intersecta = true;
				if (y < t) {
					t = y;
				}
			}
		}

	}

	return make_tuple(intersecta, t);

}
