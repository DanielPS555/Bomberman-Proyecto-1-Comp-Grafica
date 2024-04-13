#include "mapa.h"
#include "stdio.h"
#include "stdlib.h"


using namespace std;


mapa::mapa(int cant_filas, int cant_columnas) {
	this->cant_filas = cant_filas;
	this->cant_columnas = cant_columnas;
	alturaReal = cant_filas * LARGO_UNIDAD;
	anchoReal = cant_columnas * LARGO_UNIDAD;


	pisoShape = {
		 { 0.,0.,0.			,anchoReal,0.,0.,		anchoReal,alturaReal,0.,		0.,alturaReal,0. }    // vertices
		,{0,0,1}																						  //Norma
		,{ 1.,0.,1.,		 1.,1.,1.,              1.,0.,1.,					    1.,1.,1.         }	  // colores	
		,{0,1,2,3}																						  // indices
	};


	vertice verticesBordeInferior[8] = {
		{{-LARGO_UNIDAD				, -LARGO_UNIDAD			, 0				}, {1,0,0}},
		{{anchoReal + LARGO_UNIDAD	, -LARGO_UNIDAD			, 0				}, {0,1,0}},
		{{anchoReal + LARGO_UNIDAD	, -LARGO_UNIDAD			, ALTURA_PARED	}, {1,0,0}},
		{{-LARGO_UNIDAD				, -LARGO_UNIDAD			, ALTURA_PARED	}, {0,1,0}},
		{{-LARGO_UNIDAD				, 0						, ALTURA_PARED	}, {1,0,0}},
		{{-LARGO_UNIDAD				, 0						, 0				}, {0,1,0}},
		{{anchoReal + LARGO_UNIDAD	, 0						, 0				}, {1,0,0}},
		{{anchoReal + LARGO_UNIDAD	, 0						, ALTURA_PARED  }, {0,1,0}}
	};

	vertice verticesBordeIzquierdo[8] = {
		{{ -LARGO_UNIDAD              ,	0					  , 0             }, {1,1,1}},
		{{0			                  , 0			          , 0             }, {1,0,1}},
		{{0				              , 0					  , ALTURA_PARED  }, {1,1,1}},
		{{ -LARGO_UNIDAD              ,	0					  , ALTURA_PARED  }, {1,0,1}},
		{{ -LARGO_UNIDAD              ,	alturaReal			  , ALTURA_PARED  }, {1,1,1}},
		{{ -LARGO_UNIDAD              ,	alturaReal			  , 0             }, {1,0,1}},
		{{0			                  , alturaReal	          , 0             }, {1,0,1}},
		{{0				              , alturaReal			  , ALTURA_PARED  }, {1,0,1}}
	};

	vertice verticesBordeDerecho[8] = {
		{{anchoReal					, 0					  , 0             }, {1,0,0}},
		{{anchoReal	+ LARGO_UNIDAD	, 0			          , 0             }, {1,1,0}},
		{{anchoReal + LARGO_UNIDAD 	, 0					  , ALTURA_PARED  }, {1,0,0}},
		{{anchoReal                 , 0					  , ALTURA_PARED  }, {1,0,0}},
		{{anchoReal                 , anchoReal			  , ALTURA_PARED  }, {1,1,0}},
		{{anchoReal                 , anchoReal			  , 0             }, {1,0,0}},
		{{anchoReal + LARGO_UNIDAD	, anchoReal	          , 0             }, {1,0,0}},
		{{anchoReal + LARGO_UNIDAD	, anchoReal			  , ALTURA_PARED  }, {1,1,0}}
	};

	vertice verticesBordeSuperior[8] = {
		{{-LARGO_UNIDAD				, alturaReal					, 0				}, {1,1,0}},
		{{anchoReal + LARGO_UNIDAD	, alturaReal					, 0				}, {1,0,0}},
		{{anchoReal + LARGO_UNIDAD	, alturaReal					, ALTURA_PARED	}, {1,0,0}},
		{{-LARGO_UNIDAD				, alturaReal					, ALTURA_PARED	}, {1,1,0}},
		{{-LARGO_UNIDAD				, alturaReal + LARGO_UNIDAD		, ALTURA_PARED	}, {1,0,0}},
		{{-LARGO_UNIDAD				, alturaReal + LARGO_UNIDAD		, 0				}, {1,0,0}},
		{{anchoReal + LARGO_UNIDAD	, alturaReal + LARGO_UNIDAD		, 0				}, {1,1,0}},
		{{anchoReal + LARGO_UNIDAD	, alturaReal + LARGO_UNIDAD		, ALTURA_PARED  }, {1,0,0}}
	};
	bordesShape[0] = createRetangulo3d(verticesBordeInferior);
	bordesShape[1] = createRetangulo3d(verticesBordeIzquierdo);
	bordesShape[2] = createRetangulo3d(verticesBordeDerecho);
	bordesShape[3] = createRetangulo3d(verticesBordeSuperior);


	this->estructuraMapa = new mapaItem * [this->cant_filas];
	for (int i = 0; i < this->cant_filas; i++) {
		this->estructuraMapa[i] = new mapaItem[this->cant_columnas];
	}
}

float mapa::getAlturaVistaPanoramica() {
	//ToDo implementar segun el numero de min{filas, columnas, CONSTANTE}
	return 100.;
}

void iniciliarRenderVertexArray() {
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
}

void finalizarRenderVertexArray() {
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}


void renderRectangulo2d(retangulo2d rec) {
	glNormalPointer(GL_FLOAT, 0, rec.normales);
	glColorPointer(3, GL_FLOAT, 0, rec.coloresVertices);
	glVertexPointer(3, GL_FLOAT, 0, rec.cordeneasVertices);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, rec.indices);
}

void renderRectangulo3d(retangulo3d rec) {
	glNormalPointer(GL_FLOAT, 0, rec.normales);
	glColorPointer(3, GL_FLOAT, 0, rec.coloresVertices);
	glVertexPointer(3, GL_FLOAT, 0, rec.cordeneasVertices);
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, rec.indices);
}


void mapa::render() {

	glPushMatrix();

	iniciliarRenderVertexArray();

	renderRectangulo2d(pisoShape);

	for (int i = 0; i < 4; i++) {
		renderRectangulo3d(bordesShape[i]);
	}
	
	finalizarRenderVertexArray();

	glPopMatrix();
}






mapa::~mapa() {
	for (int i = 0; i < this->cant_filas ;i++){
		free(this->estructuraMapa[i]);
	}
	free(estructuraMapa);
}
