
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
	scale = 0;
	isAumentando = true;

	bool res = loadAssImp("assets/b.obj", indices, verticess, uvs, normals);
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

	this->life = 5000;
	this->alcanze = alcanze;

	this->textura = inicializarTextura("assets/b.png");
	//this->vertices = createRetangulo3d(verticesBomba);
}


bool bomba::timer(float deltaT)
{
	if (deltaT >= this->life) {
		return true;
	}
	else {
		this->life = this->life - deltaT;
		return false;
	}
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
		destruct[j + 3][1] = this->posYEnMapa + i + 1;
	}
	destruct[(alc * 4)] = new float[2];
	destruct[(alc * 4)][0] = this->posXEnMapa;
	destruct[(alc * 4)][1] = this->posYEnMapa;
	return destruct;
}

void bomba::render()
{

	glPushMatrix();
	//renderRectangulo3d(this->vertices, this->textura);

	glTranslatef(this->posYEnMapa * LARGO_UNIDAD + LARGO_UNIDAD/2, this->posXEnMapa * LARGO_UNIDAD + LARGO_UNIDAD/2, 0);
	glRotatef(90, 1, 0, 0);

	if (scale < 2 && isAumentando) {
		this->scale += 0.01;
	}
	else if (scale > 0 && !isAumentando) {
		this->scale -= 0.01;
	}
	else if (scale > 2 && isAumentando) {
		isAumentando = false;
	}
	else if (scale < 0 && !isAumentando) {
		isAumentando = true;
	}

	glScalef(scale + 4,scale + 4, scale + 4);
	render3dObject(verticess, uvs, normals, indices, textura);
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

int bomba::getAlcanze() {
	return this->alcanze;
}

bool bomba::danioBomba(mathVector posJugador, float** destruct)
{
	int j = 0;
	bool daño = false;
	int bx = floor(posJugador.x / LARGO_UNIDAD);
	int by = floor(posJugador.y / LARGO_UNIDAD);
	while ((j <= this->alcanze * 4) && !daño) {
		daño = (bx == destruct[j][1]) && (by == destruct[j][0]);
		j++;
	}
	return daño;
}

