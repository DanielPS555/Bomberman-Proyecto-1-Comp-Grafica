#include <vector>
#include "../../Utils/renderUtils.h"
#include <tuple>
#include "../../Utils/mathVector.h"
#include "enemigo.h"
#include "bomb.h"
#include "../shapes/Rectangulo2d.h"
#include "../shapes/Rectangulo3d.h"

#ifndef MAPA
#define MAPA

class modoVisualizacion;
class enemigo;

#define LARGO_UNIDAD 25  // tama�o de cada item (casilero) del mapa
#define ALTURA_PARED 25 // Altura de los cubos y paredes
#define ALTURA_EXTERIOR 40 // Altura de los cubos y paredes

#define NUMERO_PARTICIONES_PISO 30
#define NUMERO_PARTICIONES_PARED_LIMITE 25
#define NUMERO_PARTICIONES_PARED_INTERNA 2

enum tipoMapaItem {
	CAMINO,
	PARED_INDESTRUCTIBLE,
	PARED_DESTRUCTIBLE,
	BOMB,
	ENEMY,
	PUERTA
};


struct mapaItem {
	tipoMapaItem tipo;
	Rectangulo3d<NUMERO_PARTICIONES_PARED_INTERNA>* figura;
	//Aca va a haber un puntero cuando hay mas datos del elemento
};



class mapa {

private:
	int cant_filas;
	int cant_columnas;

	modoVisualizacion* modV;

	mapaItem*** estructuraMapa;

	Rectangulo2d<NUMERO_PARTICIONES_PISO>* pisoShape;
	Rectangulo2d<NUMERO_PARTICIONES_PISO>* pisoExterior;

	Rectangulo2d<1>* cielo;

	Rectangulo3d<NUMERO_PARTICIONES_PARED_LIMITE>* bordesShape[4];

	std::vector<std::tuple<int, int>> destructibles;
	std::vector<std::tuple<int, int>> destructiblesRes;
	std::vector<std::tuple<int, int>> enemisStart;

	
	GLuint textura;
	GLuint texturaPared;
	GLuint texturapiso;
	GLuint texturaIndestructibles;
	GLuint texturaTecho;
	GLuint texturaPuerta;
	GLuint texturaParedExterior;
	GLuint texturapisoExterior;
	enemigo ** enemigos;
	//vector<retangulo3d> bordesShapes;


	int xPuerta, yPuerta;
	vertice verticesPuerta;

	//Variuables redundantes (optimizacion)
	float alturaReal; // cant_filas * LARGO_UNIDAD
	float anchoReal; // cant_columnas * LARGO_UNIDAD

public:
	mapa(int cant_filas, int cant_columnas, int posXPuerta, int posYPuerta);
	

	void setModoVisualizacion(modoVisualizacion* m) {
		modV = m;
	}

	// Retorna la altura en el eje z donde debe estar la camara para que se pueda ver todo el mapa
	float getAlturaVistaPanoramica();

	void render();

	mathVector obtenerPosicionInicialJugador();
	float anguloInicialJugador();

	bool agregarBomba(float posEnXMapa, float posEnYMapa);

	int eliminarDestructibles(float** destructibles, int alcanze);

	mapaItem*** getEstructuraMapa();

	int getCantFilas();

	int getCantColumnas();

	void renderEnemigos(float deltaTiempo, mapa * map);


	bool isCeldaPared(int corrX, int coorY);

	void renderBombas(bomba** bombs);


	void getCordenadasCelda(mathVector posicion,
		mathVector& verticeInferiorIsquierdo,
		float& anchoCelda,
		float& altoCelda);

	void isColicion(mathVector posicionActual,
		mathVector nuevaPosicion,
		bool& colicionSuperior,
		bool& colicionDerecha,
		bool& colicionAbajo,
		bool& colicionIsquierda);

	bool destructEsPuerta();

	void renderPuerta();

	bool noHayEnemigos();

	bool victoria(mathVector posJugador);

	bool danioPorEnemigo(mathVector posicionActual);

	int cantEnemigosVivos();

	void resetDestructibles();

	void resetEnemies();

	void newLevel(int cant_filas, int cant_columnas, int posXPuerta, int posYPuerta, int cantdestructibles, int cantEnemies);

	bool esEnemStart(int x, int y);

	~mapa();
};


#endif