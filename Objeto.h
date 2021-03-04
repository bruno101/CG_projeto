#pragma once
#include <tuple>
class Objeto
{

public:

	/* Retorna:
	1) O booleano que indica se h� intersecao.
	2) O valor de t para o qual h� a primeira intersecao.*/
	virtual std::tuple<bool, float> hasIntersection(float p0x, float p0y, float p0z, float dx, float dy, float dz) = 0;

};

