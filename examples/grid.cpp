#include "scene.h"
#include "videohelpers.h"
#include "GL/gl.h"
#include "math.h"


class GridScene: public Scene {
	private:
	RectangularGrid g;

	public:
	GridScene(int width, int height, int fps): Scene(width, height, fps), g(10) {
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
		glTranslatef(50, 50, 0);
		glScalef(300, 300, 0);
		glColor4f(sin(framenumber*0.1/fps), sin(framenumber*0.2/fps), sin(framenumber*0.4/fps), 1.0);

		g.render();

		/*glBegin(GL_LINE_LOOP);
		glVertex2f(0, 0);
		glVertex2f(1, 0);
		glVertex2f(1, 1);
		glVertex2f(0, 1);
		glEnd();*/
		return true;
	}
};

Scene *getScene(int width, int height, int fps, int argc, char **argv) {
	return new GridScene(width, height, fps);
}
