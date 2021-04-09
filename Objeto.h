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
	vector<float> centro;
	float raio;
	string tipoObjeto;
	virtual tuple<bool, float, vector<vector<float>>, vector<float>> hasIntersection(float p0x, float p0y, float p0z, float dx, float dy, float dz) = 0;
	/*virtual void paint() = 0;*/

	void translate(float dx, float dy, float dz);
	void translateToOrigin(vector<float> point);
	void translateFromOrigin(vector<float> point);
	void rotateX(float ax, vector<float> pivot = { 0, 0, 0 });
	void rotateY(float ay, vector<float> pivot = { 0, 0, 0 });
	void rotateZ(float az, vector<float> pivot = { 0, 0, 0 });
	void transform(vector<vector<float>> M);

	static float produtoEscalar(vector<float> x, vector<float> y);
	static vector<float> produtoVetorial(vector<float> x, vector<float> y);
	static vector<float> somaVetores(vector<float> x, vector<float> y);
	static vector<float> diferencaVetores(vector<float> x, vector<float> y);
	static vector<float> multiplicaVetorPorEscalar(vector<float> x, float k);
	static vector<float> normalizaVetor(vector<float> v);
	static float normaVetor(vector<float> x);

protected:

	vector<vector<float>> material;
	bool objetoComMaisDeUmMaterial = false;

	vector<vector<float>> multiplyMatrix(vector<vector<float>> A, vector<vector<float>> B);
	vector<float> multiplyVectorByMatrix(vector<float> V, vector<vector<float>> M);
	vector<vector<float>> multiplyByMatrix(vector<vector<float>> points, vector<vector<float>> M);
	vector<vector<float>> translationMatrix(float dx, float dy, float dz);
	vector<vector<float>> translationToOriginMatrix(vector<float> point);
	vector<vector<float>> translationFromOriginMatrix(vector<float> point);
	vector<vector<float>> scaleMatrix(float sx, float sy, float sz);
	vector<vector<float>> rotationMatrixX(float ax, vector<float> pivot);
	vector<vector<float>> rotationMatrixY(float ay, vector<float> pivot);
	vector<vector<float>> rotationMatrixZ(float az, vector<float> pivot);

	virtual vector<vector<float>> getListOfPoints()=0;
	virtual void setListOfPoints(vector<vector<float>> newPoints)=0;

	/*virtual void translate(float dx, float dy, float dz) = 0;
	virtual void translateToOrigin(vector<float> point) = 0;
	virtual void translateFromOrigin(vector<float> point) = 0;
	virtual void scale(float sx, float sy, float sz) = 0;
	virtual void rotateX(float ax, vector<float> pivot = { 0, 0, 0 }) = 0;
	virtual void rotateY(float ax, vector<float> pivot = { 0, 0, 0 }) = 0;
	virtual void rotateZ(float ax, vector<float> pivot = { 0, 0, 0 }) = 0;*/


};

