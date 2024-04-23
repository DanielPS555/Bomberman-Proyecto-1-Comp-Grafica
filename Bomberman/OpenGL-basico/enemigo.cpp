#include "enemigo.h"
#include <stdio.h>
#include <GL/glu.h>
#include "SDL.h"
#include "SDL_opengl.h"
#include "util.h"
#include <cstdlib>
#include <ctime>   
#include "mapa.h"
// Para inicializar la semilla aleatoria
//i, j representan el cuadrande donde inicia el enemigo en el mapa.
// posX, posY representa la posicion del centro del enemigo.


enemigo::enemigo(){}

//posicionInicial representa el centro del cuadrante i,j
enemigo::enemigo(mathVector posicionInicial,direccion actual, int i, int j, char * dir_textura) {
	this->x = i;
	this->y = j;
	this->posicion = posicionInicial;
	this->direccionActual = actual;
	this->cambio = false;
	//no me deja poner int directamente.
	float jj = j;
	float ii = i;
	this->posicion = { ii * LARGO_UNIDAD + LARGO_UNIDAD/2, jj * LARGO_UNIDAD + LARGO_UNIDAD/2,0 };
	std::cout << "x " << posicion.x << std::endl;
	std::cout << "y " << posicion.y << std::endl;
	
	vertice verticesEnemigo[8] = {

		{{ 0		, 0					, 0 }, {1,1,1} },
		{{LARGO_UNIDAD / 2	,  0					, 0			    }, {1,1,1}},
		{{ LARGO_UNIDAD / 2	,  0					, ALTURA_PARED /2 }, {0.1,0.1,0.1}},
		{{0	        , 0					, ALTURA_PARED / 2 }, {1,1,1}},
		{{ 0	        , LARGO_UNIDAD / 2	    , ALTURA_PARED / 2}, {0.1,0.1,0.1}},
		{{ 0        , LARGO_UNIDAD / 2		, 0			    }, {1,1,1}},
		{{  LARGO_UNIDAD / 2	, LARGO_UNIDAD / 2		, 0				}, {0.1,0.1,0.1}},
		{{  LARGO_UNIDAD / 2	, LARGO_UNIDAD / 2		, ALTURA_PARED / 2	}, {1,1,1}}

	};


	this->textura = inicializarTextura(dir_textura);
	this->vertices = createRetangulo3d(verticesEnemigo);
}
void enemigo::trasladar(float t, mapa * map) {
	float deltax = 20.f * t / (1000);
	mapaItem *** estructuraMapa = map->getEstructuraMapa();
	mapaItem* item = new mapaItem();
	
	item->tipo = ENEMY;
	delete estructuraMapa[y][x];
	estructuraMapa[y][x] = NULL;

	if (direccionActual == DERECHA) {
		this->posicion.x = this->posicion.x +  deltax;
		int coordenada_sig_centro = (x + 1) * LARGO_UNIDAD + LARGO_UNIDAD/2;
		if (abs(posicion.x - coordenada_sig_centro) < 0.1 && !this->cambio) {
			this->posicion.x = coordenada_sig_centro;
			this->x = x + 1;
			this->cambio = true;
			this->direccionActual = siguienteDireccion(map);
		}
		else if ((cambio && abs(posicion.x - coordenada_sig_centro) > 0.1)) {
			cambio = false;
		}
	}
	else if (direccionActual == ABAJO) {
		this->posicion.y = this->posicion.y - deltax;

		int coordenada_sig_centro = (y - 1) * LARGO_UNIDAD + LARGO_UNIDAD / 2;

		if (abs(posicion.y - coordenada_sig_centro) < 0.1 && !this->cambio) {
			this->posicion.y = coordenada_sig_centro;
			this->y = y - 1;
			this->cambio = true;
			this->direccionActual = siguienteDireccion(map);
		}
		else if ((cambio && abs(posicion.y - coordenada_sig_centro) > 0.1)) {
			cambio = false;
		}

	}
	else if (direccionActual == ARRIBA) {
		this->posicion.y = this->posicion.y + deltax;

		int coordenada_sig_centro = (y + 1) * LARGO_UNIDAD + LARGO_UNIDAD / 2;

		if (abs(posicion.y - coordenada_sig_centro) < 0.1 && !this->cambio) {
			this->posicion.y = coordenada_sig_centro;
			this->y = y + 1;
			this->cambio = true;
			this->direccionActual = siguienteDireccion(map);
		}
		else if ((cambio && abs(posicion.y - coordenada_sig_centro) > 0.1)) {
			cambio = false;
		}

	}

	else if (direccionActual == IZQUIERDA) {
		this->posicion.x = this->posicion.x - deltax;

		int coordenada_sig_centro = (x - 1) * LARGO_UNIDAD + LARGO_UNIDAD / 2;

		if (abs(posicion.x - coordenada_sig_centro) < 0.1 && !this->cambio) {
			this->posicion.x = coordenada_sig_centro;
			this->x = x - 1;
			this->cambio = true;
			this->direccionActual = siguienteDireccion(map);
		}
		else if ((cambio && abs(posicion.x - coordenada_sig_centro) > 0.1)) {
			cambio = false;
		}
	}
	estructuraMapa[y][x] = item;
}





void enemigo::render(){
	glPushMatrix();

	glTranslatef(posicion.x - LARGO_UNIDAD/4 , posicion.y - LARGO_UNIDAD/4, 0);

	iniciliarRenderVertexArray();

	renderRectangulo3d(this->vertices, textura);

	finalizarRenderVertexArray();

	glPopMatrix();
	
}
//i, j mayor o igual que 0 para tomar en cuenta los bordes y ademas para que no se vaya del array.
direccion enemigo::siguienteDireccion(mapa * map) {
	//srand(time(nullptr));

	mapaItem *** estructuraMapa = map->getEstructuraMapa();
	int cant_filas = map->getCantFilas();
	int cant_columnas = map->getCantColumnas();

	std::vector<direccion> direccionesValidas;
	if(( this->y+1 < cant_filas && y >= 0 && (estructuraMapa[this->y + 1][this->x]) == nullptr)) {
		direccionesValidas.push_back(ARRIBA);
	}
	if(x + 1 < cant_columnas && y >= 0 && (estructuraMapa[this->y][this->x + 1] == nullptr)){
		direccionesValidas.push_back(DERECHA);
	}
	if(y > 0 && (estructuraMapa[this->y-1 ][this->x] == nullptr)) {
		direccionesValidas.push_back(ABAJO);
	}
	 if(x > 0  && (estructuraMapa[this->y][this->x - 1] == nullptr)) {
		 direccionesValidas.push_back(IZQUIERDA);
	}

	int indiceAleatorio = rand() % direccionesValidas.size();
	return direccionesValidas[indiceAleatorio];
	
}