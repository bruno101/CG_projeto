#include "Cone.h"
#include <iostream>


Cone::Cone(vector<float> centro, vector<float> Hdir, float H, float R, vector<vector<float>> material)
{
	this->centro = centro;
	this->Hdir = Hdir;
	this->V = this->somaVetores(centro, multiplicaVetorPorEscalar(Hdir, H));
	this->H = H;
	this->R = R;
	this->beta = 1 + pow(this->R / this->H, 2);
	this->material = material;
}


vector<float> Cone::getNormal(vector<float> ponto) {
	
	vector<float> v = this->diferencaVetores(this->V, ponto);
	vector<float> psi = this->produtoVetorial(v, Hdir);
	vector<float> N = this->produtoVetorial(psi, v);

	return this->multiplicaVetorPorEscalar(N, 1.0 / normaVetor(N));
}

tuple<bool, float, vector<vector<float>>, vector<float>> Cone::hasIntersection(float p0x, float p0y, float p0z, float dx, float dy, float dz) {

	vector<float> dr = { dx, dy, dz };
	vector<float> w = { this->V[0] - p0x, this->V[1] - p0y , this->V[2] - p0z };

	float A = produtoEscalar(dr, dr) - beta*pow(produtoEscalar(dr,Hdir),2);
	float B = - 2 * (produtoEscalar(w,dr) - beta*(produtoEscalar(w,Hdir))*(produtoEscalar(dr,Hdir)));
	float C = produtoEscalar(w,w) - beta*(pow(produtoEscalar(w,Hdir),2));

	float t0;

	float delta = B * B - 4 * A * C;
	vector<float> normal;

	if (delta <= 0) {
		t0 = 10000;
	}
	else if (delta == 0) {

		float t0 = (-B) / (2 * A);

		float x = produtoEscalar({V[0] - p0x - dx*t0, V[1] - p0y - dy*t0 , V[2] - p0z - dz*t0 }, Hdir);
		if (x < 0 || x > this->H) {
			t0 = 10000;
		}

	}
	else {

		float tA = (-B + sqrt(delta)) / (2 * A);
		float xA = produtoEscalar({ V[0] - p0x - dx*tA, V[1] - p0y - dy*tA , V[2] - p0z - dz*tA }, Hdir);
		float tB = (-B - sqrt(delta)) / (2 * A);
		float xB = produtoEscalar({ V[0] - p0x - dx*tB, V[1] - p0y - dy*tB , V[2] - p0z - dz*tB }, Hdir);

		if (xA < 0 || xA > this->H) {
			tA = 10000;
		}
		if (xB < 0 || xB > this->H) {
			tB = 10000;
		}


		t0 = fmin(tA, tB);
	}

	if (t0 < 10000) {

		//Checando interseção com face

		float t1;

		vector<float> v1 = this->diferencaVetores({ p0x, p0y, p0z }, this->centro);
		vector<float> n1 = multiplicaVetorPorEscalar(this->Hdir, -1.0);

		//temos A+Bx=0

		A = v1[0] * n1[0] + v1[1] * n1[1] + v1[2] * n1[2];
		B = dx*n1[0] + dy*n1[1] + dz*n1[2];

		if (B == 0) {
			//PARALELO
			t1 = 10000;
		}
		else {

			t1 = -A / B;
			float pIntX = p0x + t1*dx;
			float pIntY = p0y + t1*dy;
			float pIntZ = p0z + t1*dz;

			if (this->normaVetor(this->diferencaVetores({ pIntX, pIntY, pIntZ }, centro)) > this->R) {
				t1 = 10000;
			}

		}

		if (t0 < t1) {
			return make_tuple(true, t0, this->material, getNormal({ p0x + dx*t0, p0y + dy*t0, p0z + dz*t0 }));
		}
		else {
			return make_tuple(true, t1, this->material, Hdir);
		}

	}

	
	return make_tuple(false, t0, this->material, normal);

}
