#include "shapes.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include <stdio.h>
#include <GL/glu.h>



#ifndef RENDER_UTILS
#define RENDER_UTILS

void iniciliarRenderVertexArray();
void finalizarRenderVertexArray();
void renderRectangulo2d(retangulo2d rec);
void renderRectangulo3d(retangulo3d rec);


#endif