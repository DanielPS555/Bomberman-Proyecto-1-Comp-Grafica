#include "mapa.h"
#include "stdio.h"
#include "stdlib.h"
#include "util.h"

using namespace std;


mapa::mapa(int cant_filas, int cant_columnas) {
	this->cant_filas = cant_filas;
	this->cant_columnas = cant_columnas;
	alturaReal = cant_filas * LARGO_UNIDAD;
	anchoReal = cant_columnas * LARGO_UNIDAD;


	pisoShape = {
		 { 0.,0.,0.			,anchoReal,0.,0.,		anchoReal,alturaReal,0.,		0.,alturaReal,0. }    // vertices
		,{0,0,1}																						  //Norma
		,{ 1.,1.,1.,		 1.,1.,1.,              1.,1.,1.,					    1.,1.,1.         }	  // colores	
		,{0,1,2,3}																						  // indices
		,{0.0,0.0,0.0,1.0,1.0,1.0,1.0,0.0}
	};


	vertice verticesBordeInferior[8] = {
		{{-LARGO_UNIDAD				, -LARGO_UNIDAD			, 0				}, {1,1,1}},
		{{anchoReal + LARGO_UNIDAD	, -LARGO_UNIDAD			, 0				}, {1,1,1}},
		{{anchoReal + LARGO_UNIDAD	, -LARGO_UNIDAD			, ALTURA_PARED	}, {1,1,1}},
		{{-LARGO_UNIDAD				, -LARGO_UNIDAD			, ALTURA_PARED	}, {1,1,1}},
		{{-LARGO_UNIDAD				, 0						, ALTURA_PARED	}, {1,1,1}},
		{{-LARGO_UNIDAD				, 0						, 0				}, {1,1,1}},
		{{anchoReal + LARGO_UNIDAD	, 0						, 0				}, {1,1,1}},
		{{anchoReal + LARGO_UNIDAD	, 0						, ALTURA_PARED  }, {1,1,1}}
	};

	vertice verticesBordeIzquierdo[8] = {
		{{ -LARGO_UNIDAD              ,	0					  , 0             }, {1,1,1}},
		{{0			                  , 0			          , 0             }, {1,1,1}},
		{{0				              , 0					  , ALTURA_PARED  }, {1,1,1}},
		{{ -LARGO_UNIDAD              ,	0					  , ALTURA_PARED  }, {1,1,1}},
		{{ -LARGO_UNIDAD              ,	alturaReal			  , ALTURA_PARED  }, {1,1,1}},
		{{ -LARGO_UNIDAD              ,	alturaReal			  , 0             }, {1,1,1}},
		{{0			                  , alturaReal	          , 0             }, {1,1,1}},
		{{0				              , alturaReal			  , ALTURA_PARED  }, {1,1,1}}
	};

	vertice verticesBordeDerecho[8] = {
		{{anchoReal					, 0					  , 0             }, {1,1,1}},
		{{anchoReal	+ LARGO_UNIDAD	, 0			          , 0             }, {1,1,1}},
		{{anchoReal + LARGO_UNIDAD 	, 0					  , ALTURA_PARED  }, {1,1,1}},
		{{anchoReal                 , 0					  , ALTURA_PARED  }, {1,1,1}},
		{{anchoReal                 , anchoReal			  , ALTURA_PARED  }, {1,1,1}},
		{{anchoReal                 , anchoReal			  , 0             }, {1,1,1}},
		{{anchoReal + LARGO_UNIDAD	, anchoReal	          , 0             }, {1,1,1}},
		{{anchoReal + LARGO_UNIDAD	, anchoReal			  , ALTURA_PARED  }, {1,1,1}}
	};

	vertice verticesBordeSuperior[8] = {
		{{-LARGO_UNIDAD				, alturaReal					, 0				}, {1,1,1}},
		{{anchoReal + LARGO_UNIDAD	, alturaReal					, 0				}, {1,1,1}},
		{{anchoReal + LARGO_UNIDAD	, alturaReal					, ALTURA_PARED	}, {1,1,1}},
		{{-LARGO_UNIDAD				, alturaReal					, ALTURA_PARED	}, {1,1,1}},
		{{-LARGO_UNIDAD				, alturaReal + LARGO_UNIDAD		, ALTURA_PARED	}, {1,1,1}},
		{{-LARGO_UNIDAD				, alturaReal + LARGO_UNIDAD		, 0				}, {1,1,1}},
		{{anchoReal + LARGO_UNIDAD	, alturaReal + LARGO_UNIDAD		, 0				}, {1,1,1}},
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
					{{j * LARGO_UNIDAD			, i * LARGO_UNIDAD					, 0			    }, {1,1,1} },
					{{(j + 1) * LARGO_UNIDAD	, i * LARGO_UNIDAD					, 0			    }, {1,1,1} },
					{{(j + 1) * LARGO_UNIDAD	, i * LARGO_UNIDAD					, ALTURA_PARED  }, {1,1,1} },
					{{j * LARGO_UNIDAD	        , i * LARGO_UNIDAD					, ALTURA_PARED  }, {1,1,1} },
					{{j * LARGO_UNIDAD	        , (i + 1 ) * LARGO_UNIDAD		    , ALTURA_PARED	}, {1,1,1} },
					{{j  * LARGO_UNIDAD	        , (i + 1) * LARGO_UNIDAD			, 0			    }, {1,1,1} },
					{{ (j + 1) * LARGO_UNIDAD	, (i + 1) * LARGO_UNIDAD			, 0				}, {1,1,1} },
					{{ (j + 1) * LARGO_UNIDAD	, (i + 1) * LARGO_UNIDAD			, ALTURA_PARED	}, {1,1,1} },

				};

				this->estructuraMapa[i][j]->figura = createRetangulo3d(verticesCubo);
			}else {
				this->estructuraMapa[i][j] = nullptr;
			}
		}
	}

	destructibles.push_back(std::make_tuple(1, 2));
	destructibles.push_back(std::make_tuple(3, 4));
	destructibles.push_back(std::make_tuple(7, 8));
	destructibles.push_back(std::make_tuple(3,2));
	destructibles.push_back(std::make_tuple(5, 4));
	destructibles.push_back(std::make_tuple(6, 5));
	destructibles.push_back(std::make_tuple(8,6));


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
	return { 0.f, 0.f, 0.f };
}


//ToDo implementar en base a la configuracion del mapa
float mapa::anguloInicialJugador() {
	return 90.0f;
}

mapa::~mapa() {
	for (int i = 0; i < this->cant_filas ;i++){
		free(this->estructuraMapa[i]);
	}
	free(estructuraMapa);
}
