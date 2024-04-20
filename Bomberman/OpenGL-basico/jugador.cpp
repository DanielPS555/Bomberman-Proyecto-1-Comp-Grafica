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
jugador::jugador(mathVector posicionInicial, float anguloInicial, mapa* m) {

	conf = configuraciones::getInstancia();

	map = m;

	posicionEnMapa = posicionInicial;

	anguloActualEnMapa = anguloInicial;

	anguloActualVertical = 0.0f;

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
		resultante = normalizar(resultante);
		resultante = rotar(resultante, anguloActualEnMapa);
		resultante = multiplicarPorEscalar(resultante,  AVANCE_POR_SEGUNDO *  deltaTiempoMs / (1000));
		resultante = sumar(posicionEnMapa, resultante);

		

		if ( map->isTraslacionValida(posicionEnMapa, resultante) ) {
			posicionEnMapa = resultante;
		}

		
	}
	
}

void jugador::rotarJugador(float deltaRotacion) {
	if (conf->getIsCamaraHorizontalInvertida()) {
		deltaRotacion *= -1;
	}
	anguloActualEnMapa += deltaRotacion * conf->getSensibilidadCamara();

	if (anguloActualEnMapa > 360) {
		anguloActualEnMapa -= 360;
	}

	if (anguloActualEnMapa < 0) {
		anguloActualEnMapa += 360;
	}

	std::cout << anguloActualEnMapa << std::endl;
}

void jugador::rotarVerticalJugador(float deltaVerticalRotacion) {
	anguloActualVertical += deltaVerticalRotacion * conf->getSensibilidadCamara();
	if (anguloActualVertical > 90.0f) {
		anguloActualVertical = 90.0f;
	}
	else if (anguloActualVertical < -90.0f) {
		anguloActualVertical = -90.0f;
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

float jugador::getAnguloActualVertical() {
	return anguloActualVertical;
}


jugador::~jugador() {
	free(conf);
}