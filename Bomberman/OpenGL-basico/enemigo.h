#include "renderUtils.h"
#include "mathVector.h"


class mapa;

#ifndef ENEMIGO
#define ENEMIGO

enum direccion {
	ARRIBA,
	ABAJO,
	DERECHA,
	IZQUIERDA
};

class enemigo
{
private:
	mathVector posicion;
	direccion direccionActual;
	
	//TODO cambiar cuando tengamos obj.
	retangulo3d vertices; 
	GLuint textura;
	int x, y;
	bool cambio;

public:
	enemigo();
	enemigo(mathVector posicionInicial, direccion actual, int i, int j, char * dir_textura);
	void trasladar(float t, mapa * map);
	void render();
	direccion siguienteDireccion(mapa *  map);
	
};

#endif