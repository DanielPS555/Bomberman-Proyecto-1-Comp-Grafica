#include "SDL_opengl.h"
#include <iostream>
#include "FreeImage.h"
#include <stdio.h>
#include <conio.h>
#include <GL/glu.h>
#include "SDL.h"
#include "mathVector.h"

#ifndef JUGADOR
#define JUGADOR

#define AVANCE_POR_SEGUNDO 50.0f
class jugador{
private:
	mathVector posicionEnMapa;
	float anguloActualEnMapa; //Angulo con respecto al eje y

	// Vectores de avance por segundo (1000 ms)
	const mathVector DIRRECION_SUPERIOR  = {  0.0f,  1.0f, 0.0f };
	const mathVector DIRRECION_INFERIOR  = {  0.0f, -1.0f, 0.0f };
	const mathVector DIRRECION_DERECHA   = {  1.0f,  0.0f, 0.0f };
	const mathVector DIRRECION_ISQUIERDA = { -1.0f,  0.0f, 0.0f };
public:
	jugador(mathVector posicionInicial, float anguloInicial);

	GLuint textura;

	~jugador();

	// ToDo queda refactoriacion por realizar
	void trasladar(float deltaTiempoMs ,
		bool isMoviendoArriba,
		bool isMoviendoDerecha,
		bool isMoviendoAbajo,
		bool isMoviendoIsquierda);
		
	void rotarJugador(float deltaRotacion);

	void render();

	mathVector getPosicionEnMapa();
	float getAnguloActualEnMapa();

	

};


#endif
