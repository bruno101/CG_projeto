#pragma once
#include "Objeto.h"
class Esfera :
	public Objeto
{
private:
        vector<float> centro;
	float R;
	vector<vector<float>> material;
	vector<float> getNormal(vector<float> ponto);

public:
	Esfera(vector<float> centro, float R, vector<vector<float>> material);
	tuple<bool, float, vector<vector<float>>, vector<float>> hasIntersection(float p0x, float p0y, float p0z, float dx, float dy, float dz);
};

