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
template<std::size_t N> // Numero de diviciones por cara
struct retangulo2d {
	GLfloat cordeneasVerticesBordes[4 * 3];   //Ejemplo: 1,1,1,  -1,1,1,  -1,-1,1,  1,-1,1,  ==> v0-v1-v2-v3
	GLfloat normales[3];
	GLfloat coloresVertices[3];   
	GLubyte indices[4];
	GLfloat corrdenadasTexturasBordes[4 * 2];
	GLubyte verticesIntermedios[N];

};

struct retanguloTransparente2d {
	GLfloat cordeneasVertices[4 * 3];   //Ejemplo: 1,1,1,  -1,1,1,  -1,-1,1,  1,-1,1,  ==> v0-v1-v2-v3
	GLfloat normales[3];
	GLfloat coloresVertices[4 * 4];   //Ejemplo  1,1,1,  1,1,0,   1,0,0,    1,0,1,   ==> v0-v1-v2-v3
	GLubyte indices[4];
	GLfloat texturas[4 * 2];
};



//    v4----- v7
//   /|      /|
//  v3------v2|
//  | |     | |
//  | |v5---|-|v6
//  |/      |/
//  v0------v1
struct retangulo3d {
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


retangulo3d createRetangulo3d(vertice vertices[8]);


#endif