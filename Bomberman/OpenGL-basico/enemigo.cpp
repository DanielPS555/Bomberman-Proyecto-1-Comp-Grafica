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


enemigo::enemigo(short id){
	idEnemigo = id;
}

//posicionInicial representa el centro del cuadrante i,j
enemigo::enemigo(short id, mathVector posicionInicial,direccion actual, int i, int j, char * dir_textura) {

	bool res = loadAssImp("assets/Horse.obj", indices, verticess, uvs, normals);

	idEnemigo = id;
	this->x = i;
	this->y = j;
	this->posicion = posicionInicial;
	this->direccionActual = actual;
	this->cambio = false;
	//no me deja poner int directamente.
	float jj = j;
	float ii = i;
	this->posicion = { ii * LARGO_UNIDAD + LARGO_UNIDAD/2, jj * LARGO_UNIDAD + LARGO_UNIDAD/2,0 };
	
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
	float radio = 1;
	float deltax = 20.f * t / (1000);
	//std::cout << "deltax:" << deltax << std::endl;
	mapaItem *** estructuraMapa = map->getEstructuraMapa();
	mapaItem* item = new mapaItem();
	
	item->tipo = ENEMY;
	delete estructuraMapa[y][x];
	estructuraMapa[y][x] = NULL;
	
	if (direccionActual == DERECHA) {
		this->posicion.x = this->posicion.x +  deltax;
		int coordenada_sig_centro = (x + 1) * LARGO_UNIDAD + LARGO_UNIDAD/2;
		if (abs(posicion.x - coordenada_sig_centro) < radio && !this->cambio) {
			this->posicion.x = coordenada_sig_centro;
			this->x = x + 1;
			this->cambio = true;
			this->direccionActual = siguienteDireccion(map);
		}
		else if ((cambio && abs(posicion.x - coordenada_sig_centro) > radio)) {
			cambio = false;
		}
	}
	else if (direccionActual == ABAJO) {
		this->posicion.y = this->posicion.y - deltax;

		int coordenada_sig_centro = (y - 1) * LARGO_UNIDAD + LARGO_UNIDAD / 2;

		if (abs(posicion.y - coordenada_sig_centro) < radio && !this->cambio) {
			this->posicion.y = coordenada_sig_centro;
			this->y = y - 1;
			this->cambio = true;
			this->direccionActual = siguienteDireccion(map);
		}
		else if ((cambio && abs(posicion.y - coordenada_sig_centro) > radio)) {
			cambio = false;
		}

	}
	else if (direccionActual == ARRIBA) {
		this->posicion.y = this->posicion.y + deltax;

		int coordenada_sig_centro = (y + 1) * LARGO_UNIDAD + LARGO_UNIDAD / 2;

		if (abs(posicion.y - coordenada_sig_centro) < radio && !this->cambio) {
			this->posicion.y = coordenada_sig_centro;
			this->y = y + 1;
			this->cambio = true;
			this->direccionActual = siguienteDireccion(map);
		}
		else if ((cambio && abs(posicion.y - coordenada_sig_centro) > radio)) {
			cambio = false;
		}

	}

	else if (direccionActual == IZQUIERDA) {
		this->posicion.x = this->posicion.x - deltax;

		int coordenada_sig_centro = (x - 1) * LARGO_UNIDAD + LARGO_UNIDAD / 2;

		if (abs(posicion.x - coordenada_sig_centro) < radio && !this->cambio) {
			this->posicion.x = coordenada_sig_centro;
			this->x = x - 1;
			this->cambio = true;
			this->direccionActual = siguienteDireccion(map);
		}
		else if ((cambio && abs(posicion.x - coordenada_sig_centro) > radio)) {
			cambio = false;
		}
	}
	estructuraMapa[y][x] = item;
}





void enemigo::render(){
	glPushMatrix();

	int idLuz;

	switch (idEnemigo){
	case 0:
		idLuz = GL_LIGHT2;
		break;
	case 1:
		idLuz = GL_LIGHT3;
		break;
	case 2:
		idLuz = GL_LIGHT4;
		break;
	case 3:
		idLuz = GL_LIGHT5;
		break;
	case 4:
		idLuz = GL_LIGHT6;
		break;
	case 5:
		idLuz = GL_LIGHT7;
		break;

	}

	

	glTranslatef(posicion.x, posicion.y, 0);

	//iniciliarRenderVertexArray();

	glEnable(idLuz);
	GLfloat lightColor[] = { 1.0f, .1f, 0.1f, 1.f };
	glLightfv(idLuz, GL_DIFFUSE, lightColor);

	GLfloat light1PosPrimeraPersona[] = { x, y,  LARGO_UNIDAD / 2, 1.0f };
	glLightfv(idLuz, GL_POSITION, light1PosPrimeraPersona);

	glLightf(idLuz, GL_CONSTANT_ATTENUATION, 0.5f);
	glLightf(idLuz, GL_LINEAR_ATTENUATION, 0.00f);
	glLightf(idLuz, GL_QUADRATIC_ATTENUATION, 0.0040f);


	glScalef(0.2, 0.2, 0.2);
	glRotatef(90, 1, 0, 0);
	glRotatef(180, 0, 1, 0);

	

	//renderRectangulo3d(this->vertices, textura);
	if (direccionActual == DERECHA) {
		glRotatef(-90, 0,1,0 );
	}
	if (direccionActual == IZQUIERDA) {
		glRotatef(90, 0, 1, 0);
	}
	if (direccionActual == ABAJO) {
		glRotatef(180, 0, 1, 0);
	}

	render3dObject(verticess, uvs, normals, indices, textura);

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

int enemigo::getXEnemy()
{
	return this->x;
}

int enemigo::getYEnemy()
{
	return this->y;
}

mathVector enemigo::getPosicion()
{
	return this->posicion;
}
