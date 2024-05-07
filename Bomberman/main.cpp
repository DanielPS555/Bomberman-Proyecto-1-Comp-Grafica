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
#include "OpenGL-basico/menuGameOver.h"
#include "OpenGL-basico/menuVictoria.h"
#include "OpenGL-basico/particulas.h"
#include "OpenGL-basico/explocion.h"
#include "OpenGL-basico/util.h"

using namespace std;

using Clock = std::chrono::steady_clock;
using this_thread::sleep_for; //ToDo Eliminar
using chrono::time_point;
using chrono::duration_cast;
using chrono::milliseconds;
using chrono::seconds;

bool fin = false;
bool mostrar_menu = true;

int main(int argc, char* argv[]) {


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
	SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	//SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE | SDL_RENDERER_PRESENTVSYNC);

	SDL_GL_MakeCurrent(win, context);

	menu mnu = menu(SCREEN_WIDTH, SCREEN_HEIGHT, renderer);

	gameOverMenu goMenu = gameOverMenu(SCREEN_WIDTH, SCREEN_HEIGHT, renderer);

	victoryMenu vicMenu = victoryMenu(SCREEN_WIDTH, SCREEN_HEIGHT, renderer);

	glMatrixMode(GL_PROJECTION);

	float color = 0;
	glClearColor(color, color, color, 1);

	gluPerspective(45, 1600 / 900.f, 0.1, 1000);

	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);

	//FIN TEXTURA
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals; // Won't be used at the moment.
	std::vector<unsigned short> indices;
	bool res = loadAssImp("assets/bomber.obj", indices, vertices, uvs, normals);


	//TEXTURA
	char* archivo = new char[20];
	archivo = "assets/bombertexture.png";

	//CARGAR IMAGEN
	FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(archivo);
	FIBITMAP* bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	int w = FreeImage_GetWidth(bitmap);
	int h = FreeImage_GetHeight(bitmap);
	void* datos = FreeImage_GetBits(bitmap);
	//FIN CARGAR IMAGEN

	GLuint textura;
	glGenTextures(1, &textura);
	glBindTexture(GL_TEXTURE_2D, textura);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, datos);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


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
	bomba** bombs = new bomba * [4];
	for (int b = 0; b < 4; b++) {
		bombs[b] = nullptr;
	}
	float** victimas = nullptr;

	// -------- Exploxion y Particulas
	particleGenerator* partSist = new particleGenerator(10, 1.0);
	explocion** explociones = new explocion * [4];
	for (int l = 0; l < 4; l++) {
		explociones[l] = nullptr;
	}

	// -------- Jugador
	mathVector posAct = { 0, 0, 0 };
	int dirAct = 0;
	bool muerte = false;
	float invTime = 0;
	int puntaje = 0;

	jugador* player = new jugador(map->obtenerPosicionInicialJugador(), map->anguloInicialJugador(), map, 3);

	// -------- Mapa

	int xmn = 0;
	int ymn = 0;
	int xPuerta = 0;
	int yPuerta = 0;

	// -------- Flags Juego

	bool victoria = false;
	bool gameOver = false;
	int nivel = 1;
	bool retry = false;
	bool newLvl = false;

	// --------- Configuracion de la camara

	Hud* hud = new Hud(renderer, win);
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


	//glEnable(GL_COLOR_MATERIAL);
	

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set blending function.
	do {
		if (retry) {
			glDisable(GL_BLEND);
			puntaje = 0;
			map->resetEnemies();
			map->resetDestructibles();
			player->gainVidas(3);
			player->restart(map->obtenerPosicionInicialJugador(), map->anguloInicialJugador());
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			retry = false;
		}
		if (newLvl) {
			/*glDisable(GL_BLEND);
			xmn = (floor(Random::Float() * 20) + 11);
			ymn = (floor(Random::Float() * 20) + 11);
			xPuerta = floor(Random::Float() * xmn);
			yPuerta = floor(Random::Float() * ymn);
			free(map);
			map = new mapa(ymn, xmn, yPuerta, xPuerta);
			player->gainVidas(3);
			player->restart(map->obtenerPosicionInicialJugador(), map->anguloInicialJugador());
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
			newLvl = false;
			fin = true;
		}
		if (mostrar_menu) {
			if (mostrar_menu && victoria) {
				vicMenu.render();
				SDL_RenderClear(renderer);
			}
			else {
				if (mostrar_menu && gameOver) {
					goMenu.render();
					SDL_RenderClear(renderer);
				}
				else {
					mnu.render();
					SDL_RenderClear(renderer);

				}
			}
		}else {
			
			//Medir tiempo desde el ultimo frame hasta este
			tiempoTranscurridoUltimoFrame = duration_cast<milliseconds>(Clock::now() - beginLastFrame);
			float deltaTiempoReal = (float)tiempoTranscurridoUltimoFrame.count(); //Tiempo usado para el temporizador
			float deltaTiempo = conf->getVelocidadJuego()* deltaTiempoReal;
			beginLastFrame = Clock::now();

			if (hud->isPantallaMuerteActivada()) {
				deltaTiempo = 0;
			}
			// ---- Inicializar el frame
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_BLEND); //Enable blending.

			glEnable(GL_LIGHTING);
			

			glLoadIdentity();

			// --- Inicializar camara
			gluLookAt(0, 0, 0,	  0, 0, -0.1f,	0, 1, 0);

			//GLfloat colorAmbiental[4] = { 7.0f / 255.f, 15.0f / 255.f, 43.f / 255.f, 1.f};
			
			
			glEnable(GL_LIGHT1);

			GLfloat light1color[] = { 7.0f /255.f,	 15.0f / 255.f,	 43.0f / 255.f, 1.f };
			glLightfv(GL_LIGHT1, GL_DIFFUSE, light1color);
			
			glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.5f);
			glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.00f);
			glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0015f);

			
			


			glPushMatrix();
			
			// ---- Ajustar la camara por modo de visualizacion
			modoVis->ajustarCamaraPorModoVisualizacion();
		


			// #### ----- Inicia el proceso de render del mapa y items sobre el
		

			// ---- Sistema de movimiento, debe ser lo ultimo que se haga
			if (isRotando && modoVis->getModoVis() == MODOS_VISUALIZACION_PRIMERA_PERSONA) {
				player->rotarVerticalJugador(deltaRotacionY);
				player->rotarJugador(deltaRotacionX);
			}
			player->trasladar(deltaTiempo, isMoviendoArriba, isMoviendoDerecha, isMoviendoAbajo, isMoviendoIsquierda);


			// ---- Aplicamos las configuraciones de rotacion y traslacion dependiendo del modo de camara
			modoVis->aplicarTranformacionesPorModo(deltaTiempoReal, isMoviendoArriba || isMoviendoDerecha || isMoviendoAbajo || isMoviendoIsquierda);

			if (modoVis->getModoVis() != MODOS_VISUALIZACION_PRIMERA_PERSONA) {
				player->render(isMoviendoArriba, isMoviendoAbajo, isMoviendoDerecha, isMoviendoIsquierda);
			}
			modoVis->aplicarTransformacionPorCamara();
		

	
			//Manejo de la coloccacion de bombas
			if (ponerBomba) {
				int n = 0;
				while (n < 4 && ponerBomba) {
					if (bombs[n] == nullptr && ponerBomba) {
						posAct = player->getPosicionEnMapa();
						if (modoVis->getModoVis() == MODOS_VISUALIZACION_VISTA_ORGINAL) {
							switch (player->getCara()) {
							case ARRIBA:
								dirAct = 0;
								break;
							case ABAJO:
								dirAct = 180;
								break;
							case IZQUIERDA:
								dirAct = 90;
								break;
							case DERECHA:
								dirAct = 270;
								break;
							}
						}
						else {
							dirAct = round(player->getAnguloActualEnMapa());
						}
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
						puntaje = puntaje + map->eliminarDestructibles(victimas, bombs[i]->getAlcanze());
						muerte = bombs[i]->danioBomba(player->getPosicionEnMapa(), victimas);
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
					puntaje = puntaje + map->eliminarDestructibles(victimas, bombs[i]->getAlcanze());
					muerte = bombs[i]->danioBomba(player->getPosicionEnMapa(), victimas);
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
		
		if (invTime > 0) {
			invTime = invTime - deltaTiempo;
		}

		if (!muerte) { 
			muerte = map->danioPorEnemigo(player->getPosicionEnMapa()); 
		}

		if (muerte && (invTime <= 0)) {
			player->recibirDanio();
			muerte = false;
			isMoviendoAbajo = false;
			isMoviendoArriba = false;
			isMoviendoDerecha = false;
			isMoviendoIsquierda = false;
			invTime = 5000;
			puntaje = puntaje - 3000;
			if (puntaje < 0) {
				puntaje = 0;
			}
			if (player->getVidas() == 0) {
				gameOver = true;
				mostrar_menu = true;
				player->restart(map->obtenerPosicionInicialJugador(), map->anguloInicialJugador());
			}else {
				player->restart(map->obtenerPosicionInicialJugador(), map->anguloInicialJugador());
				hud->activarPantallaMuerte();
			}
		}

		if (map->victoria(player->getPosicionEnMapa()) && !victoria) {
			puntaje = puntaje + 15000;
			victoria = true;
			mostrar_menu = true;
		}



		hud->setPuntosYLevel(puntaje, nivel);
		hud->setNumeroVidasRestantes(player->getVidas());
		hud->setNumeroEnemigosRestantes(map->cantEnemigosVivos());

	
		map->renderEnemigos(deltaTiempo, map);
		map->render();
		map->renderPuerta();
		map->renderBombas(bombs);
		
		partSist->timer(deltaTiempo);
		partSist->render();



		glPopMatrix();
		glDisable(GL_DEPTH_TEST);

		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT3);
		glDisable(GL_LIGHT4);
		glDisable(GL_LIGHT5);

		glEnable(GL_BLEND); 
		glClear(GL_DEPTH_BUFFER_BIT);
		hud->aumentoTiempo((long)deltaTiempoReal);
		modoVis->renderHud();
		deltaRotacionX = 0.0f;
		deltaRotacionY = 0.0f;

		milliseconds tiempoDuranteFrame = duration_cast<milliseconds>(Clock::now() - beginLastFrame);
		//cout << tiempoDuranteFrame.count() << "ms \n";
		if (tiempoDuranteFrame < milliseconds(2)) {
			sleep_for(2ms);
		}


		SDL_GL_SwapWindow(win);
			
		}

		//----------MANEJO DE EVENTOS-------------

		while (SDL_PollEvent(&evento) && !hud->isPantallaMuerteActivada()) {
			
			
			if (mostrar_menu) {
				if (mostrar_menu && victoria) {
					mostrar_menu = vicMenu.eventHandler(evento);
					victoria = mostrar_menu;
					beginLastFrame = Clock::now();
					fin = vicMenu.isFinal();
					newLvl = vicMenu.weFight();
				}
				else {
					if (mostrar_menu && gameOver) {
						gameOver = goMenu.eventHandler(evento);
						beginLastFrame = Clock::now();
						fin = goMenu.isFinal();
						retry = goMenu.weResist();
					}
					else {
						mostrar_menu = mnu.eventHandler(evento);
						beginLastFrame = Clock::now();
						fin = mnu.isFinal();
					}
				}
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
						if (modoVis->getModoVis() == MODOS_VISUALIZACION_VISTA_ORGINAL) {
							player->restart(player->getPosicionEnMapa(), 0);
						}
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
