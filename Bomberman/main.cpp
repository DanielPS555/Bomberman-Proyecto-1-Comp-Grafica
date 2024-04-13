#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include "FreeImage.h"
#include <stdio.h>
#include <conio.h>
#include <GL/glu.h>
#include "OpenGL-basico/mapa.h"
#include "OpenGL-basico/jugador.h"
#include "chrono"
#include <thread> //ToDo Eliminar

using namespace std;

using Clock = std::chrono::steady_clock;
using this_thread::sleep_for; //ToDo Eliminar
using chrono::time_point;
using chrono::duration_cast;
using chrono::milliseconds;

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
	
	
	

	// --------- Manejo y carga del mapa
	mapa* map = new mapa(10, 10);


	// --------- Configuracion de la camara
	//ToDo: Poner en una clase propia, de forma que hay se puedan tener los modos de vista aparte 

	float x, y, z;
	x = 0;
	y = 0;
	z = 10;


	// --------- Flags para el manejo de movimiento y Manejo de eventos

	SDL_Event evento;

	bool rotate = false;

	bool isMoviendoArriba = false;
	bool isMoviendoAbajo = false;
	bool isMoviendoIsquierda = false;
	bool isMoviendoDerecha= false;
	
	
	// -------- Jugador
	
	
	jugador* player = new jugador(map->obtenerPosicionInicialJugador(),map->anguloInicialJugador());


	// -------- Manejo del tiempo

	time_point<Clock> beginLastFrame = Clock::now();
	milliseconds tiempoTranscurridoUltimoFrame;
	do {

		//Medir tiempo desde el ultimo frame hasta este
		tiempoTranscurridoUltimoFrame = duration_cast<milliseconds>(Clock::now() - beginLastFrame);
		float deltaTiempo = (float)tiempoTranscurridoUltimoFrame.count();
		std::cout << deltaTiempo << "ms" << std::endl;
		beginLastFrame = Clock::now();
		
		//Inicializar el frame
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		//Preparar la camara
		gluLookAt(x, y, z, 1, 1, 10, 0, 0, 1);


		// Realizar movimiendos por el ultimo frame y trasladar en el mapa
		player->trasladar(deltaTiempo, isMoviendoArriba, isMoviendoDerecha, isMoviendoAbajo, isMoviendoIsquierda);

		
		glRotatef(player->getAnguloActualEnMapa(), 0.0, 0.0, 1.0);
		mathVector posicionEnMapaJugador = player->getPosicionEnMapa();
		glTranslatef(-posicionEnMapaJugador.x, -posicionEnMapaJugador.y, -posicionEnMapaJugador.z);

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

				case SDLK_UP:
				case SDLK_w:
					isMoviendoArriba = true;
					break;

				case SDLK_DOWN:
				case SDLK_s:
					isMoviendoAbajo = true;
					break;

				case SDLK_RIGHT:
				case SDLK_d:
					isMoviendoDerecha = true;
					break;

				case SDLK_LEFT:
				case SDLK_a:
					isMoviendoIsquierda = true;
					break;
				}
				break;
			case SDL_KEYUP:
				switch (evento.key.keysym.sym) {
				case SDLK_UP:
				case SDLK_w:
					isMoviendoArriba = false;
					break;

				case SDLK_DOWN:
				case SDLK_s:
					isMoviendoAbajo = false;
					break;

				case SDLK_RIGHT:
				case SDLK_d:
					isMoviendoDerecha = false;
					break;

				case SDLK_LEFT:
				case SDLK_a:
					isMoviendoIsquierda = false;
					break;
				}
			}
		}
		

		milliseconds tiempoDuranteFrame = duration_cast<milliseconds>(Clock::now() - beginLastFrame);
		if (tiempoDuranteFrame < milliseconds(2)){
			sleep_for(2ms);
		}

		SDL_GL_SwapWindow(win);
	} while (!fin);
	//FIN LOOP PRINCIPAL
	// LIMPIEZA

	free(map);
	free(player);

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
