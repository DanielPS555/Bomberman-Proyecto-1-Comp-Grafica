#include "shapes.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include <stdio.h>
#include <GL/glu.h>


void iniciliarRenderVertexArray();
void finalizarRenderVertexArray();
void renderRectangulo2d(retangulo2d rec, GLuint textura);
void renderRectangulo3d(retangulo3d rec, GLuint textura);
