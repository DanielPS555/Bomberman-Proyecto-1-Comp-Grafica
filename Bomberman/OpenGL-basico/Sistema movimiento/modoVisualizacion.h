#include "../jugador.h"

#ifndef MODO_VISUALZIACION
#define MODO_VISUALZIACION

enum MODOS_VISUALIZACION {
	MODOS_VISUALIZACION_PRIMERA_PERSONA,
	MODOS_VISUALIZACION_VISTA_ORGINAL
};

class modoVisualizacion {

private:

	const mathVector UBICACION_CAMARA_PRIMERA_PERSONA = { 0.0f, 0.0f, 13.0f };

	//Tenemos los datos del jugador para poder aplciarlos a la camara
	jugador* player;
	MODOS_VISUALIZACION modoVisualizacionActual;

public:	
	modoVisualizacion(jugador* ply, MODOS_VISUALIZACION modoPorDefecto);

	void inicializarCamaraPorModo();

	void aplicarTranformacionesPorModo();

	void cambiarModo(MODOS_VISUALIZACION);


};


#endif






