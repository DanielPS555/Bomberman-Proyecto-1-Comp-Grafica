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
#include <thread>
#include "OpenGL-basico/bomb.h"
#include "OpenGL-basico/enemigo.h"
#include "OpenGL-basico/configuraciones.h"
#include "OpenGL-basico/Sistema movimiento/modoVisualizacion.h"
//carga obj
#include <Assimp/scene.h>
#include <Assimp/Importer.hpp>
#include <Assimp/postprocess.h>

using namespace std;

using Clock = std::chrono::steady_clock;
using this_thread::sleep_for; //ToDo Eliminar
using chrono::time_point;
using chrono::duration_cast;
using chrono::milliseconds;
using chrono::seconds;

bool fin = false;




int main(int argc, char *argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO)<0) {
		cerr << "No se pudo iniciar SDL: " << SDL_GetError() << endl;
		exit(1);
	}

	SDL_Window* win = SDL_CreateWindow("Bomberman - Obligatorio 1 - Comp Graf ",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		1600, 900, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	SDL_GLContext context = SDL_GL_CreateContext(win);


	glMatrixMode(GL_PROJECTION);

	float color = 0;
	glClearColor(color, color, color, 1);

	gluPerspective(45, 1600 / 900.f, 0.1, 1000);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);



	//FIN TEXTURA

	//----DECLARACION DE OBJETOS CREADOS------------

	
	
	// --------- Manejo y carga del mapa
	mapa* map = new mapa(11, 11);

	// --------- Datos Configuracion
	configuraciones* conf = configuraciones::getInstancia();


	// --------- Flags para el manejo de movimiento y Manejo de eventos

	SDL_Event evento;

	float deltaRotacionX = 0;
	float deltaRotacionY = 0;
	bool isRotando = false;

	bool isMoviendoArriba = false;
	bool isMoviendoAbajo = false;
	bool isMoviendoIsquierda = false;
	bool isMoviendoDerecha = false;

	// -------- Flags para manejo de la bomba

	bool ponerBomba = false;
	bool explotarBomba = false;
	bool timer = false;
	bool sePuso = false;

	// -------- Bombas
	bomba** bombs = new bomba*[4];
	for (int b = 0; b < 4; b++) {
		bombs[b] = nullptr;
	}
	float** victimas = nullptr;

	// -------- Jugador
	mathVector posAct = {0, 0, 0};
	int dirAct = 0;
	
	jugador* player = new jugador(map->obtenerPosicionInicialJugador(), map->anguloInicialJugador(), map);

	// --------- Configuracion de la camara


	modoVisualizacion* modoVis = new modoVisualizacion(player, MODOS_VISUALIZACION_PRIMERA_PERSONA);


	// -------- Manejo del tiempo

	//enemigo e = enemigo({ 0.f, 0.f, 0.f }, DERECHA, 2, 2, "assets/enemy2.jpg");
	//enemigo e2 = enemigo({ 0.f, 0.f, 0.f }, DERECHA, 4, 4, "assets/enemy.jpg");
	//enemigo e3 = enemigo({ 0.f, 0.f, 0.f }, DERECHA, 6, 6, "assets/enemy4.jpg");
	//enemigo e4 = enemigo({ 0.f, 0.f, 0.f }, DERECHA, 8, 8, "assets/enemigo3.jpg");


	time_point<Clock> beginLastFrame = Clock::now();
	milliseconds tiempoTranscurridoUltimoFrame;
	do {

		//Medir tiempo desde el ultimo frame hasta este
		tiempoTranscurridoUltimoFrame = duration_cast<milliseconds>(Clock::now() - beginLastFrame);
		float deltaTiempo = (float)tiempoTranscurridoUltimoFrame.count();
		beginLastFrame = Clock::now();
		
		// ---- Inicializar el frame
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		// ---- Preparar la camara
		modoVis->inicializarCamaraPorModo();

		// ---- Sistema de movimiento, debe ser lo ultimo que se haga
		if (isRotando) {
			player->rotarVerticalJugador(deltaRotacionY);
			player->rotarJugador(deltaRotacionX);
		}		
		player->trasladar(deltaTiempo, isMoviendoArriba, isMoviendoDerecha, isMoviendoAbajo, isMoviendoIsquierda);


		// ---- Aplicamos las configuraciones de rotacion y traslacion dependiendo del modo de camara
		modoVis->aplicarTranformacionesPorModo();
		

	
		//Manejo de la coloccacion de bombas
		if (ponerBomba) {
			int n = 0;
			while (n < 4 && ponerBomba) {
				if (bombs[n] == nullptr && ponerBomba) {
					posAct = player->getPosicionEnMapa();
					dirAct = round(player->getAnguloActualEnMapa());
					dirAct = dirAct % 360;
					bombs[n] = new bomba(posAct.y, posAct.x, 1, dirAct);
					sePuso = map->agregarBomba(bombs[n]->getXenMapa(), bombs[n]->getYenMapa());
					if (!sePuso) {
						delete bombs[n];
						bombs[n] = nullptr;
					}
					ponerBomba = false;
				}
				n++;
			}
			ponerBomba = false;
		}

		/*if (hayBomba && timer) {
			for (int i = 0; i < 4; i++) {
				if (bombs[i] != nullptr) {
					victimas = bombs[i]->explosion_trigg(victimas);
					if (victimas != nullptr) {
						map->eliminarDestructibles(victimas, 1);
						delete victimas;
						victimas = nullptr;
						delete bomb;
						bomb = nullptr;
						hayBomba = false;
					}
				}
			}
		}*/
		if (explotarBomba) {
			int i = 0;
			while (i < 4 && explotarBomba) {
				if (bombs[i] != nullptr) {
					victimas = bombs[i]->explosion_trigg(victimas);
					map->eliminarDestructibles(victimas, 1);
					delete victimas;
					victimas = nullptr;
					delete bombs[i];
					bombs[i] = nullptr;
					explotarBomba = false;
				}
				i++;
			}
			explotarBomba = false;
		}
		

	
		map->render();
		map->renderBombas(bombs);
		map->renderEnemigos(deltaTiempo,map);



		deltaRotacionX = 0.0f;
		deltaRotacionY = 0.0f;
	
		//MANEJO DE EVENTOS
		while (SDL_PollEvent(&evento)){
			switch (evento.type) {

			case SDL_MOUSEMOTION:
				deltaRotacionX = (-1.0f) * evento.motion.xrel + 0.0f;
				deltaRotacionY = (-1.0f) * evento.motion.yrel + 0.0f;

			case SDL_MOUSEBUTTONDOWN:

				switch (evento.button.button) {
				case SDL_BUTTON_LEFT:
					isRotando= true;
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (evento.button.button) {
				case SDL_BUTTON_LEFT:
					isRotando = false;
					break;
				}
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

				case SDLK_b:
					ponerBomba  = true;
					break;

				case SDLK_n:
					explotarBomba = true;
					break;
				case SDLK_v:
					modoVis->rotarCambioModo();
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
	free(conf);

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
