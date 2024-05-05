#include "shapes.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include <stdio.h>
#include <GL/glu.h>
#include "mathVector.h"



#ifndef RENDER_UTILS
#define RENDER_UTILS

extern bool mostrarTexturas;

void iniciliarRenderVertexArray();
void finalizarRenderVertexArray();

void renderRectangulo3d(retangulo3dss rec, GLuint textura);


#endif