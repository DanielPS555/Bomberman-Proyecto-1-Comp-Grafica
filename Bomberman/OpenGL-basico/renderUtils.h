#include "shapes.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include <stdio.h>
#include <GL/glu.h>
#include "mathVector.h"



#ifndef RENDER_UTILS
#define RENDER_UTILS

extern bool mostrarTexturas;

void iniciliarRenderVertexArray();
void finalizarRenderVertexArray();

template<size_t N>
void renderRectangulo2d(retangulo2d<N> rec, GLuint textura) {
	glBindTexture(GL_TEXTURE_2D, textura);
	glNormalPointer(GL_FLOAT, 0, rec.normales);
	glColorPointer(3, GL_FLOAT, 0, rec.color);
	glVertexPointer(3, GL_FLOAT, 0, rec.cordeneasVerticesBordes);
	glTexCoordPointer(2, GL_FLOAT, 0, rec.corrdenadasTexturasBordes);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, rec.indices);
}

template<size_t N>
void renderRectangulo2d(retangulo2d<N> rec) {
	glBindTexture(GL_TEXTURE_2D, {});
	glNormalPointer(GL_FLOAT, 0, rec.normales);
	glColorPointer(3, GL_FLOAT, 0, rec.color);
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

template<size_t N>
void renderRectangulo2dIntermedios(retangulo2d<N> rec, GLuint textura) {
	glBindTexture(GL_TEXTURE_2D, textura);
	for (int i = 0; i < N*N; i++) {

		GLfloat n[12];
		n[0 ] = rec.normales[0];
		n[1 ] = rec.normales[1];
		n[2 ] = rec.normales[2];

		n[3 ] = rec.normales[0];
		n[4 ] = rec.normales[1];
		n[5 ] = rec.normales[2];

		n[6 ] = rec.normales[0];
		n[7 ] = rec.normales[1];
		n[8 ] = rec.normales[2];

		n[9 ] = rec.normales[0];
		n[10] = rec.normales[1];
		n[11] = rec.normales[2];

		GLfloat c[12];
		c[ 0] = rec.color[0];
		c[ 1] = rec.color[1];
		c[ 2] = rec.color[2];

		c[ 3] = rec.color[0];
		c[ 4] = rec.color[1];
		c[ 5] = rec.color[2];

		c[ 6] = rec.color[0];
		c[ 7] = rec.color[1];
		c[ 8] = rec.color[2];

		c[9] = rec.color[0];
		c[10] = rec.color[1];
		c[11] = rec.color[2];

		glNormalPointer(GL_FLOAT, 0, n);
		glColorPointer(3, GL_FLOAT, 0, c);
		glVertexPointer(3, GL_FLOAT, 0, rec.verticesIntermedios + i*12 );
		glTexCoordPointer(2, GL_FLOAT, 0, rec.corrdenadasTexturasIntermedias + i * 8);
		glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, rec.indices);
	}
}

void renderRectangulo3d(retangulo3d rec, GLuint textura);


template<size_t N>
void generarVerticesIntermedios( (retangulo2d<N>)* datos) {
	float delta = 1.0f / N;

	//Datos x, y, z del borde inferior izquiero del cuadrado
	mathVector inf_isq = {
							datos..corss[0 * 3 + 0],
							datos.cordeneasVerticesBordes[0 * 3 + 1],
							datos.cordeneasVerticesBordes[0 * 3 + 2]
	};

	
	mathVector inf_der = { 
						   datos.cordeneasVerticesBordes[1 * 3 + 0],
						   datos.cordeneasVerticesBordes[1 * 3 + 1],
						   datos.cordeneasVerticesBordes[1 * 3 + 2] 
	};

	mathVector sup_der = {
							datos.cordeneasVerticesBordes[2 * 3 + 0],
							datos.cordeneasVerticesBordes[2 * 3 + 1],
							datos.cordeneasVerticesBordes[2 * 3 + 2]
	};

	mathVector sup_isq = {
							datos.cordeneasVerticesBordes[3 * 3 + 0],
							datos.cordeneasVerticesBordes[3 * 3 + 1],
							datos.cordeneasVerticesBordes[3 * 3 + 2]
	};
	

	unsigned int contadorGeneral = 0;

	for (int y =0; y < N; y += 1) {

		float itery = (float) y / (float) N;

		mathVector linea_rastr_i_isq = interpolarVectores(inf_isq, sup_isq, itery);
		mathVector linea_rastre_i_der = interpolarVectores(inf_der, sup_der, itery);

		mathVector linea_rastr_i2_isq = interpolarVectores(inf_isq, sup_isq, itery + delta);
		mathVector linea_rastre_i2_der = interpolarVectores(inf_der, sup_der, itery + delta);

		for (int x = 0; x< N; x+= 1) {

			float iterx = (float)x / (float)N;

			mathVector b_inf_isq = interpolarVectores(linea_rastr_i_isq, linea_rastre_i_der, iterx);
			mathVector b_inf_der = interpolarVectores(linea_rastr_i_isq, linea_rastre_i_der, iterx + delta);

			mathVector b_sup_isq = interpolarVectores(linea_rastr_i2_isq, linea_rastre_i2_der, iterx);
			mathVector b_sup_der = interpolarVectores(linea_rastr_i2_isq, linea_rastre_i2_der, iterx + delta);

			datos.verticesIntermedios[contadorGeneral * 12 + 0] = b_inf_isq.x;
			datos.verticesIntermedios[contadorGeneral * 12 + 1]  = b_inf_isq.y;
			datos.verticesIntermedios[contadorGeneral * 12 + 2]  = b_inf_isq.z;

			datos.verticesIntermedios[contadorGeneral * 12 + 3]  = b_inf_der.x;
			datos.verticesIntermedios[contadorGeneral * 12 + 4]  = b_inf_der.y;
			datos.verticesIntermedios[contadorGeneral * 12 + 5]  = b_inf_der.z;

			datos.verticesIntermedios[contadorGeneral * 12 + 6]  = b_sup_der.x;
			datos.verticesIntermedios[contadorGeneral * 12 + 7]  = b_sup_der.y;
			datos.verticesIntermedios[contadorGeneral * 12 + 8]  = b_sup_der.z;

			datos.verticesIntermedios[contadorGeneral * 12 + 9]  = b_sup_isq.x;
			datos.verticesIntermedios[contadorGeneral * 12 + 10] = b_sup_isq.y;
			datos.verticesIntermedios[contadorGeneral * 12 + 11] = b_sup_isq.z;



			datos.corrdenadasTexturasIntermedias[contadorGeneral * 8 + 0] = iterx;
			datos.corrdenadasTexturasIntermedias[contadorGeneral * 8 + 1] = itery;

			datos.corrdenadasTexturasIntermedias[contadorGeneral * 8 + 2] = iterx + delta;
			datos.corrdenadasTexturasIntermedias[contadorGeneral * 8 + 3] = itery;

			datos.corrdenadasTexturasIntermedias[contadorGeneral * 8 + 4] = iterx + delta;
			datos.corrdenadasTexturasIntermedias[contadorGeneral * 8 + 5] = itery + delta;

			datos.corrdenadasTexturasIntermedias[contadorGeneral * 8 + 6] = iterx;
			datos.corrdenadasTexturasIntermedias[contadorGeneral * 8 + 7] = itery + delta;

			

			contadorGeneral++;
		}		
	}
	
	return datos;
}


#endif