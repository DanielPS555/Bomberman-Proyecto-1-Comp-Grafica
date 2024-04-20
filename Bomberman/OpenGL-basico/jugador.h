#include "SDL_opengl.h"
#include <iostream>
#include "FreeImage.h"
#include <stdio.h>
#include <conio.h>
#include <GL/glu.h>
#include "SDL.h"
#include "mathVector.h"
#include "configuraciones.h"
#include "mapa.h"

#ifndef JUGADOR
#define JUGADOR

#define AVANCE_POR_SEGUNDO 50.0f
#define MARGEN_SEGURIDAD_COLICION 3.0f //Distancia minima que puede tener el jugador y el objeto con el coliciona
class jugador{
private:
	mathVector posicionEnMapa;

	mapa* map;

	float anguloActualEnMapa; //Angulo con respecto al eje y
	float anguloActualVertical;

	configuraciones* conf;

	// Vectores de avance por segundo (1000 ms)
	const mathVector DIRRECION_SUPERIOR  = {  0.0f,  1.0f, 0.0f };
	const mathVector DIRRECION_INFERIOR  = {  0.0f, -1.0f, 0.0f };
	const mathVector DIRRECION_DERECHA   = {  1.0f,  0.0f, 0.0f };
	const mathVector DIRRECION_ISQUIERDA = { -1.0f,  0.0f, 0.0f };
public:
	jugador(mathVector posicionInicial, float anguloInicial, mapa* m);

	GLuint textura;

	~jugador();

	// ToDo queda refactoriacion por realizar
	void trasladar(float deltaTiempoMs ,
		bool isMoviendoArriba,
		bool isMoviendoDerecha,
		bool isMoviendoAbajo,
		bool isMoviendoIsquierda);
		
	void rotarJugador(float deltaRotacion);

	void rotarVerticalJugador(float deltaVerticalRotacion);

	void render();

	mathVector getPosicionEnMapa();
	float getAnguloActualEnMapa();
	float getAnguloActualVertical();

	

};


#endif
