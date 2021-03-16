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
	/*virtual void paint() = 0;*/

protected:

	vector<float> centro;
	float raio;
	virtual bool podeTerIntersecao(float p0x, float p0y, float p0z, float dx, float dy, float dz) = 0;
	float produtoEscalar(vector<float> x, vector<float> y);
	vector<float> produtoVetorial(vector<float> x, vector<float> y);
	vector<float> somaVetores(vector<float> x, vector<float> y);
	vector<float> diferencaVetores(vector<float> x, vector<float> y);
	vector<float> multiplicaVetorPorEscalar(vector<float> x, float k);
	float normaVetor(vector<float> x);

};

