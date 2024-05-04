#include "renderUtils.h"

bool mostrarTexturas = 1;

void iniciliarRenderVertexArray() {
	if (mostrarTexturas == 1) {
		glEnable(GL_TEXTURE_2D);
	}
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void finalizarRenderVertexArray() {
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
}


void renderRectangulo2d(retangulo2d rec,GLuint textura) {
	glBindTexture(GL_TEXTURE_2D, textura);
	glNormalPointer(GL_FLOAT, 0, rec.normales);
	glColorPointer(3, GL_FLOAT, 0, rec.coloresVertices);
	glVertexPointer(3, GL_FLOAT, 0, rec.cordeneasVertices);
	glTexCoordPointer(2, GL_FLOAT, 0, rec.texturas);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, rec.indices);
}

void renderRectangulo2d(retangulo2d rec, GLuint textura, float colores[12]) {
	glBindTexture(GL_TEXTURE_2D, textura);
	glNormalPointer(GL_FLOAT, 0, rec.normales);
	glColorPointer(3, GL_FLOAT, 0, colores);
	glVertexPointer(3, GL_FLOAT, 0, rec.cordeneasVertices);
	glTexCoordPointer(2, GL_FLOAT, 0, rec.texturas);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, rec.indices);
}


void renderRectangulo2d(retangulo2d rec) {
	glBindTexture(GL_TEXTURE_2D, {});
	glNormalPointer(GL_FLOAT, 0, rec.normales);
	glColorPointer(3, GL_FLOAT, 0, rec.coloresVertices);
	glVertexPointer(3, GL_FLOAT, 0, rec.cordeneasVertices);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, rec.indices);
}

void renderRectangulo2d(retanguloTransparente2d rec) {
	glBindTexture(GL_TEXTURE_2D, {});
	glNormalPointer(GL_FLOAT, 0, rec.normales);
	glColorPointer(4, GL_FLOAT, 0, rec.coloresVertices);
	glVertexPointer(3, GL_FLOAT, 0, rec.cordeneasVertices);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, rec.indices);
}

void renderRectangulo2d(retanguloTransparente2d rec, GLuint textura) {
	glBindTexture(GL_TEXTURE_2D, textura);
	glNormalPointer(GL_FLOAT, 0, rec.normales);
	glColorPointer(4, GL_FLOAT, 0, rec.coloresVertices);
	glVertexPointer(3, GL_FLOAT, 0, rec.cordeneasVertices);
	glTexCoordPointer(2, GL_FLOAT, 0, rec.texturas);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, rec.indices);
}

void renderRectangulo3d(retangulo3d rec, GLuint textura) {
	glBindTexture(GL_TEXTURE_2D, textura);
	glNormalPointer(GL_FLOAT, 0, rec.normales);
	glColorPointer(3, GL_FLOAT, 0, rec.coloresVertices);
	glVertexPointer(3, GL_FLOAT, 0, rec.cordeneasVertices);
	glTexCoordPointer(2, GL_FLOAT, 0, rec.texturas);
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, rec.indices);
	
}


