#include "mapa.h"
#include "stdio.h"
#include "stdlib.h"


using namespace std;


mapa::mapa(int cant_filas, int cant_columnas) {
	this->cant_filas = cant_filas;
	this->cant_columnas = cant_columnas;
	alturaReal = cant_filas    * LARGO_UNIDAD;
	anchoReal  = cant_columnas * LARGO_UNIDAD;


	pisoShape = {
		 { 0.,0.,0.			,anchoReal,0.,0.,		anchoReal,alturaReal,0.,		0.,alturaReal,0. }    // vertices
		,{0,0,1}																						  //Norma
		,{ 1.,0.,1.,		 1.,1.,1.,              1.,0.,1.,					    1.,1.,1.         }	  // colores	
		,{0,1,2,3}																						  // indices
	};


	// TODO encpsular esta logica dentro de un utils.h    ,  Incluir colores, LAS NORMALES Y ORDEN DE LAS CARAS DEBERIA SER AUTOMATICO
	// Desde ACA

	float cordInf[8][3] = {
		{-LARGO_UNIDAD				, -LARGO_UNIDAD			, 0				},
		{anchoReal + LARGO_UNIDAD	, -LARGO_UNIDAD			, 0				},
		{anchoReal + LARGO_UNIDAD	, -LARGO_UNIDAD			, ALTURA_PARED	},
		{-LARGO_UNIDAD				, -LARGO_UNIDAD			, ALTURA_PARED	},
		{-LARGO_UNIDAD				, 0						, ALTURA_PARED	},
		{-LARGO_UNIDAD				, 0						, 0				},
		{anchoReal + LARGO_UNIDAD	, 0						, 0				},
		{anchoReal + LARGO_UNIDAD	, 0						, ALTURA_PARED  }
	};


	bordesShape[0] = {
		{
			cordInf[0][0],cordInf[0][1],cordInf[0][2],	cordInf[1][0],cordInf[1][1],cordInf[1][2],	cordInf[2][0],cordInf[2][1],cordInf[2][2],	cordInf[3][0],cordInf[3][1],cordInf[3][2], //Cara sur
			cordInf[3][0],cordInf[3][1],cordInf[3][2],	cordInf[4][0],cordInf[4][1],cordInf[4][2],	cordInf[5][0],cordInf[5][1],cordInf[5][2],	cordInf[0][0],cordInf[0][1],cordInf[0][2], //Cara oeste
			cordInf[3][0],cordInf[3][1],cordInf[3][2],	cordInf[2][0],cordInf[2][1],cordInf[2][2],	cordInf[7][0],cordInf[7][1],cordInf[7][2],	cordInf[4][0],cordInf[4][1],cordInf[4][2], //Cara superior
			cordInf[5][0],cordInf[5][1],cordInf[5][2],	cordInf[4][0],cordInf[4][1],cordInf[4][2],	cordInf[7][0],cordInf[7][1],cordInf[7][2],	cordInf[3][0],cordInf[3][1],cordInf[3][2], //Cara norte
			cordInf[1][0],cordInf[1][1],cordInf[1][2],	cordInf[6][0],cordInf[6][1],cordInf[6][2],	cordInf[7][0],cordInf[7][1],cordInf[7][2],	cordInf[2][0],cordInf[2][1],cordInf[2][2], //Cara este
			cordInf[0][0],cordInf[0][1],cordInf[0][2],	cordInf[5][0],cordInf[5][1],cordInf[5][2],	cordInf[6][0],cordInf[6][1],cordInf[6][2],	cordInf[1][0],cordInf[1][1],cordInf[1][2], //Cara inferior
		},
		{
			0	,-1	,0,
			-1	,0	,0,
			0	,0	,1,
			0	,1	,0,
			1	,0	,0,
			0	,0	,-1
		},
		{
			0,0,1,	0,0,1,	0,0,1,	0,0,1,
			0,0,1,	0,0,1,	0,0,1,	0,0,1,
			0,0,1,	0,0,1,	0,0,1,	0,0,1,
			0,0,1,	0,0,1,	0,0,1,	0,0,1,
			0,0,1,	0,0,1,	0,0,1,	0,0,1,
			0,0,1,	0,0,1,	0,0,1,	0,0,1,
		},
		{	0,1,2,3,
			4,5,6,7,
			8,9,10,11,
			12,13,14,15,
			16,17,18,19,
			20,21,22,23
		}
	};


	// HASTA ACA


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
	iniciliarRenderVertexArray();

	renderRectangulo2d(pisoShape);

	renderRectangulo3d(bordesShape[0]);
	
	finalizarRenderVertexArray();
}






mapa::~mapa() {
	for (int i = 0; i < this->cant_filas ;i++){
		free(this->estructuraMapa[i]);
	}
	free(estructuraMapa);
}
