#include "configuraciones.h"



configuraciones::configuraciones() {
	isCamaraHorizontalInvertida = CAMARA_HORIZONTAL_INVERTIDA;
	isCamaraVerticalInvertida = CAMARA_VERTICAL_INVERTIDA;

	sensibilidadCamara = SENSIBILIDAD_CAMARA;
	velocidadJuego = VELOCIDAD_JUEGO;
}

configuraciones::~configuraciones(){}

bool configuraciones::getIsCamaraHorizontalInvertida() {
	return isCamaraHorizontalInvertida;
}

bool configuraciones::getIsCamaraVerticalInvertida() {
	return isCamaraVerticalInvertida;
}

float configuraciones::getSensibilidadCamara() {
	return sensibilidadCamara;
}

void configuraciones::setIsCamaraHorizontalInvertida(bool isInvertida) {
	isCamaraHorizontalInvertida = isInvertida;
}

void configuraciones::setIsCamaraVerticalInvertida(bool isInvertida) {
	isCamaraVerticalInvertida = isInvertida;
}

void configuraciones::setSensibilidadCamaraa(float s) {
	sensibilidadCamara = s;
}

void configuraciones::setVelocidadJuego(float v) {
	velocidadJuego = v;
}

float configuraciones::getVelocidadJuego() {
	return velocidadJuego;
}