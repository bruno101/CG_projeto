#pragma once
#include "Objeto.h"
class Cilindro :
	public Objeto
{
private:
	vector<float> centro;
	vector<float> centroBase2;
	vector<float> Hdir;
	float H;
	float R;
	vector<vector<float>> material;
	vector<float> getNormal(vector<float> ponto);

public:
	Cilindro(vector<float> centro, vector<float> Hdir, float H, float R, vector<vector<float>> material);
	tuple<bool, float, vector<vector<float>>, vector<float>> hasIntersection(float p0x, float p0y, float p0z, float dx, float dy, float dz);


};

