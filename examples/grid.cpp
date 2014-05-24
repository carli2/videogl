#include "scene.h"
#include "videohelpers.h"
#include "GL/gl.h"
#include "math.h"
#include <cstdio>


class GridScene: public Scene {
	private:
	RectangularGrid g;
	GLuint tex;

	public:
	GridScene(int width, int height, int fps): Scene(width, height, fps), g(100) {
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glDisable(GL_DEPTH_TEST);

		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		const int texsize = 256;
		uint32_t *buf = new uint32_t[texsize * texsize];
		for(int i = 0; i < texsize * texsize; i++) {
			buf[i] = i + (255 << 16);
		}
		glTexImage2D(GL_TEXTURE_2D, 0, 3, texsize, texsize, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf);
		delete buf;
	}

	~GridScene() {
		glDeleteTextures(1, &tex);
	}

	bool render(int framenumber) {
		if(framenumber > 5*fps) {
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
		glColor4f(1.0, 1.0, 1.0, 1.0);

		glBegin(GL_QUADS);
		glVertex2f(0, 0);
		glVertex2f(1, 0);
		glVertex2f(1, 1);
		glVertex2f(0, 1);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, tex);
		glEnable(GL_TEXTURE_2D);
		g.render();
		glDisable(GL_TEXTURE_2D);

		// logic
		g.leaf();

		return true;
	}
};

Scene *getScene(int width, int height, int fps, int argc, char **argv) {
	return new GridScene(width, height, fps);
}
