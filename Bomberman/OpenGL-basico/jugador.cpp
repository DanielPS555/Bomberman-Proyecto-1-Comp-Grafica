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
jugador::jugador(mathVector posicionInicial, float anguloInicial, mapa* m, int vidas) {

	conf = configuraciones::getInstancia();

	map = m;

	posicionEnMapa = posicionInicial;

	anguloActualEnMapa = anguloInicial;

	anguloActualVertical = 0.0f;

	this->textura = inicializarTextura("assets/ladrillo.jpg");

	this->vidas = vidas;

}


mathVector jugador::getPosicionEnMapa() {
	return posicionEnMapa;
}

float jugador::getAnguloActualEnMapa() {
	return anguloActualEnMapa;
}

mathVector jugador::corregirNuevaPosicionPorColicion(mathVector posicionActual, mathVector posicionNueva) {

	bool colicionSuperior = false;
	bool colicionInferior = false;
	bool colicionDerecha = false;
	bool colicionIsquierda = false;

	// Rodeo a la nueva posicion de un cuadrado de centro "Nueva posicion" y largo MARGEN_SEGURIDAD_COLICION, en base a eso busco si hay colicion teniendo un marge de seguridad para no llegar a atrabezar las paredes
	map->isColicion(posicionActual, sumar(posicionNueva, {  MARGEN_SEGURIDAD_COLICION,  MARGEN_SEGURIDAD_COLICION ,0.0f })
		, colicionSuperior, colicionDerecha, colicionInferior, colicionIsquierda);
	map->isColicion(posicionActual, sumar(posicionNueva, { -MARGEN_SEGURIDAD_COLICION,  MARGEN_SEGURIDAD_COLICION ,0.0f })
		, colicionSuperior, colicionDerecha, colicionInferior, colicionIsquierda);
	map->isColicion(posicionActual, sumar(posicionNueva, {  MARGEN_SEGURIDAD_COLICION, -MARGEN_SEGURIDAD_COLICION ,0.0f })
		, colicionSuperior, colicionDerecha, colicionInferior, colicionIsquierda);
	map->isColicion(posicionActual, sumar(posicionNueva, { -MARGEN_SEGURIDAD_COLICION, -MARGEN_SEGURIDAD_COLICION ,0.0f })
		, colicionSuperior, colicionDerecha, colicionInferior, colicionIsquierda);

	 
	mathVector verticeInferiorIsquierdoCelda = {0.0f, 0.0f, 0.0f};
	float ancho, altura;

	mathVector posicionCorregida = posicionNueva;

	map->getCordenadasCelda(posicionActual, verticeInferiorIsquierdoCelda, ancho, altura);

	if (colicionSuperior) {
		posicionCorregida.y = verticeInferiorIsquierdoCelda.y + altura - MARGEN_SEGURIDAD_COLICION;
	}

	if (colicionInferior) {
		posicionCorregida.y = verticeInferiorIsquierdoCelda.y + MARGEN_SEGURIDAD_COLICION;
	}

	if (colicionDerecha) {
		posicionCorregida.x = verticeInferiorIsquierdoCelda.x + ancho -  MARGEN_SEGURIDAD_COLICION;
	}

	if (colicionIsquierda) {
		posicionCorregida.x = verticeInferiorIsquierdoCelda.x + MARGEN_SEGURIDAD_COLICION;
	}

	return posicionCorregida;



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

		posicionEnMapa = corregirNuevaPosicionPorColicion(posicionEnMapa, resultante);
		
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

void jugador::recibirDanio()
{
	this->vidas = this->vidas - 1;
}

int jugador::getVidas()
{
	return this->vidas;
}

void jugador::restart(mathVector posicionInicial, float anguloInicial)
{
	posicionEnMapa = posicionInicial;

	anguloActualEnMapa = anguloInicial;

	anguloActualVertical = 0.0f;
}


jugador::~jugador() {
	free(conf);
}