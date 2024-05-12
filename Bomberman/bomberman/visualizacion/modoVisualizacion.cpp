#include "modoVisualizacion.h"

modoVisualizacion::modoVisualizacion(jugador* ply, Hud* hud, MODOS_VISUALIZACION modoPorDefecto) {
	player = ply;
	modoVisualizacionActual = modoPorDefecto;
	hud_player = hud;
}

void modoVisualizacion::ajustarCamaraPorModoVisualizacion() {
	switch (modoVisualizacionActual)
	{
	case MODOS_VISUALIZACION_PRIMERA_PERSONA:
		//gluLookAt(0, 0, 0, 0, 0.1f, 0, 0, 0, 1);

		glRotatef(270, 1, 0, 0);


		break;
	case MODOS_VISUALIZACION_VISTA_ORGINAL:
		gluLookAt(0, 0, 0, 0, 0, -0.1f, 0, 1, 0);


		break;
	default:
		break;
	}
}

void modoVisualizacion::aplicarTranformacionesPorModo(float deltaTiempo, bool estaMoviendose) {

	if (estaMoviendose) {
		tiempo += deltaTiempo;
	}
	

	GLfloat light1PosPrimeraPersona[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat light1PosVistaOriginal[] = { 0.0f, 0.0f, -140.0f, 1.0f };

	int altura = 0;

	switch (modoVisualizacionActual) {
	case MODOS_VISUALIZACION_PRIMERA_PERSONA:


		if (configuraciones::getInstancia()->getModoIluminacion() == MODOS_ILUMINACION_NOCHE) {
			glLightfv(GL_LIGHT1, GL_POSITION, light1PosPrimeraPersona);
		}
		

		glRotatef(-player->getAnguloActualVertical(), 1.0, 0.0, 0.0);

		
		glTranslatef(0.0f, 0.0f, cos((float)tiempo / 70) * 0.5f);

		glTranslatef(-UBICACION_CAMARA_PRIMERA_PERSONA.x, -UBICACION_CAMARA_PRIMERA_PERSONA.y, -UBICACION_CAMARA_PRIMERA_PERSONA.z);

		break;
	case MODOS_VISUALIZACION_VISTA_ORGINAL:

		if (configuraciones::getInstancia()->getModoIluminacion() == MODOS_ILUMINACION_NOCHE) {
			glLightfv(GL_LIGHT1, GL_POSITION, light1PosVistaOriginal);
		}
		

		glTranslatef(-UBICACION_CAMARA_VISTA_ORIGINAL.x, -UBICACION_CAMARA_VISTA_ORIGINAL.y, -UBICACION_CAMARA_VISTA_ORIGINAL.z);


		break;
	default:
		break;
	}
}

MODOS_VISUALIZACION modoVisualizacion::getModoVis() {
	return this->modoVisualizacionActual;
}

void modoVisualizacion::aplicarTransformacionPorCamara() {
	mathVector posicionEnMapaJugador;

	glRotatef(-player->getAnguloActualEnMapa(), 0.0, 0.0, 1.0);
	switch (modoVisualizacionActual) {
	case MODOS_VISUALIZACION_PRIMERA_PERSONA:

		posicionEnMapaJugador = player->getPosicionEnMapa();
		glTranslatef(-posicionEnMapaJugador.x, -posicionEnMapaJugador.y, -posicionEnMapaJugador.z);

		break;
	case MODOS_VISUALIZACION_VISTA_ORGINAL:

		posicionEnMapaJugador = player->getPosicionEnMapa();
		glTranslatef(-posicionEnMapaJugador.x, -posicionEnMapaJugador.y, -posicionEnMapaJugador.z);


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

void modoVisualizacion::renderHud() {
	switch (modoVisualizacionActual) {
	case MODOS_VISUALIZACION_PRIMERA_PERSONA:
		// Pendiente rotacion de camara
		hud_player->render();
		break;
	case MODOS_VISUALIZACION_VISTA_ORGINAL:
		hud_player->render();
		break;
	default:
		break;
	}
}
