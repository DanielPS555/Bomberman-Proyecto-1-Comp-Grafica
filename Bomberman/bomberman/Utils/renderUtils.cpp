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


void renderRectangulo3d(retangulo3dss rec, GLuint textura) {
	glBindTexture(GL_TEXTURE_2D, textura);
	glNormalPointer(GL_FLOAT, 0, rec.normales);
	glColorPointer(3, GL_FLOAT, 0, rec.coloresVertices);
	glVertexPointer(3, GL_FLOAT, 0, rec.cordeneasVertices);
	glTexCoordPointer(2, GL_FLOAT, 0, rec.texturas);
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, rec.indices);
	
}

void render3dObject(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvs, std::vector<glm::vec3> normals, std::vector<unsigned short> indices, GLuint texture) {
	if (mostrarTexturas == 1) {
		glEnable(GL_TEXTURE_2D);
	}
	glBindTexture(GL_TEXTURE_2D, texture);
	glVertexPointer(3, GL_FLOAT, 0, vertices.data());
	glTexCoordPointer(2, GL_FLOAT, 0, uvs.data());
	glNormalPointer(GL_FLOAT, 0, normals.data());

	// Habilitar los punteros
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	// Dibujar el objeto con los índices
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, indices.data());

	// Deshabilitar los punteros
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisable(GL_TEXTURE_2D);
}



