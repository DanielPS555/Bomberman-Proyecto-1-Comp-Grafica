#include <vector>
#include "renderUtils.h"
#include <tuple>
#include "mathVector.h"


#ifndef MAPA
#define MAPA


#define LARGO_UNIDAD 25  // tama�o de cada item (casilero) del mapa
#define ALTURA_PARED 30 // Altura de los cubos y paredes

enum tipoMapaItem {
	CAMINO,
	PARED_INDESTRUCTIBLE,
	PARED_DESTRUCTIBLE,
	BOMBA
};


struct mapaItem {
	tipoMapaItem tipo;
	retangulo3d figura;
	//Aca va a haber un puntero cuando hay mas datos del elemento
};



class mapa {

private:
	int cant_filas;
	int cant_columnas;

	mapaItem*** estructuraMapa;

	retangulo2d pisoShape;
	retangulo3d bordesShape[4];
	std::vector<std::tuple<int, int>> destructibles;
	
	GLuint textura;
	GLuint texturaPared;
	GLuint texturapiso;
	GLuint texturaIndestructibles;
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


	mathVector obtenerPosicionInicialJugador();
	float anguloInicialJugador();

	bool agregarBomba(float posEnXMapa, float posEnYMapa);
	void eliminarDestructibles(float** destructibles, int alcanze);

	~mapa();
};


#endif