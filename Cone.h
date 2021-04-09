#pragma once
#include "Objeto.h"
class Cone :
	public Objeto
{
private:
	vector<float> centro;
	vector<float> Hdir;
	vector<float> V;
	float beta;
	float H;
	float R;
	vector<vector<float>> material;
	vector<float> getNormal(vector<float> ponto);

	vector<vector<float>> getListOfPoints();
	void setListOfPoints(vector<vector<float>> newPoints);

public:
	Cone(vector<float> centro, vector<float> Hdir, float H, float R, vector<vector<float>> material);
	tuple<bool, float, vector<vector<float>>, vector<float>> hasIntersection(float p0x, float p0y, float p0z, float dx, float dy, float dz);
	void scale(float sBase, float sAltura);

};

