#include "../renderUtils.h"
#include "../configuraciones.h"
#include "../util.h"
#include <SDL_ttf.h>


#ifndef HUD
#define HUD

#define PROFUNDIDAD_HUD 300.0f


class Hud{

private:
	float posicion_z;
	float width;
	float heigth;

	SDL_Renderer* renderer;

	TTF_Font* fuente_tiempo;


	void renderCajaSupIsq();
	void renderCajaSupCentro();
	void renderCajaSupDer();
	void renderTextTiempo();
	

public:
	Hud(SDL_Renderer* r);
	
	void render();

	~Hud();

};

#endif 