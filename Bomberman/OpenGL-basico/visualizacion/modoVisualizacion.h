#include "../jugador.h"
#include "hud.h"

#ifndef MODO_VISUALZIACION
#define MODO_VISUALZIACION

enum MODOS_VISUALIZACION {
	MODOS_VISUALIZACION_PRIMERA_PERSONA,
	MODOS_VISUALIZACION_VISTA_ORGINAL
};

class modoVisualizacion {

private:

	const mathVector UBICACION_CAMARA_PRIMERA_PERSONA = { 0.0f, 0.0f, 13.0f };
	const mathVector UBICACION_CAMARA_VISTA_ORIGINAL = { 0.0f, 0.0f, 150.0f };

	//Tenemos los datos del jugador para poder aplciarlos a la camara
	jugador* player;
	MODOS_VISUALIZACION modoVisualizacionActual;
	Hud* hud_player; 

public:	
	modoVisualizacion(jugador* ply, Hud* hud, MODOS_VISUALIZACION modoPorDefecto);

	void ajustarCamaraPorModoVisualizacion();

	void aplicarTranformacionesPorModo();

	void renderHud();

	void cambiarModo(MODOS_VISUALIZACION);

	void rotarCambioModo();

	void aplicarTransformacionPorCamara();

	MODOS_VISUALIZACION getModoVis();

};


#endif






