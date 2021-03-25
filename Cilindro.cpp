#include "Cilindro.h"
#include <math.h>
#include <tuple>
#include <iostream>


Cilindro::Cilindro(vector<float> centro, vector<float> Hdir, float H, float R, vector<vector<float>> material)
{
	this->centro = centro;
	this->centroBase2 = this->somaVetores(centro, multiplicaVetorPorEscalar(Hdir, H));
	this->Hdir = Hdir;
	this->H = H;
	this->R = R;
	this->material = material;
}

vector<float> Cilindro::getNormal(vector<float> ponto) {
	//implementar
	vector<float> w = this->diferencaVetores(ponto, this->centro);
	vector<float> N = { this->diferencaVetores(w, this->multiplicaVetorPorEscalar(this->Hdir, this->produtoEscalar(w, this->Hdir))) };
	return this->multiplicaVetorPorEscalar(N, 1.0 / normaVetor(N));
}

tuple<bool, float, vector<vector<float>>, vector<float>> Cilindro::hasIntersection(float p0x, float p0y, float p0z, float dx, float dy, float dz) {

	vector<float> w = this->diferencaVetores({ p0x,p0y,p0z }, this->centro);
	vector<float> dr = { dx, dy, dz };

	float A = 1 - pow((this->produtoEscalar(dr, this->Hdir)), 2);
	float B = 2 * (this->produtoEscalar(w, dr) - (produtoEscalar(w, this->Hdir)) * (produtoEscalar(dr, this->Hdir)));
	float C = this->produtoEscalar(w,w) - pow(produtoEscalar(w,this->Hdir),2) - pow(this->R,2);

	float t0;

	float delta = B * B - 4 * A * C;
	vector<float> normal;

	if (delta <= 0) {
		t0 = 10000;
	}
	else if (delta == 0) {
		float t0 = (-B) / (2 * A);
		float x = produtoEscalar(this->Hdir, { p0x + dx*t0 - this->centro[0], p0y + dy*t0 - this->centro[1], p0z + dz*t0 - this->centro[2] });
		if ( x > this->H || x < 0) {
			t0 = 10000;
		}
	}
	else {
		float tA = (-B + sqrt(delta)) / (2 * A);
		float xA = produtoEscalar(this->Hdir, { p0x + dx*tA - this->centro[0], p0y + dy*tA - this->centro[1], p0z + dz*tA - this->centro[2] });
		float tB = (-B - sqrt(delta)) / (2 * A);
		float xB = produtoEscalar(this->Hdir, { p0x + dx*tB - this->centro[0], p0y + dy*tB - this->centro[1], p0z + dz*tB - this->centro[2] });

		if (xA > this->H || xA < 0) {
			tA = 10000;
		} 
		if (xB > this->H || xB < 0) {
			tB = 10000;
		}

		t0 = fmin(tA, tB);
	}

	//Checando interseção com faces

	//face 1

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

	//face 2

	float t2;

	vector<float> v2 = this->diferencaVetores({ p0x, p0y, p0z }, this->centroBase2);
	vector<float> n2 = this->Hdir;

	//temos A+Bx=0

	A = v2[0] * n2[0] + v2[1] * n2[1] + v2[2] * n2[2];
	B = dx*n2[0] + dy*n2[1] + dz*n2[2];

	if (B == 0) {
		//PARALELO
		t2 = 10000;
	}
	else {

		t2 = -A / B;
		float pIntX = p0x + t2*dx;
		float pIntY = p0y + t2*dy;
		float pIntZ = p0z + t2*dz;

		if (this->normaVetor(this->diferencaVetores({ pIntX, pIntY, pIntZ }, centroBase2)) > this->R) {
			t2 = 10000;
		}

	}

	float min = fmin(t0, fmin(t1, t2));

	if (min == 10000) {
		return make_tuple(false, 10000, this->material, normal);
	}
	else if (t0 == min) {
		return make_tuple(true, t0, this->material, this->getNormal({ p0x + dx*t0, p0y + dy*t0, p0z + dz*t0 }));
	}
	else if (t1 == min) {
		return make_tuple(true, t1, this->material, n1);
	}
	else {
		return make_tuple(true, t2, this->material, n2);
	}


}