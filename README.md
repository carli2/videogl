videogl
=======

OpenGL based Video compositor

Use this piece of software if you want to create videos out of 3d scenes. This project uses SDL and OpenGL for a video preview and osmesa and ffmpeg for video production.

Usage
-----

`osmesa [Options]`

Available options:
 * -h --help    show this help
 * -x --width   set video width (default 800)
 * -y --height  set video height (default 480)
 * -r --rate    set video width (default 25)
 * -o --output  set output file (no output: live preview)
 * -l --lib     use scene from this shared library


Custom Scenes
-------------

Begin with the following template:

```C++
#include "scene.h"
#include <GL/gl.h>

class MyScene: public Scene {
	public:
	MyScene(int width, int height, int fps /* add params here */): Scene(width, height, fps) {
		// initialize GL flags ...
		// load textures ...
	}

	bool render(int framenumber) {
		if(framenumber > 15*fps) {
			// stop playing after 15 seconds
			return false;
		}
		// place GL render code here ...
		return true;
	}
};

Scene *getScene(int width, int height, int fps, int argc, char **argv) {
	// create the scene
	// parse additional parameters here
	return new MyScene(width, height, fps);
}

```

You can try an example:
 - compile example/hello.cpp with `gcc examples/hello.cpp -shared -ohello.so -fPIC`
 - render the scene with `osmesa -l ./hello.so`


Helper Functions
----------------

There are some helpers like the `RectangularGrid` helper. An example is `grid.so` which can also be viewed in http://youtu.be/kDjiN1X54q8
