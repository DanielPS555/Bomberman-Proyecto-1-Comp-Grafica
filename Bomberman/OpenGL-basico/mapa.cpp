#include "mapa.h"
#include "stdio.h"
#include "stdlib.h"
#include "util.h"
#include "renderUtils.h"
#include "enemigo.h"
using namespace std;


mapa::mapa(int cant_filas, int cant_columnas, int posXPuerta, int posYPuerta) {
	this->cant_filas = cant_filas;
	this->cant_columnas = cant_columnas;
	this->xPuerta = posXPuerta;
	this->yPuerta = posYPuerta;
	alturaReal = cant_filas * LARGO_UNIDAD;
	anchoReal = cant_columnas * LARGO_UNIDAD;


	pisoShape = {
		 { 0.,0.,0.			,anchoReal,0.,0.,		anchoReal,alturaReal,0.,		0.,alturaReal,0. }    // vertices
		,{0,0,1}																						  //Norma
		,{ 1.,1.,1.,		0.1,0.1,0.1,              1.,1.,1.,					   0.1,0.1,0.1         }	  // colores	
		,{0,1,2,3}																						  // indices
		,{0.0,0.0,0.0,1.0,1.0,1.0,1.0,0.0}
	};

	techoShape = {
	 { 0.,0.,ALTURA_PARED		,anchoReal,0.,ALTURA_PARED,		anchoReal,alturaReal,ALTURA_PARED,		0.,alturaReal,ALTURA_PARED }    // vertices
	,{0,0,1}																						  //Norma
	,{ 1.,1.,1.,		 1.,1.,1.,              1.,1.,1.,					    1.,1.,1.         }	  // colores	
	,{0,1,2,3}																						  // indices
	,{0.0,0.0,0.0,1.0,1.0,1.0,1.0,0.0}
	};


	vertice verticesBordeInferior[8] = {
		{{-LARGO_UNIDAD				, -LARGO_UNIDAD			, 0				}, {1,1,1}},
		{{anchoReal + LARGO_UNIDAD	, -LARGO_UNIDAD			, 0				}, {0.1,0.1,0.1}},
		{{anchoReal + LARGO_UNIDAD	, -LARGO_UNIDAD			, ALTURA_PARED	}, {1,1,1}},
		{{-LARGO_UNIDAD				, -LARGO_UNIDAD			, ALTURA_PARED	}, {0.1,0.1,0.1}},
		{{-LARGO_UNIDAD				, 0						, ALTURA_PARED	}, {1,1,1}},
		{{-LARGO_UNIDAD				, 0						, 0				}, {0.1,0.1,0.1}},
		{{anchoReal + LARGO_UNIDAD	, 0						, 0				}, {1,1,1}},
		{{anchoReal + LARGO_UNIDAD	, 0						, ALTURA_PARED  }, {0.1,0.1,0.1}}
	};

	vertice verticesBordeIzquierdo[8] = {
		{{ -LARGO_UNIDAD              ,	0					  , 0             }, {1,1,1}},
		{{0			                  , 0			          , 0             }, {0.1,0.1,0.1}},
		{{0				              , 0					  , ALTURA_PARED  }, {1,1,1}},
		{{ -LARGO_UNIDAD              ,	0					  , ALTURA_PARED  }, {0.1,0.1,0.1}},
		{{ -LARGO_UNIDAD              ,	alturaReal			  , ALTURA_PARED  }, {1,1,1}},
		{{ -LARGO_UNIDAD              ,	alturaReal			  , 0             }, {0.1,0.1,0.1}},
		{{0			                  , alturaReal	          , 0             }, {1,1,1}},
		{{0				              , alturaReal			  , ALTURA_PARED  }, {0.1,0.1,0.1}}
	};

	vertice verticesBordeDerecho[8] = {
		{{anchoReal					, 0					  , 0             }, {1,1,1}},
		{{anchoReal	+ LARGO_UNIDAD	, 0			          , 0             }, {0.1,0.1,0.1}},
		{{anchoReal + LARGO_UNIDAD 	, 0					  , ALTURA_PARED  }, {1,1,1}},
		{{anchoReal                 , 0					  , ALTURA_PARED  }, {0.1,0.1,0.1}},
		{{anchoReal                 , anchoReal			  , ALTURA_PARED  }, {1,1,1}},
		{{anchoReal                 , anchoReal			  , 0             }, {0.1,0.1,0.1}},
		{{anchoReal + LARGO_UNIDAD	, anchoReal	          , 0             }, {1,1,1}},
		{{anchoReal + LARGO_UNIDAD	, anchoReal			  , ALTURA_PARED  }, {0.1,0.1,0.1}}
	};

	vertice verticesBordeSuperior[8] = {
		{{-LARGO_UNIDAD				, alturaReal					, 0				}, {0.1,0.1,0.1}},
		{{anchoReal + LARGO_UNIDAD	, alturaReal					, 0				}, {1,1,1}},
		{{anchoReal + LARGO_UNIDAD	, alturaReal					, ALTURA_PARED	}, {0.1,0.1,0.1}},
		{{-LARGO_UNIDAD				, alturaReal					, ALTURA_PARED	}, {1,1,1}},
		{{-LARGO_UNIDAD				, alturaReal + LARGO_UNIDAD		, ALTURA_PARED	}, {0.1,0.1,0.1}},
		{{-LARGO_UNIDAD				, alturaReal + LARGO_UNIDAD		, 0				}, {1,1,1}},
		{{anchoReal + LARGO_UNIDAD	, alturaReal + LARGO_UNIDAD		, 0				}, {0.1,0.1,0.1}},
		{{anchoReal + LARGO_UNIDAD	, alturaReal + LARGO_UNIDAD		, ALTURA_PARED  }, {1,1,1}}
	};

	bordesShape[0] = createRetangulo3d(verticesBordeInferior);
	bordesShape[1] = createRetangulo3d(verticesBordeIzquierdo);
	bordesShape[2] = createRetangulo3d(verticesBordeDerecho);
	bordesShape[3] = createRetangulo3d(verticesBordeSuperior);

	//inicializo texturas
	this->textura = inicializarTextura("assets/ladrillo.jpg");
	this->texturaPared = inicializarTextura("assets/pared.jpg");
	this->texturapiso = inicializarTextura("assets/piso2.jpg");
	this->texturaIndestructibles = inicializarTextura("assets/pared.jpg");
	this->texturaTecho = inicializarTextura("assets/nube.jpg");
	this->texturaPuerta = inicializarTextura("assets/nube.jpg");

	this->estructuraMapa = new mapaItem ** [this->cant_filas];
	for (int i = 0; i < this->cant_filas; i++) {
		this->estructuraMapa[i] = new mapaItem * [this->cant_columnas];
	}

	for (int i = 0; i < this->cant_filas; i++) {
		for (int j = 0; j < this->cant_columnas; j++) {
			if (i >= 1 && j >= 1 && i < cant_filas - 1 && j < cant_filas - 1 && i % 2 == 1 && j % 2 == 1) {
				this->estructuraMapa[i][j] = new mapaItem;
				this->estructuraMapa[i][j]->tipo = PARED_INDESTRUCTIBLE;

				vertice verticesCubo[8] = {
					{{j * LARGO_UNIDAD			, i * LARGO_UNIDAD					, 0			    }, {0.1,0.1,0.1} },
					{{(j + 1) * LARGO_UNIDAD	, i * LARGO_UNIDAD					, 0			    }, {1,1,1} },
					{{(j + 1) * LARGO_UNIDAD	, i * LARGO_UNIDAD					, ALTURA_PARED  }, {0.1,0.1,0.1} },
					{{j * LARGO_UNIDAD	        , i * LARGO_UNIDAD					, ALTURA_PARED  }, {1,1,1} },
					{{j * LARGO_UNIDAD	        , (i + 1 ) * LARGO_UNIDAD		    , ALTURA_PARED	}, {0.1,0.1,0.1} },
					{{j  * LARGO_UNIDAD	        , (i + 1) * LARGO_UNIDAD			, 0			    }, {1,1,1} },
					{{ (j + 1) * LARGO_UNIDAD	, (i + 1) * LARGO_UNIDAD			, 0				}, {0.1,0.1,0.1} },
					{{ (j + 1) * LARGO_UNIDAD	, (i + 1) * LARGO_UNIDAD			, ALTURA_PARED	}, {1,1,1} },

				};

				this->estructuraMapa[i][j]->figura = createRetangulo3d(verticesCubo);
			}else {
				this->estructuraMapa[i][j] = nullptr;
			}
		}
	}

	destructibles.push_back(std::make_tuple(1, 2));
	destructibles.push_back(std::make_tuple(2, 5));
	destructibles.push_back(std::make_tuple(3, 4));
	destructibles.push_back(std::make_tuple(7, 8));
	destructibles.push_back(std::make_tuple(3,2));
	destructibles.push_back(std::make_tuple(3, 2));
	destructibles.push_back(std::make_tuple(5, 4));
	destructibles.push_back(std::make_tuple(6, 5));
	destructibles.push_back(std::make_tuple(8,6));

	if(!destructEsPuerta()){
		destructibles.push_back(std::make_tuple(this->yPuerta, this->xPuerta));
	}

	for (const auto& par : this->destructibles) {
		int i = std::get<0>(par);
		int j = std::get<1>(par);

		this->estructuraMapa[i][j] = new mapaItem;
		this->estructuraMapa[i][j]->tipo = PARED_DESTRUCTIBLE;

		vertice verticesCubo[8] = {
			{{j * LARGO_UNIDAD			, i * LARGO_UNIDAD					, 0			    }, {1,1,1}},
			{{(j + 1) * LARGO_UNIDAD	, i * LARGO_UNIDAD					, 0			    }, {1,1,1}},
			{{(j + 1) * LARGO_UNIDAD	, i * LARGO_UNIDAD					, ALTURA_PARED  }, {1.1,1,1}},
			{{j * LARGO_UNIDAD	        , i * LARGO_UNIDAD					, ALTURA_PARED  }, {1,1,1}},
			{{j * LARGO_UNIDAD	        , (i + 1) * LARGO_UNIDAD		    , ALTURA_PARED	}, {1,1,1}},
			{{j * LARGO_UNIDAD	        , (i + 1) * LARGO_UNIDAD			, 0			    }, {1,1,1}},
			{{ (j + 1) * LARGO_UNIDAD	, (i + 1) * LARGO_UNIDAD			, 0				}, {1,1,1}},
			{{ (j + 1) * LARGO_UNIDAD	, (i + 1) * LARGO_UNIDAD			, ALTURA_PARED	}, {1,1,1}}
		};

		this->estructuraMapa[i][j]->figura = createRetangulo3d(verticesCubo);

	}

	this->enemigos  = new enemigo * [4];

	this->enemigos[0] = new enemigo({ 0.f, 0.f, 0.f }, DERECHA, 2, 2, "assets/enemy.jpg");
	this->enemigos[1] = new enemigo({ 0.f, 0.f, 0.f }, DERECHA, 4, 4, "assets/enemy2.jpg");
	this->enemigos[2] = new enemigo({ 0.f, 0.f, 0.f }, DERECHA, 4, 4, "assets/enemigo3.jpg");
	this->enemigos[3] = new enemigo({ 0.f, 0.f, 0.f }, DERECHA, 4, 4, "assets/enemy4.jpg");

}

