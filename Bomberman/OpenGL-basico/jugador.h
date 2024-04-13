#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include "FreeImage.h"
#include <stdio.h>
#include <conio.h>
#include <GL/glu.h>

class jugador{
private:
	float posXEnMapa, posYEnMapa, posZEnMapa;
	float anguloActualEnMapa; //Angulo con respecto al eje x
public:
	jugador(float posXEnMapaInicial, float posYEnMapaInicial, float posZEnMapaInicial, float anguloInicial);

	// ToDo queda refactoriacion por realizar
	void trasladar(float deltaTiempo,
		bool isMoviendoArriba,
		bool isMoviendoDerecha,
		bool isMoviendoAbajo,
		bool isMoviendoIsquierda,
		float deltaAngulo);
		GLuint textura;

	void render();

	float getPosicionXEnMapa();

	float getPosicionYEnMapa();

	float getPosicionZEnMapa();

	float getAnguloActualEnMapa();

	

};

