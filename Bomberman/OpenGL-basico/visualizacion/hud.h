#include "../renderUtils.h"


#ifndef HUD
#define HUD


class Hud{

private:
	float posicion_x;
	float posicion_y;
	float posicion_z;
	float width;
	float heigth;

	retangulo2d superfice;

public:
	Hud();
	
	void render();



};

#endif 