//ToDo implementar segun el numero de min{filas, columnas, CONSTANTE}
float mapa::getAlturaVistaPanoramica() {
	return 100.0f;
}


void mapa::render() {

	
	glPushMatrix();

	iniciliarRenderVertexArray();

	//cargarTextura(this->textura);

	renderRectangulo2d(pisoShape,this->texturapiso);
	//renderRectangulo2d(techoShape, this->texturaTecho);

	for (int i = 0; i < 4; i++) {
		renderRectangulo3d(bordesShape[i],this->texturaPared);
	}


	for (int i = 0; i < this->cant_filas; i++) {
		for (int j = 0; j < this->cant_columnas; j++) {
			if (this->estructuraMapa[i][j] != nullptr) {
				GLuint textura;
				if (this->estructuraMapa[i][j]->tipo == PARED_INDESTRUCTIBLE) {
					textura = this->texturaIndestructibles;
				}
				else {
					textura = this->textura;
				}
				renderRectangulo3d(this->estructuraMapa[i][j]->figura, textura);
			}
		}
	}
	
	finalizarRenderVertexArray();

	glPopMatrix();
}



//ToDo implementar en base a la configuracion del mapa
mathVector mapa::obtenerPosicionInicialJugador() {
	return { LARGO_UNIDAD/2, LARGO_UNIDAD / 2, 0.f };
}


