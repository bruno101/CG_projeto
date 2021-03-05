#include "Objeto.h"
#include <tuple>
#include <vector>

float Objeto::produtoEscalar(vector<float> x, vector<float> y) {

	return (x[0] * y[0] + x[1] * y[1] + x[2] * y[2]);

}

vector<float> Objeto::produtoVetorial(vector<float> x, vector<float> y) {

	vector<float> z;

	z[0] = x[1]*y[2]-x[2]*y[1];
	z[1] = x[2]*y[0]-x[0]*y[2];
	z[2] = x[0]*y[1]-x[1]*y[0];

	return z;

}

vector<float> Objeto::somaVetores(vector<float> x, vector<float> y) {
	vector<float> v;
	v[0] = x[0] + y[0];
	v[1] = x[1] + y[1];
	v[2] = x[2] + y[2];
	return v;
}

vector<float> Objeto::diferencaVetores(vector<float> x, vector<float> y) {
	vector<float> v;
	v[0] = x[0] - y[0];
	v[1] = x[1] - y[1];
	v[2] = x[2] - y[2];
	return v;
}

vector<float> Objeto::multiplicaVetorPorEscalar(vector<float> x, float k) {
	vector<float> v;
	v[0] = x[0] * k;
	v[1] = x[1] * k;
	v[2] = x[2] * k;
	return v;
}

float Objeto::normaVetor(vector<float> x) {
	return (sqrt(x[0]*x[0]+x[1]*x[1]*x[2]*x[2]));
}