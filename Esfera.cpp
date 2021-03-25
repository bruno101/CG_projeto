#include "Esfera.h"



Esfera::Esfera()
{
}


vector<float> Esfera::getNormal(vector<float> ponto) {
	return{ 0,0,0 };
}

tuple<bool, float, vector<vector<float>>, vector<float>> Esfera::hasIntersection(float p0x, float p0y, float p0z, float dx, float dy, float dz) {
	return make_tuple(false, 10000, this->material, this->getNormal({ 0,0,0 }));
}