//ToDo implementar en base a la configuracion del mapa
float mapa::anguloInicialJugador() {
	return 0.0f;
}

bool mapa::agregarBomba(float posEnXMapa, float posEnYMapa)
{	
	int i = posEnXMapa;
	int j = posEnYMapa;
	if (i < this->cant_columnas && i >= 0 && j < this->cant_filas && j >= 0) {
		if (this->estructuraMapa[i][j] == nullptr) {
			this->estructuraMapa[i][j] = new mapaItem;
			this->estructuraMapa[i][j]->tipo = BOMB;

			destructibles.push_back(std::make_tuple(posEnXMapa, posEnYMapa));
			return true;
		}
		return false;
	}
	else {
		return false;
	}
}

int mapa::eliminarDestructibles(float** destruir, int alcanze)
{
	int puntos = 0;
	std::tuple<int, int> temp;
	if (destruir != nullptr) {
		for (int i = 0; i <= alcanze * 4; i++) {
			int y = destruir[i][0];
			int x = destruir[i][1];
			int s = 0;
			auto it = destructibles.begin();
			while (it != destructibles.end()) {
				temp = destructibles[s];
				int r = std::get<0>(temp);
				int f = std::get<1>(temp);
				if (y == r && x == f) {
					delete this->estructuraMapa[r][f];
					this->estructuraMapa[r][f] = nullptr;
					destructibles.erase(it);
					it = destructibles.end();
					puntos += 1500;
				}
				else {
					s++;
					++it;
				}
			}
			for (int p = 0; p < 4; p++) {
				if (enemigos[p] != nullptr) {
					int enx = this->enemigos[p]->getXEnemy();
					int eny = this->enemigos[p]->getYEnemy();
					int posy = floor(this->enemigos[p]->getPosicion().y /LARGO_UNIDAD);
					int posx = floor(this->enemigos[p]->getPosicion().x / LARGO_UNIDAD);
					if (x == enx && y == eny) {
						delete this->estructuraMapa[eny][enx];
						this->estructuraMapa[eny][enx] = nullptr;
						enemigo* en = this->enemigos[p];
						this->enemigos[p] = nullptr; 
						delete en;
						puntos += 5000;
					}
					else {
						if (x == posx && y == posy) {
							delete this->estructuraMapa[posy][posx];
							this->estructuraMapa[posy][posx] = nullptr;
							enemigo* en = this->enemigos[p];
							this->enemigos[p] = nullptr;
							delete en;
							puntos += 5000; 
						}
					}
				}
			}
		}
	}
	if (this->estructuraMapa[this->yPuerta][this->xPuerta] == nullptr) {
		this->estructuraMapa[this->yPuerta][this->xPuerta] = new mapaItem;
		this->estructuraMapa[this->yPuerta][this->xPuerta]->tipo = PUERTA;
	}
	return puntos;
}

