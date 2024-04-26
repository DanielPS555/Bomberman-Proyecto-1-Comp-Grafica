#include "../renderUtils.h"
#include "../configuraciones.h"
#include "../util.h"


#ifndef HUD
#define HUD

#define PROFUNDIDAD_HUD 300.0f


class Hud{

private:
	float posicion_z;
	float width;
	float heigth;

	retanguloTransparente2d header;
	

public:
	Hud();
	
	void render();



};

#endif 