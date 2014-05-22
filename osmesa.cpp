#include <iostream>
#include <stdio.h>
#include <GL/osmesa.h>
#include <GL/gl.h>
#include <SDL/SDL.h>
#include "scene.h"
#include <time.h>
#include <sched.h>
#include <getopt.h>

int32_t *buffer;

int main(int argc, char **argv) {
	char *outputvideofile = 0;
	int w = 800;
	int h = 480;
	int fps = 25;

	// parse parameters
	static struct option long_options[] =
	{
		/* These options set a flag. */
		{"width",  required_argument, 0, 'w'},
		{"height",  required_argument, 0, 'h'},
		{"rate",    required_argument, 0, 'r'},
		{"output",    required_argument, 0, 'o'},
		{0, 0, 0, 0}
	};
	int option_index = 0, c;
	while((c = getopt_long(argc, argv, "w:h:r:o:", long_options, &option_index)) != -1) {
		switch(c) {
			case 'w':
			w = atoi(optarg);
			break;
			case 'h':
			h = atoi(optarg);
			break;
			case 'r':
			fps = atoi(optarg);
			break;
			case 'o':
			outputvideofile = optarg;
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
			printf("No context");
			return -1;
		}
		if(!OSMesaMakeCurrent(context, buffer, GL_UNSIGNED_BYTE, w, h)) {
			printf("MakeCurrent failed");
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
	Scene scene(w, h);

	// run the main loop
	int framenum = 0;
	while(true) {
		clock_t time = clock();
		bool cont = scene.render(framenum++);
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

	if(outputvideofile) {
		delete buffer;
		pclose(rawvideo);
		OSMesaDestroyContext(context);
	} else {
		SDL_Quit();
	}
}
