#include "jugador.h"
#include <iostream>

jugador::jugador(mathVector posicionInicial, float anguloInicial) {

	posicionEnMapa = posicionInicial;
	anguloActualEnMapa = anguloInicial;

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


jugador::~jugador() {

}