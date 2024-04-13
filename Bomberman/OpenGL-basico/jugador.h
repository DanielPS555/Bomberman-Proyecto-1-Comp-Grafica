#include "mathVector.h"

#ifndef JUGADOR
#define JUGADOR



#define AVANCE_POR_SEGUNDO 50.0f

class jugador{
private:
	mathVector posicionEnMapa;
	float anguloActualEnMapa; //Angulo con respecto al eje x

	// Vectores de avance por segundo (1000 ms)
	const mathVector DIRRECION_SUPERIOR  = {  0.0f,  1.0f, 0.0f };
	const mathVector DIRRECION_INFERIOR  = {  0.0f, -1.0f, 0.0f };
	const mathVector DIRRECION_DERECHA   = {  1.0f,  0.0f, 0.0f };
	const mathVector DIRRECION_ISQUIERDA = { -1.0f,  0.0f, 0.0f };
public:
	jugador(mathVector posicionInicial, float anguloInicial);

	~jugador();

	// ToDo queda refactoriacion por realizar
	void trasladar(float deltaTiempoMs ,
		bool isMoviendoArriba,
		bool isMoviendoDerecha,
		bool isMoviendoAbajo,
		bool isMoviendoIsquierda);


	mathVector getPosicionEnMapa();
	float getAnguloActualEnMapa();

};


#endif
