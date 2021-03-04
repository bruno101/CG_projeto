#pragma once
#include <tuple>
#include <vector>

using namespace std;

class Objeto
{

public:

	/* Retorna:
	1) O booleano que indica se há intersecao.
	2) O valor de t para o qual há a primeira intersecao.*/
	virtual tuple<bool, float> hasIntersection(float p0x, float p0y, float p0z, float dx, float dy, float dz) = 0;

protected:

	float produtoEscalar(vector<float> x, vector<float> y);
	vector<float> produtoVetorial(vector<float> x, vector<float> y);

};

