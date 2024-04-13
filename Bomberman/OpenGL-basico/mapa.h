#pragma once
#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include <stdio.h>
#include <GL/glu.h>
#include <vector>
#include "shapes.h"

#define LARGO_UNIDAD 5  // tamaño de cada item (casilero) del mapa
#define ALTURA_PARED 5 // Altura de los cubos y paredes

enum tipoMapaItem {
	CAMINO,
	PARED_INDESTRUCTIBLE,
	PARED_DESTRUCTIBLE
};


struct mapaItem {
	tipoMapaItem tipo;
	//Aca va a haber un puntero cuando hay mas datos del elemento
};


class mapa {

private:
	int cant_filas;
	int cant_columnas;

	mapaItem** estructuraMapa;

	retangulo2d pisoShape;
	retangulo3d bordesShape[4];

	//vector<retangulo3d> bordesShapes;


	//Variuables redundantes (optimizacion)
	float alturaReal; // cant_filas * LARGO_UNIDAD
	float anchoReal; // cant_columnas * LARGO_UNIDAD

public:
	mapa(int cant_filas, int cant_columnas);
	//para el futuro: mapa(XMLFile configuracion)

	// Retorna la altura en el eje z donde debe estar la camara para que se pueda ver todo el mapa
	float getAlturaVistaPanoramica();

	void render();

	~mapa();
};