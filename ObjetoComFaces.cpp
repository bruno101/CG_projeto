#include "ObjetoComFaces.h"
#include <stdlib.h>
#include <vector>
#include <tuple>
#include <GL/glut.h>


ObjetoComFaces::ObjetoComFaces(vector<vector<float>> vertices, vector<vector<int>> faces, GLfloat color[3])
{
	this->vertices = vertices;
	this->faces = faces;
	copy(color, color+3, this->color);
}

tuple<bool, float> ObjetoComFaces::hasIntersection(float p0x, float p0y, float p0z, float dx, float dy, float dz)
{
	return make_tuple(1,1);
}

void ObjetoComFaces::paint() {

	for (int i = 0; i < size(this->faces); i++) {

		if (size(this->faces[i]) == 3) {

			glBegin(GL_TRIANGLES);
			glColor3fv(this->color);
			glVertex3fv(this->vertices[this->faces[i][0]].data());
			glVertex3fv(this->vertices[this->faces[i][1]].data());
			glVertex3fv(this->vertices[this->faces[i][2]].data());
			glEnd();

		}

		if (size(this->faces[i]) == 4) {

			glBegin(GL_QUADS);
			glColor3fv(this->color);
			glVertex3fv(this->vertices[this->faces[i][0]].data());
			glVertex3fv(this->vertices[this->faces[i][1]].data());
			glVertex3fv(this->vertices[this->faces[i][2]].data());
			glVertex3fv(this->vertices[this->faces[i][3]].data());
			glEnd();

		}

	}

}