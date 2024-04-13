#include "jugador.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include "FreeImage.h"
#include <stdio.h>
#include <conio.h>
#include <GL/glu.h>
#include "util.h"
jugador::jugador(float posXEnMapaInicial, 
	float posYEnMapaInicial, 
	float posZEnMapaInicial, 
	float anguloInicial) {

	posXEnMapa = posXEnMapaInicial;
	posYEnMapa = posYEnMapaInicial;
	posZEnMapa = posZEnMapaInicial;
	anguloActualEnMapa = anguloInicial;

	this->textura = inicializarTextura("assets/ladrillo.jpg");

}


float jugador::getPosicionXEnMapa() {
	return posXEnMapa;
}

float jugador::getPosicionYEnMapa() {
	return posYEnMapa;
}

float jugador::getPosicionZEnMapa() {
	return posZEnMapa;
}

float jugador::getAnguloActualEnMapa() {
	return anguloActualEnMapa;
}

void jugador::trasladar(float deltaTiempo,
	bool isMoviendoArriba,
	bool isMoviendoDerecha,
	bool isMoviendoAbajo,
	bool isMoviendoIsquierda,
	float deltaAngulo) {

	//e


}

void jugador::render() {
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textura);
		glBegin(GL_TRIANGLES);
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord2f(0, 0);
		glVertex3f(0., 0., 0.);
		glTexCoord2f(0, 1);
		glVertex3f(25., 0., 0.);
		glTexCoord2f(1, 0);
		glVertex3f(25., 25., 0.);
		glEnd();
		glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}