void mapa::renderEnemigos(float deltatiempo, mapa * map) {
	for (int i = 0; i < 4;i++) {
		if (enemigos[i] != nullptr) {
			enemigos[i]->trasladar(deltatiempo, map);
			enemigos[i]->render();
		}
	}
}

void mapa::renderBombas(bomba** bombs)
{
	for (int i = 0; i < 4; i++) {
		if (bombs[i] != nullptr) {
			bombs[i]->render();
		}
	}
}



void mapa::isColicion(mathVector posicionActual,
					  mathVector nuevaPosicion,
					  bool& colicionSuperior,
					  bool& colicionDerecha,
					  bool& colicionInferior,
					  bool& colicionIsquierda) {
	// ---- Etapa 1: ¿Choque contra las paredes del mapa?
	int casilleroNuevolMapaX = floor(nuevaPosicion.x / LARGO_UNIDAD);
	int casilleroNuevolMapaY = floor(nuevaPosicion.y / LARGO_UNIDAD);

	bool cSup = false;
	bool cDer = false; 
	bool cIsq = false;
	bool cInf = false;


	if (casilleroNuevolMapaX < 0 ) {
		cIsq = true;
		
	}

	if (casilleroNuevolMapaX >= cant_columnas) {
		cDer = true;
		
	}

	if (casilleroNuevolMapaY < 0) {
		cInf = true;
		
	}

	if (casilleroNuevolMapaY >= cant_filas) {
		cSup = true;
		
	}

	if (cIsq || cDer || cInf || cSup) {
		colicionIsquierda = cIsq || colicionIsquierda;
		colicionDerecha = cDer || colicionDerecha;
		colicionSuperior = cSup || colicionSuperior;
		colicionInferior = cInf || colicionInferior;
		return;
	}

	// ---- Etapa 2: ¿Choque diagonal?

	int casilleroActualMapaX = floor(posicionActual.x / LARGO_UNIDAD);
	int casilleroActualMapaY = floor(posicionActual.y / LARGO_UNIDAD);

	int deltaCasillerosX = casilleroNuevolMapaX - casilleroActualMapaX;
	int deltaCasillerosY = casilleroNuevolMapaY - casilleroActualMapaY;


	if (deltaCasillerosX != 0 && deltaCasillerosY != 0) {

		if (isCeldaPared(casilleroActualMapaX + deltaCasillerosX, casilleroActualMapaY) 
			&& isCeldaPared(casilleroActualMapaX, casilleroActualMapaY + deltaCasillerosY)) {

			if (deltaCasillerosX < 0) {
				cIsq = true;

			}

			if (deltaCasillerosX > 0) {
				cDer = true;

			}

			if (deltaCasillerosY < 0) {
				cInf = true;

			}

			if (deltaCasillerosY > 0) {
				cSup = true;

			}

		}

		
	}

	if (cIsq || cDer || cInf || cSup) {
		colicionIsquierda = cIsq || colicionIsquierda;
		colicionDerecha = cDer || colicionDerecha;
		colicionSuperior = cSup || colicionSuperior;
		colicionInferior = cInf || colicionInferior;
		return;
	}

	//Etapa 3

	mapaItem* ptrLugarMap = estructuraMapa[casilleroNuevolMapaY][casilleroNuevolMapaX];
	if (isCeldaPared(casilleroNuevolMapaX, casilleroNuevolMapaY)) {
		colicionIsquierda = colicionIsquierda || casilleroNuevolMapaX < casilleroActualMapaX;
		colicionInferior  = colicionInferior || casilleroNuevolMapaY < casilleroActualMapaY;

		colicionDerecha = colicionDerecha || casilleroNuevolMapaX > casilleroActualMapaX;
		colicionSuperior = colicionSuperior || casilleroNuevolMapaY > casilleroActualMapaY;
	}
	
		
}

