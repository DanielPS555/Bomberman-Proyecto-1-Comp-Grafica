#include "modoVisualizacion.h"

modoVisualizacion::modoVisualizacion(jugador* ply, MODOS_VISUALIZACION modoPorDefecto) {
	player = ply;
	modoVisualizacionActual = modoPorDefecto;
}

void modoVisualizacion::inicializarCamaraPorModo() {
	switch (modoVisualizacionActual)
	{
	case MODOS_VISUALIZACION_PRIMERA_PERSONA:
		gluLookAt(0, 0, 0, 0, 0.1f, 0, 0, 0, 1);
		

		break;
	case MODOS_VISUALIZACION_VISTA_ORGINAL:
		gluLookAt(0, 0, 0, 0, 0, -0.1f, 0, 1, 0);


		break;
	default:
		break;
	}
}

void modoVisualizacion::aplicarTranformacionesPorModo() {

	mathVector posicionEnMapaJugador;

	switch (modoVisualizacionActual){
	case MODOS_VISUALIZACION_PRIMERA_PERSONA:
		glRotatef(-player->getAnguloActualVertical(), 1.0, 0.0, 0.0);
		glRotatef(-player->getAnguloActualEnMapa(), 0.0, 0.0, 1.0);

		posicionEnMapaJugador = player->getPosicionEnMapa();
		glTranslatef(-posicionEnMapaJugador.x, -posicionEnMapaJugador.y, -posicionEnMapaJugador.z);
		
		//En la vista primera persona, es importante que cuando la camara rota, lo haga teniendo el que centro de rotacion
		// Es la propia camara, es por eso que se coloca el mapa y resto de las cosas en la dirrecion contraria de donde deberia estar la camara
		glTranslatef(-UBICACION_CAMARA_PRIMERA_PERSONA.x, -UBICACION_CAMARA_PRIMERA_PERSONA.y, -UBICACION_CAMARA_PRIMERA_PERSONA.z);

		break;
	case MODOS_VISUALIZACION_VISTA_ORGINAL:
		glRotatef(-player->getAnguloActualEnMapa(), 0.0, 0.0, 1.0);

		posicionEnMapaJugador = player->getPosicionEnMapa();
		glTranslatef(-posicionEnMapaJugador.x, -posicionEnMapaJugador.y, -posicionEnMapaJugador.z);

		//En la vista primera persona, es importante que cuando la camara rota, lo haga teniendo el que centro de rotacion
		// Es la propia camara, es por eso que se coloca el mapa y resto de las cosas en la dirrecion contraria de donde deberia estar la camara
		glTranslatef(-UBICACION_CAMARA_VISTA_ORIGINAL.x, -UBICACION_CAMARA_VISTA_ORIGINAL.y, -UBICACION_CAMARA_VISTA_ORIGINAL.z);

		break;
	default:
		break;
	}
}

void modoVisualizacion::cambiarModo(MODOS_VISUALIZACION modo) {
	modoVisualizacionActual = modo;
}

void modoVisualizacion::rotarCambioModo() {
	switch (modoVisualizacionActual) {
	case MODOS_VISUALIZACION_PRIMERA_PERSONA:
		modoVisualizacionActual = MODOS_VISUALIZACION_VISTA_ORGINAL;
		break;
	case MODOS_VISUALIZACION_VISTA_ORGINAL:
		modoVisualizacionActual = MODOS_VISUALIZACION_PRIMERA_PERSONA;
		break;
	default:
		break;
	}
}
