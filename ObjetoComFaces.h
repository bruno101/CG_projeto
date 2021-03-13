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
	vector<vector<float>> vertices;
	vector<vector<int>> faces;
	GLfloat color[3];
	vector<float> getNormalFace(int numFace);
	virtual bool podeTerIntersecao(float p0x, float p0y, float p0z, float dx, float dy, float dz);
	tuple<bool, float> hasIntersectionFace(float p0x, float p0y, float p0z, float dx, float dy, float dz, int numFace);

public:
	ObjetoComFaces(vector<vector<float>> vertices, vector<vector<int>> faces, GLfloat color[3]);
	tuple<bool, float> hasIntersection(float p0x, float p0y, float p0z, float dx, float dy, float dz);
	void paint();
};

