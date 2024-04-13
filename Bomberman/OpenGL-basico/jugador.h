#include "mathVector.h"

#ifndef JUGADOR
#define JUGADOR



#define AVANCE_POR_SEGUNDO 10.0f

class jugador{
private:
	mathVector posicionEnMapa;
	float anguloActualEnMapa; //Angulo con respecto al eje x

	// Vectores de avance por segundo (1000 ms)
	mathVector DIRRECION_SUPERIOR  = { 0.0f					,  AVANCE_POR_SEGUNDO	, 0.0f };
	mathVector DIRRECION_INFERIOR  = { 0.0f					, -AVANCE_POR_SEGUNDO	, 0.0f };
	mathVector DIRRECION_DERECHA   = { AVANCE_POR_SEGUNDO		, 0.0f					, 0.0f };
	mathVector DIRRECION_ISQUIERDA = { -AVANCE_POR_SEGUNDO	, 0.0f					, 0.0f };
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
