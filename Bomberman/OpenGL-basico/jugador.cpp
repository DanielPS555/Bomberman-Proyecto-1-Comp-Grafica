#include "jugador.h"

jugador::jugador(float posXEnMapaInicial, 
	float posYEnMapaInicial, 
	float posZEnMapaInicial, 
	float anguloInicial) {

	posXEnMapa = posXEnMapaInicial;
	posYEnMapa = posYEnMapaInicial;
	posZEnMapa = posZEnMapaInicial;
	anguloActualEnMapa = anguloInicial;

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


jugador::~jugador() {

}