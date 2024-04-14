#include "jugador.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include "FreeImage.h"
#include <stdio.h>
#include <conio.h>
#include <GL/glu.h>
#include "util.h"
#include <iostream>
jugador::jugador(mathVector posicionInicial, float anguloInicial) {

	posicionEnMapa = posicionInicial;
	anguloActualEnMapa = anguloInicial;

	this->textura = inicializarTextura("assets/ladrillo.jpg");

}


mathVector jugador::getPosicionEnMapa() {
	return posicionEnMapa;
}

float jugador::getAnguloActualEnMapa() {
	return anguloActualEnMapa;
}

void jugador::trasladar(float deltaTiempoMs,
	bool isMoviendoArriba,
	bool isMoviendoDerecha,
	bool isMoviendoAbajo,
	bool isMoviendoIsquierda) {

	
	mathVector resultante = { 0.f, 0.f, 0.f };
	
	if (isMoviendoArriba) {
		resultante = sumar(resultante, DIRRECION_SUPERIOR);
	}

	if (isMoviendoAbajo) {
		resultante = sumar(resultante, DIRRECION_INFERIOR);
	}

	if (isMoviendoDerecha) {
		resultante = sumar(resultante, DIRRECION_DERECHA);
	}

	if (isMoviendoIsquierda) {
		resultante = sumar(resultante, DIRRECION_ISQUIERDA);
	}
	
	if (!isNulo(resultante)) {
		std::cout << "movimiento " << deltaTiempoMs <<  std::endl;

		resultante = normalizar(resultante);

		resultante = multiplicarPorEscalar(resultante,  AVANCE_POR_SEGUNDO *  deltaTiempoMs / (1000));

		posicionEnMapa = sumar(posicionEnMapa, resultante);
	}
	
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


jugador::~jugador() {

}