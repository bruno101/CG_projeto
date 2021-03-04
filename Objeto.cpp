#include "Objeto.h"
#include <tuple>
#include <vector>

float Objeto::produtoEscalar(vector<float> x, vector<float> y) {

	return (x[0] * y[0] + x[1] * y[1] + x[2] * y[2]);

}

vector<float> Objeto::produtoVetorial(vector<float> x, vector<float> y) {

	vector<float> z;

	z[0] = x[1]*y[2]-x[2]*y[1];
	z[1] = x[0]*y[2]-x[2]*y[0];
	z[2] = x[0]*y[1]-x[1]*y[0];

	return z;

}
