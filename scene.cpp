#include "scene.h"
#include <GL/gl.h>

Scene::Scene(int width, int height) {
	this->width = width;
	this->height = height;

	glClearColor(0.0, 0.4, 0.0, 0.5);
	glDisable(GL_DEPTH_TEST);
}

Scene::~Scene() {
}

bool Scene::render(int framenumber) {
	if(framenumber > 50) {
		// stop play
		return false;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.5, 2.5, -2.5, 2.5, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -4);
	glColor4f(0.1, 0.2, framenumber/50.0, 1.0);
	glBegin(GL_TRIANGLES);
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);
	glVertex3f(0, 1, 0);
	glEnd();
	return true;
}
