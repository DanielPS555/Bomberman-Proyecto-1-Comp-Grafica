#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include <stdio.h>
#include <GL/glu.h>


#ifndef SHAPES
#define SHAPES

//  v3------v2
//  |        |
//  |        |
//  |        |
//  v0------v1


//    v4----- v7
//   /|      /|
//  v3------v2|
//  | |     | |
//  | |v5---|-|v6
//  |/      |/
//  v0------v1
struct retangulo3dss {
	GLfloat cordeneasVertices[6 * 4 * 3]; // Idem al ejemplo anterior, pero utilizamos 6 vertices
	GLfloat normales[6 * 3];
	GLfloat coloresVertices[6 * 4 * 3];
	GLubyte indices[6 * 4];
	GLfloat texturas[6 * 4 * 2];
};

struct vertice {
	GLfloat cordenadas[3];
	GLfloat colores[3];
	GLfloat texturas[2];
};


retangulo3dss createRetangulo3d(vertice vertices[8]);


#endif