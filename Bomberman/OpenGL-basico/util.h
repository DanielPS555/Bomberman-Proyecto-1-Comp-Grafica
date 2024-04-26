
#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include "FreeImage.h"
#include <stdio.h>
#include <conio.h>
#include <GL/glu.h>

#ifndef UTILS
#define UTILS

GLuint inicializarTextura(char * archivo);
GLuint inicializarTexturaPng(char* archivo);


#endif // !UTILS