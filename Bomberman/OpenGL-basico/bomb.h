
#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include "FreeImage.h"
#include <stdio.h>
#include <conio.h>
#include <GL/glu.h>

#ifndef BOMBA
#define BOMBA

class bomba {
	private:
		float posXEnMapa, posYEnMapa;
		float alcanze; //tamaño de la explocion

	public:
		bomba(float posXEnMapa, float posYEnMapa, float alcanze);

		float** explosion_trigg(float** destruct);
};

#endif