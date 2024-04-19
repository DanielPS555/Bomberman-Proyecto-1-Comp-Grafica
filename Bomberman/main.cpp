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
		1600, 900 , SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
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


	// --------- Configuracion de la camara
	//ToDo: Poner en una clase propia, de forma que hay se puedan tener los modos de vista aparte 

	float posicion_camara_x = 0; 
	float posicion_camara_y = -2;
	float posicion_camara_z = 13;


	// --------- Flags para el manejo de movimiento y Manejo de eventos

	SDL_Event evento;

	float deltaRotacionX = 0;
	float deltaRotacionY = 0;
	bool isRotando = false;

	bool isMoviendoArriba = false;
	bool isMoviendoAbajo = false;
	bool isMoviendoIsquierda = false;
	bool isMoviendoDerecha= false;

	// -------- Flags para manejo de la bomba

	bool ponerBomba = false;
	bool explotarBomba = false;
	bool hayBomba = false;
	bool timer = false;
	
	// -------- Bombas
	time_point<Clock> bombTime = Clock::now();
	bomba* bomb = nullptr;
	float** victimas = nullptr;
	
	// -------- Jugador
	
	
	jugador* player = new jugador(map->obtenerPosicionInicialJugador(), map->anguloInicialJugador());

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
		
		//Inicializar el frame
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		//Preparar la camara
		gluLookAt(0, 0, 0, 0, 2, 0, 0, 0, 1);


		

		
		
		
		
		
		
		
		
		
		
		
		
		
		// -- Sistema de movimiento, debe ser lo ultimo que se haga

		if (isRotando) {
			player->rotarVerticalJugador(deltaRotacionY);
			player->rotarJugador(deltaRotacionX);
		}		

		player->trasladar(deltaTiempo, isMoviendoArriba, isMoviendoDerecha, isMoviendoAbajo, isMoviendoIsquierda);


		glRotatef(-player->getAnguloActualVertical(), 1.0, 0.0, 0.0);
		glRotatef(-player->getAnguloActualEnMapa(), 0.0, 0.0, 1.0);

		mathVector posicionEnMapaJugador = player->getPosicionEnMapa();
		glTranslatef(-posicionEnMapaJugador.x, -posicionEnMapaJugador.y, -posicionEnMapaJugador.z);	
		//En la vista primera persona, es importante que cuando la camara rota, lo haga teniendo el que centro de rotacion
		// Es la propia camara, es por eso que se coloca el mapa y resto de las cosas en la dirrecion contraria de donde deberia estar la camara
		glTranslatef(-posicion_camara_x, -posicion_camara_y, -posicion_camara_z);




		//Manejo de la coloccacion de bombas
		if (ponerBomba) {
			bomb = new bomba(4.0, 4.0, 1);
			hayBomba = map->agregarBomba(4.0, 4.0);
			bombTime = Clock::now() += seconds(5);
			ponerBomba = false;
		}

		if (hayBomba && timer && bombTime <= Clock::now()) {
			victimas = bomb->explosion_trigg(victimas);
			if (victimas != nullptr) {
				map->eliminarDestructibles(victimas, 1);
				delete victimas;
				victimas = nullptr;
				delete bomb;
				bomb = nullptr;
				hayBomba = false;
			}
		}
		if (hayBomba && explotarBomba) {
			victimas = bomb->explosion_trigg(victimas);
			map->eliminarDestructibles(victimas, 1);
			delete victimas;
			victimas = nullptr;
			delete bomb;
			bomb = nullptr;
			hayBomba = false;
			explotarBomba = false;
		}
		

	
		map->render();
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
