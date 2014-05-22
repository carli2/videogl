videogl
=======

OpenGL based Video compositor

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

Shared libraries have to have a function of the following signature:
`Scene *getScene(int width, int height, int fps, int argc, char **argv)`
include scene.h in order to implement the interface of Scene.


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