bool mapa::isCeldaPared(int corrX, int coorY) {

	if (corrX < 0 || corrX >= cant_columnas || coorY < 0 || coorY >= cant_filas) {
		return true;
	}

	mapaItem* ptrLugarMap = estructuraMapa[coorY][corrX];
	return ptrLugarMap != nullptr && (ptrLugarMap->tipo == PARED_DESTRUCTIBLE || ptrLugarMap->tipo == PARED_INDESTRUCTIBLE);
}


void mapa::getCordenadasCelda(mathVector posicion,
							  mathVector& verticeInferiorIsquierdo,
							  float& anchoCelda,
							  float& altoCelda) {

	int casilleroActualMapaX = floor(posicion.x / LARGO_UNIDAD);
	int casilleroActualMapaY = floor(posicion.y / LARGO_UNIDAD);

	verticeInferiorIsquierdo = { (float)casilleroActualMapaX * LARGO_UNIDAD, (float)casilleroActualMapaY * LARGO_UNIDAD, 0.0f };
	anchoCelda = LARGO_UNIDAD;
	altoCelda = LARGO_UNIDAD;
}


mapaItem*** mapa::getEstructuraMapa() {
	return this->estructuraMapa;
}

int mapa::getCantFilas(){
	return this->cant_filas;
}
int mapa::getCantColumnas() {
	return this->cant_columnas;
}

