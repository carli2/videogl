#include "videohelpers.h"
#include "GL/gl.h"
#include <iostream>
#include <math.h>


extern RectangularGrid::RectangularGrid(int resolution) {
	this->resolution = resolution;
	// allocate memory
	indices = new uint32_t[resolution * resolution * 6];
	vertices = new float[(1 + resolution) * (1 + resolution) * 5];
	// fill indices
	for(int i = 0; i < resolution; i++) {
		for(int j = 0; j < resolution; j++) {
			int idx = 6 * (i * resolution + j);
			indices[idx+0] = (i+0) * (resolution + 1) + (j+0);
			indices[idx+1] = (i+1) * (resolution + 1) + (j+0);
			indices[idx+2] = (i+0) * (resolution + 1) + (j+1);
			indices[idx+3] = (i+1) * (resolution + 1) + (j+0);
			indices[idx+4] = (i+0) * (resolution + 1) + (j+1);
			indices[idx+5] = (i+1) * (resolution + 1) + (j+1);
		}
	}
	// reset the grid
	resetGrid();
}

RectangularGrid::~RectangularGrid() {
	delete indices;
	delete vertices;
}

void RectangularGrid::resetGrid() {
	for(int i = 0; i <= resolution; i++) {
		for(int j = 0; j <= resolution; j++) {
			int idx = 5 * (i * (resolution + 1) + j);
			vertices[idx+0] = i / (float)resolution;
			vertices[idx+1] = j / (float)resolution;
			vertices[idx+2] = 0;
			vertices[idx+3] = i / (float)resolution;
			vertices[idx+4] = j / (float)resolution;
		}
	}
}

void RectangularGrid::render() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 5 * 4, vertices);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 5 * 4, vertices + 3);
	glDrawElements(GL_TRIANGLES, 6 * resolution * resolution, GL_UNSIGNED_INT, indices);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void bringToLength(float *a, float *b, float length) {
	float x = a[0]-b[0], y = a[1]-b[1];
	// test if we stretch too much
	if(x*x + y*y > length*length) {
		// smooth it out
		float reallen = sqrt(x*x + y*y);
		x /= reallen;
		y /= reallen;
		/*
		float midx = (a[0] + b[0]) / 2.0;
		float midy = (a[1] + b[1]) / 2.0;
		a[0] = midx + length / 2 * x;
		a[1] = midy + length / 2 * y;
		b[0] = midx - length / 2 * x;
		b[1] = midy - length / 2 * y;
		*/
		b[0] = a[0] - length * x;
		b[1] = a[1] - length * y;
	}
}

void RectangularGrid::leaf() {
	// move one vertex
	int rightUpperIndex = 5 * ((1 + resolution) * resolution);
	vertices[rightUpperIndex + 0] -= 0.03;
	vertices[rightUpperIndex + 1] += 0.02;
	// smooth all edges to get old length
	for(int i = resolution-1; i >= 0; i--) {
		for(int j = 0; j <= resolution; j++) {
			bringToLength(&vertices[5*(j + (1 + resolution) * (i+1))],
			              &vertices[5*(j + (1 + resolution) * i)],
				      1.0/resolution);
		}
	}
	for(int i = 0; i < resolution; i++) {
		for(int j = resolution; j >= 0; j--) {
			bringToLength(&vertices[5*(i + (1 + resolution) * j)],
			              &vertices[5*((i+1) + (1 + resolution) * j)],
				      1.0/resolution);
		}
	}
}
