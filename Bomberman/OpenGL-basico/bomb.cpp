
#include "bomb.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include "FreeImage.h"
#include "chrono"
#include <stdio.h>
#include <conio.h>
#include <GL/glu.h>
#include "util.h"


bomba::bomba(float posXEnMapa, float posYEnMapa, float alcanze)
{
	this->posXEnMapa = posXEnMapa;
	this->posYEnMapa = posYEnMapa;
	this->alcanze = alcanze;
}


float** bomba::explosion_trigg(float** destruct)
{
	int alc = this->alcanze;
	destruct = new float* [(alc * 4) + 1];
	int j = 0;
	for (int i = 0; i < alc; i++) {
		j = i * 4;
		destruct[j] = new float[2];
		destruct[j][0] = this->posXEnMapa + i + 1;
		destruct[j][1] = this->posYEnMapa;
		destruct[j + 1] = new float[2];
		destruct[j + 1][0] = this->posXEnMapa - i - 1;
		destruct[j + 1][1] = this->posYEnMapa;
		destruct[j + 2] = new float[2];
		destruct[j + 2][0] = this->posXEnMapa;
		destruct[j + 2][1] = this->posYEnMapa - i - 1;
		destruct[j + 3] = new float[2];
		destruct[j + 3][0] = this->posXEnMapa;
		destruct[j + 3][1] = this->posYEnMapa + 1;
	}
	destruct[(alc * 4)] = new float[2];
	destruct[(alc * 4)][0] = this->posXEnMapa;
	destruct[(alc * 4)][1] = this->posYEnMapa;
	return destruct;
}

