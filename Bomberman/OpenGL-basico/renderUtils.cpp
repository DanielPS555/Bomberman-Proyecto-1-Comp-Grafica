#include "renderUtils.h"


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