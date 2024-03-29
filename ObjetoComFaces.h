#pragma once
#include "Objeto.h"
#include <stdlib.h>
#include <vector>
#include <tuple>
#include <GL/glut.h>

using namespace std;

class ObjetoComFaces :
	public Objeto
{
private:
	vector<vector<int>> faces;
	vector<vector<float>> normaisFaces;
	vector<vector<vector<float>>> materiaisFaces;
	vector<float> getNormalFace(int numFace);
	virtual bool podeTerIntersecao(float p0x, float p0y, float p0z, float dx, float dy, float dz);
	tuple<bool, float> hasIntersectionFace(float p0x, float p0y, float p0z, float dx, float dy, float dz, int numFace);
	bool estaDentroDaFace(vector<float> pInt, int numFace);

	vector<vector<float>> getListOfPoints();
	void setListOfPoints(vector<vector<float>> newPoints);

public:
	vector<vector<float>> vertices;
	ObjetoComFaces(vector<vector<float>> vertices, vector<vector<int>> faces, vector<vector<float>> material);
	ObjetoComFaces(vector<vector<float>> vertices, vector<vector<int>> faces, vector<vector<vector<float>>> materiaisFaces);
	void scale(float sx, float sy, float sz);
	void scaleP(float sx, float sy, float sz, vector<float> p);

	tuple<bool, float, vector<vector<float>>, vector<float>> hasIntersection(float p0x, float p0y, float p0z, float dx, float dy, float dz);
};

