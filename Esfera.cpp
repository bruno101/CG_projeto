#include "Esfera.h"



Esfera::Esfera(vector<float> centro, float R, vector<vector<float>> material)
{
	this->centro = centro;
	this->R = R;
	this->material = material;
}


vector<float> Esfera::getNormal(vector<float> ponto) {
	vector<float> w = this->diferencaVetores(ponto, this->centro);
	return this->multiplicaVetorPorEscalar(w, 1.0 / normaVetor(w));
}

tuple<bool, float, vector<vector<float>>, vector<float>> Esfera::hasIntersection(float p0x, float p0y, float p0z, float dx, float dy, float dz) {
	
	vector<float> w = this->diferencaVetores({ p0x,p0y,p0z }, this->centro);
	vector<float> dr = { dx, dy, dz };

	float A = produtoEscalar(dr, dr);
	float B = 2 * produtoEscalar(w, dr);
	float C = produtoEscalar(w, w) - R*R;

	float t0;

	float delta = B*B - 4 * A*C;

	vector<float> normal;

	if (delta < 0) {
		t0 = 10000;

	}
	else if (delta == 0) {
		float t0 = (-B) / (2 * A);

		if (t0 < 0) {
			t0 = 10000;
		}

	}
	else {
		float tA = (-B + sqrt(delta)) / (2 * A);
		float tB = (-B - sqrt(delta)) / (2 * A);

		if (tA < 0 && tB < 0) {
			t0 = 10000;
		}
		else if (tA < 0) {
			t0 = tB;
		}
		else if (tB < 0){
			t0 = tA;
		}
		else {
			t0 = fmin(tA, tB);
		}

	}

	if (t0 == 10000) {
		return make_tuple(false, 10000, this->material, normal);

	}
	else {
		return make_tuple(true, t0, this->material, this->getNormal({ p0x + dx*t0, p0y + dy*t0, p0z + dz*t0 }));

	}
	
}

vector<vector<float>> Esfera::getListOfPoints() {
	return{ this->centro };
}

void Esfera::setListOfPoints(vector<vector<float>> newPoints) {
	this->centro = newPoints[0];
}

void Esfera::scale(float s) {
	this->R *= s;
}

void Esfera::scaleP(float s, vector<float> p) {
	this->centro = multiplyVectorByMatrix(this->centro, scaleMatrixP(s, s, s, p));
	this->R *= s;
}