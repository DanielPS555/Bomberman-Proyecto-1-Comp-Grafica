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
#include "OpenGL-basico/visualizacion/modoVisualizacion.h"
//carga obj
#include <Assimp/scene.h>
#include <Assimp/Importer.hpp>
#include <Assimp/postprocess.h>
#include <SDL_ttf.h>
#include "OpenGL-basico/menu.h"
#include "OpenGL-basico/particulas.h"
#include "OpenGL-basico/explocion.h"

using namespace std;

using Clock = std::chrono::steady_clock;
using this_thread::sleep_for; //ToDo Eliminar
using chrono::time_point;
using chrono::duration_cast;
using chrono::milliseconds;
using chrono::seconds;

bool fin = false;
bool mostrar_menu = true;

int main(int argc, char *argv[]) {
	
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		cerr << "No se pudo iniciar SDL: " << SDL_GetError() << endl;
		return 1;
	}

	if (TTF_Init() == -1) {
		std::cerr << "SDL_ttf no pudo inicializarse! SDL_ttf Error: " << TTF_GetError() << std::endl;
		SDL_Quit();
		return -1;
	}

	


	// Creaci�n de la ventana y el renderer de SDL
	SDL_Window* win = SDL_CreateWindow("Bomberman - Obligatorio 1 - Comp Graf ",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);


	SDL_GLContext context = SDL_GL_CreateContext(win);
	//SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
	SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	//SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE | SDL_RENDERER_PRESENTVSYNC);

	SDL_GL_MakeCurrent(win, context);

	menu mnu = menu(SCREEN_WIDTH, SCREEN_HEIGHT, renderer);

	glMatrixMode(GL_PROJECTION);

	float color = 0;
	glClearColor(color, color, color, 1);

	gluPerspective(45, 1600 / 900.f, 0.1, 1000);

	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);


	
	//FIN TEXTURA

	//----DECLARACION DE OBJETOS CREADOS------------

	
	// --------- Manejo y carga del mapa
	mapa* map = new mapa(11, 11, 8, 7);

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
	bool timer = true;
	bool sePuso = false;

	// -------- Bombas
	bomba** bombs = new bomba*[4];
	for (int b = 0; b < 4; b++) {
		bombs[b] = nullptr;
	}
	float** victimas = nullptr;

	// -------- Exploxion y Particulas
	particleGenerator* partSist = new particleGenerator(10, 1.0);
	explocion** explociones = new explocion*[4];
	for (int l = 0; l < 4; l++) {
		explociones[l] = nullptr;
	}

	// -------- Jugador
	mathVector posAct = {0, 0, 0};
	int dirAct = 0;
	bool muerte = false;

	jugador* player = new jugador(map->obtenerPosicionInicialJugador(), map->anguloInicialJugador(), map, 3);

	// --------- Configuracion de la camara

	Hud* hud = new Hud(renderer);
	modoVisualizacion* modoVis = new modoVisualizacion(player, hud, MODOS_VISUALIZACION_PRIMERA_PERSONA);


	// -------- Manejo del tiempo

	//enemigo e = enemigo({ 0.f, 0.f, 0.f }, DERECHA, 2, 2, "assets/enemy2.jpg");
	//enemigo e2 = enemigo({ 0.f, 0.f, 0.f }, DERECHA, 4, 4, "assets/enemy.jpg");
	//enemigo e3 = enemigo({ 0.f, 0.f, 0.f }, DERECHA, 6, 6, "assets/enemy4.jpg");
	//enemigo e4 = enemigo({ 0.f, 0.f, 0.f }, DERECHA, 8, 8, "assets/enemigo3.jpg");


	time_point<Clock> beginLastFrame = Clock::now();
	milliseconds tiempoTranscurridoUltimoFrame;
	
	// Cargar imagen de fondo
	SDL_Surface* backgroundSurface = SDL_LoadBMP("bomberman.bmp");
	if (!backgroundSurface) {
		cerr << "Error al cargar la imagen de fondo: " << SDL_GetError() << endl;
		TTF_Quit();
		SDL_Quit();
		return 1;
	}

	int cursorIndex = 0;



	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set blending function.
	do {
		if (mostrar_menu) {

			mnu.render();	
			SDL_RenderClear(renderer);

		}else {
			//Medir tiempo desde el ultimo frame hasta este
			tiempoTranscurridoUltimoFrame = duration_cast<milliseconds>(Clock::now() - beginLastFrame);
			float deltaTiempoReal = (float)tiempoTranscurridoUltimoFrame.count(); //Tiempo usado para el temporizador
			float deltaTiempo = conf->getVelocidadJuego()* deltaTiempoReal;
			beginLastFrame = Clock::now();
			// ---- Inicializar el frame
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_BLEND); //Enable blending.
		
			glLoadIdentity();

			// --- Inicializar camara
			gluLookAt(0, 0, 0, 0, 0, -0.1f, 0, 1, 0);



			glPushMatrix();
			// ---- Ajustar la camara por modo de visualizacion
			modoVis->ajustarCamaraPorModoVisualizacion();
		


			// #### ----- Inicia el proceso de render del mapa y items sobre el
		

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

      
		if (timer) {
			for (int i = 0; i < 4; i++) {
				if (bombs[i] != nullptr) {
					if(bombs[i]->timer(deltaTiempo)){
						victimas = bombs[i]->explosion_trigg(victimas);
						map->eliminarDestructibles(victimas, bombs[i]->getAlcanze());
						muerte = bombs[i]->dañoBomba(player->getPosicionEnMapa(), victimas);
						explocion* exp = new explocion(2000, victimas);
						exp->generateExplocion(bombs[i]->getAlcanze(), partSist);
						int e = 0;
						while (explociones[e] != nullptr) {
							e++;
						}
						explociones[e] = exp;
						delete victimas;
						victimas = nullptr;
						delete bombs[i];
						bombs[i] = nullptr;
					}
					i++;
				}
				explotarBomba = false;
			}
		}

		if (explotarBomba && !timer) {
			int i = 0;
			while (i < 4 && explotarBomba) {
				if (bombs[i] != nullptr) {
					victimas = bombs[i]->explosion_trigg(victimas);
					map->eliminarDestructibles(victimas, bombs[i]->getAlcanze());
					muerte = bombs[i]->dañoBomba(player->getPosicionEnMapa(), victimas);
					explocion* exp = new explocion(2000, victimas);
					exp->generateExplocion(bombs[i]->getAlcanze(), partSist);
					int e = 0;
					while (explociones[e] != nullptr) {
						e++;
					}
					explociones[e] = exp;
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

		for (int j = 0; j < 4; j++) {
			if (explociones[j] != nullptr) {
				if (explociones[j]->timer(deltaTiempo)) {
					explocion* del = explociones[j];
					explociones[j] = nullptr;
					delete del;
				}
			}
		}
		
		if (!muerte) {
			muerte = map->dañoPorEnemigo(player->getPosicionEnMapa());
		}

		if (muerte) {
			player->recibirDaño();
		}

	
		map->render();
		map->renderPuerta();
		map->renderBombas(bombs);
		map->renderEnemigos(deltaTiempo,map);
		partSist->timer(deltaTiempo);
		partSist->render();



		glPopMatrix();
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND); 
			glClear(GL_DEPTH_BUFFER_BIT);
			hud->aumentoTiempo((long)deltaTiempoReal);
			modoVis->renderHud();
			deltaRotacionX = 0.0f;
			deltaRotacionY = 0.0f;

			milliseconds tiempoDuranteFrame = duration_cast<milliseconds>(Clock::now() - beginLastFrame);
			if (tiempoDuranteFrame < milliseconds(2)) {
				sleep_for(2ms);
			}


			SDL_GL_SwapWindow(win);
			
		}

		//----------MANEJO DE EVENTOS-------------

		while (SDL_PollEvent(&evento)) {
			
			
			if (mostrar_menu) {
				mostrar_menu = mnu.eventHandler(evento);
				beginLastFrame = Clock::now();
			}
			else {
				switch (evento.type) {
				case SDL_MOUSEMOTION:
					deltaRotacionX = (-1.0f) * evento.motion.xrel + 0.0f;
					deltaRotacionY = (-1.0f) * evento.motion.yrel + 0.0f;

				case SDL_MOUSEBUTTONDOWN:
					switch (evento.button.button) {
					case SDL_BUTTON_LEFT:
						isRotando = true;
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
						mostrar_menu = !mostrar_menu;
						SDL_GL_SwapWindow(win);
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

					case SDLK_n:
						explotarBomba = true;
						break;

					case SDLK_b:
						ponerBomba = true;
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
		}
	} while (!fin);
	//FIN LOOP PRINCIPAL
	// LIMPIEZA

	free(map);
	free(player);
	free(hud);
	free(conf);
	SDL_DestroyRenderer(renderer);
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
