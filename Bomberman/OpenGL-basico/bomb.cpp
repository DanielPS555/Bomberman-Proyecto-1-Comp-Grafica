
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
#include "mapa.h"


bomba::bomba(float posXEnMapa, float posYEnMapa, float alcanze, float dirAct)
{
	int bx, by;
	if ((45 <= dirAct && dirAct < 135) || (-315 <= dirAct && dirAct < -225)) {
		// sumar x
		bx = floor(posXEnMapa / 25);
		by = floor(posYEnMapa / 25) - 1;
	}
	else {
		if ((135 <= dirAct && dirAct < 225) || (-225 <= dirAct && dirAct < -135)) {
			// restar y 
			bx = floor(posXEnMapa / 25) - 1;
			by = floor(posYEnMapa / 25);
		}
		else {
			if ((225 <= dirAct && dirAct < 315) || (-135 <= dirAct && dirAct < -45)) {
				// restar x
				bx = floor(posXEnMapa / 25);
				by = floor(posYEnMapa / 25) + 1;
			}
			else {
				if ((315 <= dirAct && dirAct <= 360) || (0 <= dirAct && dirAct < 45) || (-360 <= dirAct && dirAct <= -315) || (-45 <= dirAct && dirAct < 0)) {
					// sumar y
					bx = floor(posXEnMapa / 25) + 1;
					by = floor(posYEnMapa / 25);
				}
			}
		}
	}
	this->posXEnMapa = bx;
	this->posYEnMapa = by;

	this->alcanze = alcanze;

	vertice verticesBomba[8] = {
			{{by * LARGO_UNIDAD	+ (LARGO_UNIDAD / 3)		, bx * LARGO_UNIDAD + (LARGO_UNIDAD / 3)		, 0}			   , {1,1,1}},
			{{(by + 1) * LARGO_UNIDAD - (LARGO_UNIDAD / 3)	, bx * LARGO_UNIDAD + (LARGO_UNIDAD / 3)		, 0			    }  , {1,1,1}},
			{{(by + 1) * LARGO_UNIDAD - (LARGO_UNIDAD / 3)	, bx * LARGO_UNIDAD + (LARGO_UNIDAD / 3)		, ALTURA_PARED / 3}, {1.1,1,1}},
			{{by * LARGO_UNIDAD + (LARGO_UNIDAD / 3)		, bx * LARGO_UNIDAD + (LARGO_UNIDAD / 3)		, ALTURA_PARED / 3}, {1,1,1}},
			{{by * LARGO_UNIDAD + (LARGO_UNIDAD / 3)		, (bx + 1) * LARGO_UNIDAD - (LARGO_UNIDAD / 3)	, ALTURA_PARED / 3}, {1,1,1}},
			{{by * LARGO_UNIDAD + (LARGO_UNIDAD / 3)		, (bx + 1) * LARGO_UNIDAD - (LARGO_UNIDAD / 3)	, 0			    }  , {1,1,1}},
			{{(by + 1) * LARGO_UNIDAD - (LARGO_UNIDAD / 3)	, (bx + 1) * LARGO_UNIDAD - (LARGO_UNIDAD / 3)	, 0				}  , {1,1,1}},
			{{(by + 1) * LARGO_UNIDAD - (LARGO_UNIDAD / 3)	, (bx + 1) * LARGO_UNIDAD - (LARGO_UNIDAD / 3)	, ALTURA_PARED / 3}, {1,1,1}}
	};
	this->textura = inicializarTextura("assets/canon.png");
	this->vertices = createRetangulo3d(verticesBomba);
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

void bomba::render()
{
	glPushMatrix();

	iniciliarRenderVertexArray();

	renderRectangulo3d(this->vertices, this->textura);

	finalizarRenderVertexArray();

	glPopMatrix();
}

int bomba::getYenMapa()
{
	return this->posYEnMapa;
}

int bomba::getXenMapa()
{
	return this->posXEnMapa;
}

