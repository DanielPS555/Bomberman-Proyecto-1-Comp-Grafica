#include "SDL_opengl.h"
#include <iostream>
#include "FreeImage.h"
#include <stdio.h>
#include <conio.h>
#include <GL/glu.h>
#include "SDL.h"
#include "../../Utils/mathVector.h"
#include "../../Configuraciones/configuraciones.h"
#include "mapa.h"
#include "../../Utils/util.h"

#ifndef JUGADOR
#define JUGADOR

#define AVANCE_POR_SEGUNDO 30.0f
#define MARGEN_SEGURIDAD_COLICION 0.5f //Distancia minima que puede tener el jugador y el objeto con el coliciona


class jugador{
private:

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals; // Won't be used at the moment.
	std::vector<unsigned short> indices;

	direccion cara;
	float paso1;
	GLuint textura;

	mathVector posicionEnMapa;

	mapa* map;

	float anguloActualEnMapa; //Angulo con respecto al eje y
	float anguloActualVertical;

	configuraciones* conf;

	int vidas;

	// Vectores de avance por segundo (1000 ms)
	const mathVector DIRRECION_SUPERIOR  = {  0.0f,  1.0f, 0.0f };
	const mathVector DIRRECION_INFERIOR  = {  0.0f, -1.0f, 0.0f };
	const mathVector DIRRECION_DERECHA   = {  1.0f,  0.0f, 0.0f };
	const mathVector DIRRECION_ISQUIERDA = { -1.0f,  0.0f, 0.0f };


	mathVector corregirNuevaPosicionPorColicion(mathVector posicionActual, mathVector posicionNueva);
public:
	jugador(mathVector posicionInicial, float anguloInicial, mapa* m, int vidas);


	~jugador();

	// ToDo queda refactoriacion por realizar
	void trasladar(float deltaTiempoMs ,
		bool isMoviendoArriba,
		bool isMoviendoDerecha,
		bool isMoviendoAbajo,
		bool isMoviendoIsquierda);
		
	void rotarJugador(float deltaRotacion, float cotaGrados);

	void rotarVerticalJugador(float deltaVerticalRotacion);


	mathVector getPosicionEnMapa();
	float getAnguloActualEnMapa();
	float getAnguloActualVertical();

	void recibirDanio();

	int getVidas();

	void gainVidas(int vidas);

	void restart(mathVector posicionInicial, float anguloInicial);

	void render(bool isMoviendoArriba, bool isMoviendoAbajo, bool isMoviendoDerecha, bool isMoviendoIsquierda);

	direccion getCara();
};


#endif
