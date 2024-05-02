
#include "SDL.h"
#include "SDL_opengl.h"
#include "renderUtils.h"
#include <iostream>
#include "FreeImage.h"
#include <stdio.h>
#include <conio.h>
#include <GL/glu.h>
#include "mathVector.h"

#ifndef BOMBA
#define BOMBA

class bomba {
	private:
		int posXEnMapa, posYEnMapa;
		float alcanze; //tama�o de la explocion
		retangulo3d vertices;
		GLuint textura; 
		float life;

	public:
		bomba(float posXEnMapa, float posYEnMapa, float alcanze, float dirAct);

		bool timer(float deltaT);
		float** explosion_trigg(float** destruct);
		void render();
		int getYenMapa();
		int getXenMapa();
		int getAlcanze();

		bool danioBomba(mathVector posJugador, float** destruct);
};

#endif