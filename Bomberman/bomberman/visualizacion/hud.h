#include "../Utils/renderUtils.h"
#include "../Configuraciones/configuraciones.h"
#include "../Utils/util.h"
#include "../Utils/renderUtils.h"
#include "../shapes/Rectangulo2d.h"
#include <SDL_ttf.h>


#ifndef HUD
#define HUD

#define PROFUNDIDAD_HUD 300.0f

#define TIEMPO_ANIMACION_MUERTE 3000 // en ms
#define OPACIDAD_PANTALLA_MUERTE 90.0f

class Hud{

private:
	float posicion_z;
	float width;
	float heigth;

	unsigned long millSec;

	unsigned long puntos;
	unsigned int level;
	unsigned int numeroEnemigosRestantes;

	unsigned int numeroVidasRestantes;

	SDL_Renderer* renderer;
	SDL_Window* win;

	TTF_Font* fuente;

	GLuint textureIconoHeart;
	GLuint textureIconoEnemy;

	Rectangulo2d<1>* iconoHeart;
	Rectangulo2d<1>* iconoEnemy;


	void renderText(float resizeFont, std::string texto, float bordeSupIsqX, 
		float bordeSupIsqY, bool centrarAntesDeTrasladar, SDL_Color color);

	void renderTextConSombra(float resizeFont, std::string texto, float bordeSupIsqX,
		float bordeSupIsqY, bool centrarAntesDeTrasladar, SDL_Color color);

	void renderTextTiempo();	
	void renderCajaSupIsq();
	void renderCajaSupCentro();
	void renderCajaSupDer();
	
	void renderTextPuntosYLevel();
	void renderTextVidas();

	void renderPantallaMuerte();


	//Animaciones muerte
	unsigned long tiempoInicioAnimacionMuerte;
	bool modoMuerteActivado;

public:
	Hud(SDL_Renderer* r, SDL_Window * win);
	
	void render();

	void aumentoTiempo(long deltaAumento);

	void setPuntosYLevel(unsigned long p, unsigned int level);

	void setNumeroVidasRestantes(unsigned int numVidas);

	void setNumeroEnemigosRestantes(unsigned int numEnemigos);

	//Metodo sincronico, cuando termine la pantalla de muerte abra terminado y aceptado por parte del usuario.
	void activarPantallaMuerte();

	bool isPantallaMuerteActivada();

	~Hud();

};

#endif 