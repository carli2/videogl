#include <iostream>
#include <GL/osmesa.h>
#include <GL/gl.h>
#include <SDL/SDL.h>
#include "scene.h"
#include <time.h>
#include <sched.h>
#include <getopt.h>
#include <dlfcn.h>

int32_t *buffer;

void show_help() {
	std::cout << "Usage: osmesa [Options]" << std::endl;
	std::cout << "Available options:" << std::endl;
	std::cout << "  -h --help    show this help" << std::endl;
	std::cout << "  -x --width   set video width (default 800)" << std::endl;
	std::cout << "  -y --height  set video height (default 480)" << std::endl;
	std::cout << "  -r --rate    set video width (default 25)" << std::endl;
	std::cout << "  -o --output  set output file (no output: live preview)" << std::endl;
	std::cout << "  -l --lib     use scene from this shared library" << std::endl;
	std::cout << std::endl;
	std::cout << "Shared libraries have to have a function of the following signature:" << std::endl;
	std::cout << "  Scene *getScene(int width, int height, int fps, int argc, char **argv)" << std::endl;
	std::cout << "include scene.h in order to implement the interface of Scene." << std::endl;
}

class DemoScene: public Scene {
	public:
	DemoScene(int width, int height, int fps): Scene(width, height, fps) {
		glClearColor(0.0, 0.4, 0.0, 0.5);
		glDisable(GL_DEPTH_TEST);
	}

	bool render(int framenumber) {
		if(framenumber > 2*fps) {
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
		glColor4f(0.1, 0.2, framenumber/2.0/fps, 1.0);
		glBegin(GL_TRIANGLES);
		glVertex3f(0, 0, 0);
		glVertex3f(1, 0, 0);
		glVertex3f(0, 1, 0);
		glEnd();
		return true;
	}
};

int main(int argc, char **argv) {
	char *outputvideofile = 0;
	char *lib = 0;
	int w = 800;
	int h = 480;
	int fps = 25;

	// parse parameters
	static struct option long_options[] =
	{
		/* These options set a flag. */
		{"help",   no_argument, 0, 'h'},
		{"width",  required_argument, 0, 'x'},
		{"height", required_argument, 0, 'y'},
		{"rate",   required_argument, 0, 'r'},
		{"output", required_argument, 0, 'o'},
		{"lib",    required_argument, 0, 'l'},
		{0, 0, 0, 0}
	};
	int option_index = 0, c;
	while((c = getopt_long(argc, argv, "hx:y:r:o:l:", long_options, &option_index)) != -1) {
		switch(c) {
			case 'h':
			show_help();
			return 1;
			case 'x':
			w = atoi(optarg);
			break;
			case 'y':
			h = atoi(optarg);
			break;
			case 'r':
			fps = atoi(optarg);
			break;
			case 'o':
			outputvideofile = optarg;
			break;
			case 'l':
			lib = optarg;
			break;
		}
	}

	OSMesaContext context;
	FILE *rawvideo;
	if(outputvideofile) {
		// OSMESA
		buffer = new int32_t[w*h];
		context = OSMesaCreateContextExt(OSMESA_RGBA, 24, 0, 0, NULL);
		if(!context) {
			std::cerr << "No context" << std::endl;
			return -1;
		}
		if(!OSMesaMakeCurrent(context, buffer, GL_UNSIGNED_BYTE, w, h)) {
			std::cerr << "MakeCurrent failed" << std::endl;
			return -1;
		}
		char command[1024];
		sprintf(command, "ffmpeg -vcodec rawvideo -f rawvideo -pix_fmt bgr32 -s %dx%d -r %d -i /dev/stdin -acodec null -vcodec libx264 -qp 0 -f mp4 %s -y", w, h, fps, outputvideofile);
		rawvideo = popen(command, "w");
	} else {
		// SDL-GL
		SDL_Init(SDL_INIT_VIDEO);
		SDL_SetVideoMode(w, h, 0, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL);
	}
	glViewport(0, 0, w, h);

	// create scene
	Scene *scene;
	if(lib) {
		void *handle = dlopen(lib, RTLD_LAZY | RTLD_LOCAL);
		if(!handle) {
			std::cerr << "library not found: " << lib << std::endl;
			return -1;
		}
		// C++-style
		void *getScene = dlsym(handle, "_Z8getSceneiiiiPPc");
		if(!getScene) {
			// C-Style
			getScene = dlsym(handle, "getScene");
			std::cerr << "warning: you are using C style symbols" << std::endl;
		}
		if(!getScene) {
			std::cerr << "library has no symbol getScene" << std::endl;
			return -1;
		}
		scene = ((Scene *(*)(int, int, int, int, char**))getScene)(w, h, fps, argc, argv);
		if(!scene) {
			std::cerr << "no scene provided by library" << std::endl;
			return -1;
		}
		//dlclose(handle);
	} else {
		scene = new DemoScene(w, h, fps);
	}

	// run the main loop
	int framenum = 0;
	while(true) {
		clock_t time = clock();
		bool cont = scene->render(framenum++);
		if(!cont) {
			break;
		}
		glFinish();
		if(outputvideofile) {
			for(int i = 0; i < h; i++) {
				fwrite(buffer+w*(h-i-1), 4, w, rawvideo);
			}
		} else {
			SDL_GL_SwapBuffers();
			SDL_Event ev;
			while(SDL_PollEvent(&ev)) {
				if(ev.type == SDL_QUIT
					|| (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE)) {
					cont = false;
				}
			}
			while(clock() - time < CLOCKS_PER_SEC*1.0/fps) {
				sched_yield();
			}
		}
		if(!cont) {
			break;
		}
	}

	// Destroy everything!
	delete scene;

	if(outputvideofile) {
		delete buffer;
		pclose(rawvideo);
		OSMesaDestroyContext(context);
	} else {
		SDL_Quit();
	}
}