mapa::~mapa() {
	for (int i = 0; i < this->cant_filas ;i++){
		free(this->estructuraMapa[i]);
	}
	free(estructuraMapa);
}

bool mapa::destructEsPuerta()
{	
	bool esPuerta = false;
	auto itP = destructibles.begin();
	for (const auto& par : this->destructibles) {
		int i = std::get<0>(par);
		int j = std::get<1>(par);
		if (this->yPuerta == i && this->xPuerta == j) {
			esPuerta = true;
		}
	}
	return esPuerta;
}

void mapa::renderPuerta()
{
	if (!destructEsPuerta()) {
		glPushMatrix();

		iniciliarRenderVertexArray();

		vertice verticesPuerta[8] = {
				{{this->xPuerta * LARGO_UNIDAD + (LARGO_UNIDAD / 4)		    , this->yPuerta * LARGO_UNIDAD + (LARGO_UNIDAD / 4)			, 0}			   , {1,1,1}},
				{{(this->xPuerta + 1) * LARGO_UNIDAD - (LARGO_UNIDAD / 4)	, this->yPuerta * LARGO_UNIDAD + (LARGO_UNIDAD / 4)			, 0			    }  , {1,1,1}},
				{{(this->xPuerta + 1) * LARGO_UNIDAD - (LARGO_UNIDAD / 4)	, this->yPuerta * LARGO_UNIDAD + (LARGO_UNIDAD / 4)			, ALTURA_PARED / 2}, {1.1,1,1}},
				{{this->xPuerta * LARGO_UNIDAD + (LARGO_UNIDAD / 4)		    , this->yPuerta * LARGO_UNIDAD + (LARGO_UNIDAD / 4)			, ALTURA_PARED / 2}, {1,1,1}},
				{{this->xPuerta * LARGO_UNIDAD + (LARGO_UNIDAD / 4)		    , (this->yPuerta + 1) * LARGO_UNIDAD - (LARGO_UNIDAD / 4)	, ALTURA_PARED / 2}, {1,1,1}},
				{{this->xPuerta * LARGO_UNIDAD + (LARGO_UNIDAD / 4)			, (this->yPuerta + 1) * LARGO_UNIDAD - (LARGO_UNIDAD / 4)	, 0			    }  , {1,1,1}},
				{{(this->xPuerta + 1) * LARGO_UNIDAD - (LARGO_UNIDAD / 4)	, (this->yPuerta + 1) * LARGO_UNIDAD - (LARGO_UNIDAD / 4)	, 0				}  , {1,1,1}},
				{{(this->xPuerta + 1) * LARGO_UNIDAD - (LARGO_UNIDAD / 4)	, (this->yPuerta + 1) * LARGO_UNIDAD - (LARGO_UNIDAD / 4)	, ALTURA_PARED / 2}, {1,1,1}}
		};

		renderRectangulo3d(createRetangulo3d(verticesPuerta), this->texturaPuerta);

		finalizarRenderVertexArray();

		glPopMatrix();
	}
}

bool mapa::noHayEnemigos() {
	for (int i = 0; i < 4; i++) {
		if (enemigos[i] != nullptr) {
			return false;
		}
	}
	return true;
}

bool mapa::dañoPorEnemigo(mathVector posicionActual)
{
	bool daño = false;
	int i = 0;
	while (i < 4 && !daño) {
		if (enemigos[i] != nullptr) {
			mathVector posEn = enemigos[i]->getPosicion();
			daño = (posicionActual.x <= (posEn.x + 20) && posicionActual.x >= (posEn.x - 20)) && (posicionActual.y <= (posEn.y + 20) && posicionActual.y <= (posEn.y + 20));
		}
		i++;
	}
	return daño;
}

bool mapa::victoria(mathVector posJugador)
{	
	int xJugador = floor(posJugador.x * LARGO_UNIDAD);
	int yJugador = floor(posJugador.y * LARGO_UNIDAD);
	if(noHayEnemigos() && !destructEsPuerta() && (xJugador == this->xPuerta) && (yJugador == this->yPuerta)) {
		return true;
	}
	else {
		return false;
	}
}


