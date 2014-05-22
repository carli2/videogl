#include "../scene.h"
#include "GL/gl.h"
#include "math.h"


class HelloScene: public Scene {
	public:
	HelloScene(int width, int height, int fps): Scene(width, height, fps) {
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glDisable(GL_DEPTH_TEST);
	}

	bool render(int framenumber) {
		if(framenumber > 150*fps) {
			// stop play
			return false;
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, width, height, 0, -10.0, 10.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glColor4f(sin(framenumber*0.1/fps), sin(framenumber*0.2/fps), sin(framenumber*0.4/fps), 1.0);
		glBegin(GL_QUADS);
		glVertex3f(0, 0, 0);
		glVertex3f(width, 0, 0);
		glVertex3f(width, height, 0);
		glVertex3f(0, height, 0);
		glEnd();
		return true;
	}
};

Scene *getScene(int width, int height, int fps, int argc, char **argv) {
	return new HelloScene(width, height, fps);
}
