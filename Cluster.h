#pragma once
#include "Objeto.h"
#include "ObjetoComFaces.h"
#include <vector>

using namespace std;

class Cluster :
	public Objeto
{
public:
	Cluster(vector<ObjetoComFaces*> objetos);
	tuple<bool, float, vector<vector<float>>, vector<float>> hasIntersection(float p0x, float p0y, float p0z, float dx, float dy, float dz);

private:
	vector<ObjetoComFaces*> objetos;
	bool podeTerIntersecao(float p0x, float p0y, float p0z, float dx, float dy, float dz);

	vector<vector<float>> getListOfPoints();
	void setListOfPoints(vector<vector<float>> newPoints);

};

