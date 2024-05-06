#include "renderUtils.h"
#include "mathVector.h"
#include "objloader.h"
#include "util.h"
class mapa;

#ifndef ENEMIGO
#define ENEMIGO

class enemigo
{
private:
	short idEnemigo;
	mathVector posicion;
	direccion direccionActual;
	
	//TODO cambiar cuando tengamos obj.
	retangulo3dss vertices; 
	GLuint textura;
	int x, y;
	bool cambio;

	std::vector<unsigned short> indices;
	std::vector<glm::vec3> verticess;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

public:
	enemigo(short idEnemigo);
	enemigo(short idEnemigo, mathVector posicionInicial, direccion actual, int i, int j, char * dir_textura);
	void trasladar(float t, mapa * map);
	void render();
	direccion siguienteDireccion(mapa *  map);
	int getXEnemy();
	int getYEnemy();
	mathVector getPosicion();
};

#endif