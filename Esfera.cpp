#include "Esfera.h"



Esfera::Esfera(vector<float> centro, float R, vector<vector<float>> material)
{
	this->centro = centro;
	this.R = R;
	this.material = material;
}


vector<float> Esfera::getNormal(vector<float> ponto) {
	vector<floa> w = this->diferencaVetores(ponto, this->centro);
	return this->multiplicaVetorPorEscalar(w, 1.0/normaVetor(w))
}

tuple<bool, float, vector<vector<float>>, vector<float>> Esfera::hasIntersection(float p0x, float p0y, float p0z, float dx, float dy, float dz) {
	vector<float> w = this->diferencaVetores({ p0x,p0y,p0z }, this->centro);
	vector<float> dr = { dx, dy, dz };
	
	float A = produtoEscalar(
}
