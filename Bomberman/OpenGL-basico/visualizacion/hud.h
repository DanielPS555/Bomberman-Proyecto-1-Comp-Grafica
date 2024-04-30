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

	unsigned long millSec;

	SDL_Renderer* renderer;

	TTF_Font* fuente;

	void renderText(float resizeFont, std::string texto, float centroX, float centroY);

	void renderCajaSupIsq();
	void renderCajaSupCentro();
	void renderCajaSupDer();
	void renderTextTiempo();

	GLuint idTexturaTiempo;
	

public:
	Hud(SDL_Renderer* r);
	
	void render();

	void aumentoTiempo(long deltaAumento);

	~Hud();

};

#endif 