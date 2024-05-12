#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include "FreeImage.h"
#include <stdio.h>
#include <conio.h>
#include <GL/glu.h>
#include "bomberman/Models/mapa.h"
#include "bomberman/Models/jugador.h"
#include "chrono"
#include <thread>
#include "../../Utils/renderUtils.h"
#include "bomberman/Models/bomb.h"
#include "bomberman/Models/enemigo.h"
#include "bomberman/Configuraciones/configuraciones.h"
#include "bomberman/visualizacion/modoVisualizacion.h"
//carga obj
#include <Assimp/scene.h>
#include <Assimp/Importer.hpp>
#include <Assimp/postprocess.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "bomberman/Menus/menu.h"
#include "bomberman/Menus/MenuGameOverF.h"
#include "bomberman/Menus/MenuGameOverF.h"
#include "bomberman/Menus/MenuVictoriaF.h"
#include "bomberman/SistemaParticulas/particulas.h"
#include "bomberman/Models/explocion.h"
#include "bomberman/Utils/util.h"
#include "bomberman/Utils/objloader.h"
#include "bomberman/Utils/random.h"

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

	// Inicializar SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cerr << "Error al inicializar SDL_mixer: " << Mix_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	Mix_Chunk* efecto_explosion = Mix_LoadWAV("assets/sonido/explosion.mp3");
	Mix_Chunk* efecto_caminar = Mix_LoadWAV("assets/sonido/pasos.mp3");
	Mix_Chunk* efecto_muerte = Mix_LoadWAV("assets/sonido/muerte.mp3");
	Mix_Chunk* efecto_caballo = Mix_LoadWAV("assets/sonido/caballo.mp3");
	Mix_Chunk* efecto_caballo2 = Mix_LoadWAV("assets/sonido/caballo2.mp3");
	Mix_Chunk* efecto_caballo3 = Mix_LoadWAV("assets/sonido/caballo3.mp3");

	int caballo = 1;

	// Cargar el archivo de audio
	Mix_Music* ambiente_dia = Mix_LoadMUS("assets/sonido/ambiente_dia.mp3");
	Mix_Music* ambiente_nocturno = Mix_LoadMUS("assets/sonido/ambiente_nocturno.mp3");
	
	Mix_PlayMusic(ambiente_dia, -1);
	Mix_VolumeMusic(10);

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

	GameOverMenuF goMenu = GameOverMenuF(SCREEN_WIDTH, SCREEN_HEIGHT, renderer);

	MenuVictoriaF vicMenu = MenuVictoriaF(SCREEN_WIDTH, SCREEN_HEIGHT, renderer);

	glMatrixMode(GL_PROJECTION);

	float color = 0;
	glClearColor(color, color, color, 1);

	gluPerspective(45, SCREEN_WIDTH / SCREEN_HEIGHT, 0.1, 1000);

	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);

	//FIN TEXTURA
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals; // Won't be used at the moment.
	std::vector<unsigned short> indices;
	bool res = loadAssImp("assets/bomber.obj", indices, vertices, uvs, normals);

	GLuint texturaFuego = inicializarTexturaPng("assets/fire.png");
	GLuint texturaHumo = inicializarTexturaPng("assets/smoke.png");


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


	Uint32 intervalo = 10000; // 10 segundos en milisegundos
	Uint32 tiempoSiguienteReproduccion = SDL_GetTicks() + intervalo;

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
	bool isMoviendo = false;

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

	
	

	// -------- Jugador
	mathVector posAct = { 0, 0, 0 };
	int dirAct = 0;
	bool muerte = false;
	float invTime = 0;
	int puntaje = 0;

	jugador* player = new jugador(map->obtenerPosicionInicialJugador(), map->anguloInicialJugador(), map, 3);


	// --------- Configuracion de la camara
	Hud* hud = new Hud(renderer, win);
	modoVisualizacion* modoVis = new modoVisualizacion(player, hud, MODOS_VISUALIZACION_PRIMERA_PERSONA);

	map->setModoVisualizacion(modoVis);

	// -------- Exploxion y Particulas
	particleGenerator* partSist = new particleGenerator(10, 1.0, player, modoVis);
	explocion** explociones = new explocion * [4];
	for (int l = 0; l < 4; l++) {
		explociones[l] = nullptr;
	}

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


	//inicializarTexturaBomba();

	// -------- Manejo del tiempo

	time_point<Clock> beginLastFrame = Clock::now();
	milliseconds tiempoTranscurridoUltimoFrame;
	
	// Cargar imagen de fondo
	SDL_Surface* backgroundSurface = SDL_LoadBMP("assets/bomberman.bmp");
	if (!backgroundSurface) {
		cerr << "Error al cargar la imagen de fondo: " << SDL_GetError() << endl;
		TTF_Quit();
		SDL_Quit();
		return 1;
	}

	int cursorIndex = 0;
	

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
			glDisable(GL_BLEND);
			xmn = ((round(Random::Float() * 5) * 2) + 11);
			ymn = xmn;
			//ymn = (round(Random::Float() * 9) + 11);
			xPuerta = 1 + round(Random::Float() * (xmn - 2));
			yPuerta = 1 + round(Random::Float() * (ymn - 2));
			if((yPuerta % 2 == 1) && (xPuerta % 2 == 1)){
				yPuerta = yPuerta - 1;
			}
			int cantDes = 4 + round(Random::Float() * ((xmn * ymn) / 2));
			int cantEnem = 2 + round(Random::Float() * 2);
			map->newLevel(ymn, xmn, xPuerta, yPuerta, cantDes, cantEnem);
			player->gainVidas(3);
			player->restart(map->obtenerPosicionInicialJugador(), map->anguloInicialJugador());
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			newLvl = false;
			nivel = nivel + 1;
		}
		if (mostrar_menu) {
			isMoviendoAbajo = isMoviendoArriba = isMoviendoDerecha = isMoviendoIsquierda = false;

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

			if (configuraciones::getInstancia()->getModoIluminacion() == MODOS_ILUMINACION_NOCHE) {
				glClearColor(3.f / 256.f, 0.f / 256.f, 6.f / 256.f, 1);
			}
			else {
				glClearColor(243.f / 256.f, 187.f / 256.f, 75.f / 256.f, 1);
				//glClearColor(245.f / 256.f, 206.f / 256.f, 127.f / 256.f, 1);
			}
			
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_BLEND); //Enable blending.

			glEnable(GL_LIGHTING);
			

			glLoadIdentity();

			// --- Inicializar camara
			gluLookAt(0, 0, 0,	  0, 0, -0.1f,	0, 1, 0);

			
			// Luz ambiental en caso de modo noche
			if ( configuraciones::getInstancia()->getModoIluminacion() == MODOS_ILUMINACION_NOCHE) {
				glEnable(GL_LIGHT1);

				GLfloat light1color[] = { 7.0f / 255.f,	 15.0f / 255.f,	 43.0f / 255.f, 1.f };
				glLightfv(GL_LIGHT1, GL_DIFFUSE, light1color);


				glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.05f);
				glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.00f);
				glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0010f);

				


				GLfloat lightAmbientalcolor[] = { 7.0f / 255.f,	15.0f / 255.f,	43.f / 255.f	, 0.1f };// ;
				glMaterialfv(GL_FRONT, GL_AMBIENT, lightAmbientalcolor);

			}
			
			


			glPushMatrix();
			
			// ---- Ajustar la camara por modo de visualizacion
			modoVis->ajustarCamaraPorModoVisualizacion();
		


			// #### ----- Inicia el proceso de render del mapa y items sobre el
		

			// ---- Sistema de movimiento, debe ser lo ultimo que se haga

			if (isRotando) {
				if (modoVis->getModoVis() == MODOS_VISUALIZACION_PRIMERA_PERSONA) {
					player->rotarVerticalJugador(deltaRotacionY);
					player->rotarJugador(deltaRotacionX, -1);
				}
				else {
					player->rotarJugador(deltaRotacionX, 30);
				}
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
						Mix_PlayChannel(-1, efecto_explosion, 0);
						victimas = bombs[i]->explosion_trigg(victimas);
						puntaje = puntaje + map->eliminarDestructibles(victimas, bombs[i]->getAlcanze());
						muerte = bombs[i]->danioBomba(player->getPosicionEnMapa(), victimas);
						explocion* exp = new explocion(texturaFuego, texturaHumo, victimas);
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
					explocion* exp = new explocion(texturaFuego, texturaHumo, victimas);
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
				Mix_PlayChannel(-1, efecto_muerte, 0);
				player->restart(map->obtenerPosicionInicialJugador(), map->anguloInicialJugador());
			}else {
				Mix_PlayChannel(-1, efecto_muerte, 0);
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

	
		

		if (configuraciones::getInstancia()->getModoIluminacion() == MODOS_ILUMINACION_ATARDESER) {

			glEnable(GL_LIGHT0);
			glEnable(GL_LIGHT1);

			//GLfloat light1color[] = { 205.0f / 255.f,	16.0f / 255.f,		77.f / 255.f	, 0.1f };
			GLfloat light1color[] = { 252.0f / 255.f,	147.0f / 255.f,		119.f / 255.f	, 0.1f };
			glLightfv(GL_LIGHT1, GL_DIFFUSE, light1color);
			GLfloat light1colorSpecular[] = { 181.0f / 255.f,	27.0f / 255.f,		127.f / 255.f	, 0.1f };
			glLightfv(GL_LIGHT1, GL_SPECULAR, light1color);

			GLfloat posicion[] = { 10.f, 10.f, 10.f, 0.f };
			glLightfv(GL_LIGHT1, GL_POSITION, posicion);

			
			GLfloat light0Color[] = { 248.0f / 255.f, 208.0f / 255.f, 130.f / 255.f, 0.1f };
			glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Color);

			GLfloat posicion0[] = { -1.f, -1.f, 1.f, 0.f };
			glLightfv(GL_LIGHT0, GL_POSITION, posicion0);



			GLfloat lightAmbientalcolor[] = { 253.0f / 255.f,	132.0f / 255.f,		31.f / 255.f	, 0.1f };// ;
			glMaterialfv(GL_FRONT, GL_AMBIENT, lightAmbientalcolor);


		}

		map->renderEnemigos(deltaTiempo, map);

		partSist->timer(deltaTiempo);
		partSist->renderLight();

		map->render();
		map->renderPuerta();
		map->renderBombas(bombs);

		
		partSist->render();

		glPopMatrix();
		glDisable(GL_DEPTH_TEST);

		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT3);
		glDisable(GL_LIGHT4);
		glDisable(GL_LIGHT5);
		glDisable(GL_LIGHT7);

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

		while (SDL_PollEvent(&evento) && !hud->isPantallaMuerteActivada()) {
			
			
			if (mostrar_menu) {
				if (mostrar_menu && victoria) {
					victoria = vicMenu.eventHandler(evento);
					mostrar_menu = victoria;
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

				if (SDL_GetTicks() >= tiempoSiguienteReproduccion) {

					if (caballo == 1) {
						Mix_PlayChannel(0, efecto_caballo, 0);
					}
					else if (caballo == 2) {
						Mix_PlayChannel(0, efecto_caballo2, 0);
					}
					else {
						Mix_PlayChannel(0, efecto_caballo3, 0);
					}
					caballo = (caballo + 1) % 3;
					tiempoSiguienteReproduccion += intervalo; // Configurar el próximo tiempo de reproducción
				}

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
					case SDLK_q:
						fin = true;
						break;
					case SDLK_ESCAPE:
					case SDLK_p:
						mostrar_menu = !mostrar_menu;
						//Mix_PlayMusic(music, -1);
						SDL_GL_SwapWindow(win);
						break;

					case SDLK_UP:
					case SDLK_w:
						if (isMoviendo == false) {
							Mix_PlayChannel(2, efecto_caminar, -1);
						}
						isMoviendoArriba = true;
						isMoviendo = true;
						break;

					case SDLK_DOWN:
					case SDLK_s:
						if (isMoviendo == false) {
							Mix_PlayChannel(2, efecto_caminar, -1);
						}
						isMoviendoAbajo = true;
						isMoviendo = true;
						break;

					case SDLK_RIGHT:
					case SDLK_d:
						if (isMoviendo == false) {
							Mix_PlayChannel(2, efecto_caminar, -1);
						}
						isMoviendoDerecha = true;
						isMoviendo = true;
						break;

					case SDLK_LEFT:
					case SDLK_a:
						if (isMoviendo == false) {
							Mix_PlayChannel(2, efecto_caminar, -1);
						}
						isMoviendoIsquierda = true;
						isMoviendo = true;
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
						Mix_HaltChannel(2);
						isMoviendoArriba = false;
						isMoviendo = false;
						break;


					case SDLK_DOWN:
					case SDLK_s:
						Mix_HaltChannel(2);
						isMoviendoAbajo = false;
						isMoviendo = false;
						break;

					case SDLK_RIGHT:
					case SDLK_d:
						Mix_HaltChannel(2);
						isMoviendo = false;
						isMoviendoDerecha = false;
						break;

					case SDLK_LEFT:
					case SDLK_a:
						Mix_HaltChannel(2);
						isMoviendo = false;
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
