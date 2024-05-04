#include "shapes.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include <stdio.h>
#include <GL/glu.h>



#ifndef RENDER_UTILS
#define RENDER_UTILS

extern bool mostrarTexturas;

void iniciliarRenderVertexArray();
void finalizarRenderVertexArray();

template<size_t N>
void renderRectangulo2d(retangulo2d<N> rec, GLuint textura) {
	glBindTexture(GL_TEXTURE_2D, textura);
	glNormalPointer(GL_FLOAT, 0, rec.normales);
	glColorPointer(3, GL_FLOAT, 0, rec.coloresVertices);
	glVertexPointer(3, GL_FLOAT, 0, rec.cordeneasVerticesBordes);
	glTexCoordPointer(2, GL_FLOAT, 0, rec.corrdenadasTexturasBordes);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, rec.indices);
}

template<size_t N>
void renderRectangulo2d(retangulo2d<N> rec) {
	glBindTexture(GL_TEXTURE_2D, {});
	glNormalPointer(GL_FLOAT, 0, rec.normales);
	glColorPointer(3, GL_FLOAT, 0, rec.coloresVertices);
	glVertexPointer(3, GL_FLOAT, 0, rec.cordeneasVerticesBordes);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, rec.indices);
}


void renderRectangulo2d(retanguloTransparente2d rec);


void renderRectangulo2d(retanguloTransparente2d rec, GLuint textura);

template<size_t N>
void renderRectangulo2d(retangulo2d<N> rec, GLuint textura,  float colores[12]) {
	glBindTexture(GL_TEXTURE_2D, textura);
	glNormalPointer(GL_FLOAT, 0, rec.normales);
	glColorPointer(3, GL_FLOAT, 0, colores);
	glVertexPointer(3, GL_FLOAT, 0, rec.cordeneasVerticesBordes);
	glTexCoordPointer(2, GL_FLOAT, 0, rec.corrdenadasTexturasBordes);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, rec.indices);
}

void renderRectangulo3d(retangulo3d rec, GLuint textura);

#endif