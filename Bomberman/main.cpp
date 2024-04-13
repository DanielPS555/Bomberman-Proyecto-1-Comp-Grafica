#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include "FreeImage.h"
#include <stdio.h>
#include <conio.h>
#include <GL/glu.h>
#include "OpenGL-basico/mapa.h"
using namespace std;

bool fin = false;

int main(int argc, char *argv[]) {

	

	if (SDL_Init(SDL_INIT_VIDEO)<0) {
		cerr << "No se pudo iniciar SDL: " << SDL_GetError() << endl;
		exit(1);
	}

	SDL_Window* win = SDL_CreateWindow("Bomberman - Obligatorio 1 - Comp Graf ",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		1600, 900 , SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	SDL_GLContext context = SDL_GL_CreateContext(win);

	glMatrixMode(GL_PROJECTION);

	float color = 0;
	glClearColor(color, color, color, 1);

	gluPerspective(45, 1600 / 900.f, 0.1, 300);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	
	bool rotate = false;
	SDL_Event evento;

	mapa* map = new mapa(10, 10);

	float x, y, z;

	x = 0;
	y = 0;
	z = 10;

	bool isAdelanto = false;
	bool isRetroseso = false;

	float adelanto = 0.0;


	float degrees = 0;
	
	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		
		if (isAdelanto) {
			adelanto += 0.01;
		}

		if (isRetroseso) {
			adelanto -= 0.01;
		}
		

		//cout << isAdelanto << " " << isRetroseso << "\n";

		
		gluLookAt(x, y, z, 1, 1, 10, 0, 0, 1);

		if (rotate) {
			degrees = degrees + 0.01f;
		}
		glRotatef(degrees, 0.0, 0.0, 1.0);

		glTranslated(-adelanto, -adelanto, 0.0);

		glTranslatef(-25., -25.0, 0.);
		map->render();		

	
		//MANEJO DE EVENTOS
		while (SDL_PollEvent(&evento)){
			switch (evento.type) {
			case SDL_MOUSEBUTTONDOWN:
				rotate = true;
				break;
			case SDL_MOUSEBUTTONUP:
				rotate = false;
				break;
			case SDL_QUIT:
				fin = true;
				break;
			case SDL_KEYDOWN:
				switch (evento.key.keysym.sym) {
				case SDLK_ESCAPE:
					fin = true;
					break;
				case SDLK_RIGHT:
					break;
				case SDLK_UP:
				case SDLK_w:
					isAdelanto = true;
					break;
				case SDLK_DOWN:
				case SDLK_s:
					isRetroseso = true;
					break;
				}
				break;
			case SDL_KEYUP:
				switch (evento.key.keysym.sym) {
				case SDLK_UP:
				case SDLK_w:
					isAdelanto = false;
					break;
				case SDLK_DOWN:
				case SDLK_s:
					isRetroseso = false;
					break;
				}
			}
		}
		

		SDL_GL_SwapWindow(win);
	} while (!fin);
	//FIN LOOP PRINCIPAL
	// LIMPIEZA

	free(map);

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
