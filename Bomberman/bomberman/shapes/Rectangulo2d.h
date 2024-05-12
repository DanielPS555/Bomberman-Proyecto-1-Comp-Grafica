#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include <stdio.h>
#include <GL/glu.h>

#ifndef DEF_Rectangulo2d
#define DEF_Rectangulo2d

template<std:: size_t N> // Numero de particiones por cara 
class Rectangulo2d {


private:
	float repeticionesTexturaX;
	float repeticionesTexturaY;
	GLfloat cordeneasVerticesBordes[4 * 3];   //Ejemplo: 1,1,1,  -1,1,1,  -1,-1,1,  1,-1,1,  ==> v0-v1-v2-v3
	GLfloat normales[4*3];
	GLfloat color[4 * 4];
	GLfloat corrdenadasTexturasBordes[4 * 2];
	GLfloat verticesIntermedios[N * N * 4 * 3];
	GLfloat corrdenadasTexturasIntermedias[N * N * 4 * 2];

	void generarVerticesIntermedios() {
		float delta = 1.0f / N;

		//Datos x, y, z del borde inferior izquiero del cuadrado
		mathVector inf_isq = {
								cordeneasVerticesBordes[0 * 3 + 0],
								cordeneasVerticesBordes[0 * 3 + 1],
								cordeneasVerticesBordes[0 * 3 + 2]
		};


		mathVector inf_der = {
							   cordeneasVerticesBordes[1 * 3 + 0],
							   cordeneasVerticesBordes[1 * 3 + 1],
							   cordeneasVerticesBordes[1 * 3 + 2]
		};

		mathVector sup_der = {
								cordeneasVerticesBordes[2 * 3 + 0],
								cordeneasVerticesBordes[2 * 3 + 1],
								cordeneasVerticesBordes[2 * 3 + 2]
		};

		mathVector sup_isq = {
								cordeneasVerticesBordes[3 * 3 + 0],
								cordeneasVerticesBordes[3 * 3 + 1],
								cordeneasVerticesBordes[3 * 3 + 2]
		};


		unsigned int contadorGeneral = 0;

		for (int y = 0; y < N; y += 1) {

			float itery = (float)y / (float)N;

			mathVector linea_rastr_i_isq = interpolarVectores(inf_isq, sup_isq, itery);
			mathVector linea_rastre_i_der = interpolarVectores(inf_der, sup_der, itery);

			mathVector linea_rastr_i2_isq = interpolarVectores(inf_isq, sup_isq, itery + delta);
			mathVector linea_rastre_i2_der = interpolarVectores(inf_der, sup_der, itery + delta);

			for (int x = 0; x < N; x += 1) {

				float iterx = (float)x / (float)N;

				mathVector b_inf_isq = interpolarVectores(linea_rastr_i_isq, linea_rastre_i_der, iterx);
				mathVector b_inf_der = interpolarVectores(linea_rastr_i_isq, linea_rastre_i_der, iterx + delta);

				mathVector b_sup_isq = interpolarVectores(linea_rastr_i2_isq, linea_rastre_i2_der, iterx);
				mathVector b_sup_der = interpolarVectores(linea_rastr_i2_isq, linea_rastre_i2_der, iterx + delta);

				verticesIntermedios[contadorGeneral * 12 + 0] = b_inf_isq.x;
				verticesIntermedios[contadorGeneral * 12 + 1] = b_inf_isq.y;
				verticesIntermedios[contadorGeneral * 12 + 2] = b_inf_isq.z;

				verticesIntermedios[contadorGeneral * 12 + 3] = b_inf_der.x;
				verticesIntermedios[contadorGeneral * 12 + 4] = b_inf_der.y;
				verticesIntermedios[contadorGeneral * 12 + 5] = b_inf_der.z;

				verticesIntermedios[contadorGeneral * 12 + 6] = b_sup_der.x;
				verticesIntermedios[contadorGeneral * 12 + 7] = b_sup_der.y;
				verticesIntermedios[contadorGeneral * 12 + 8] = b_sup_der.z;

				verticesIntermedios[contadorGeneral * 12 + 9] = b_sup_isq.x;
				verticesIntermedios[contadorGeneral * 12 + 10] = b_sup_isq.y;
				verticesIntermedios[contadorGeneral * 12 + 11] = b_sup_isq.z;


				corrdenadasTexturasIntermedias[contadorGeneral * 8 + 0] = (iterx) *repeticionesTexturaX;
				corrdenadasTexturasIntermedias[contadorGeneral * 8 + 1] = itery * repeticionesTexturaY;

				corrdenadasTexturasIntermedias[contadorGeneral * 8 + 2] = (iterx + delta) * repeticionesTexturaX;
				corrdenadasTexturasIntermedias[contadorGeneral * 8 + 3] = (itery) *repeticionesTexturaY;

				corrdenadasTexturasIntermedias[contadorGeneral * 8 + 4] = (iterx + delta) * repeticionesTexturaX;
				corrdenadasTexturasIntermedias[contadorGeneral * 8 + 5] = (itery + delta) * repeticionesTexturaY;

				corrdenadasTexturasIntermedias[contadorGeneral * 8 + 6] = iterx * repeticionesTexturaX;
				corrdenadasTexturasIntermedias[contadorGeneral * 8 + 7] = (itery + delta) * repeticionesTexturaY;

				contadorGeneral++;
			}
		}
	}


public:

	Rectangulo2d(GLfloat corr[4 * 3], GLfloat nor[4 * 3], GLfloat c[4], float repeticionesTexturaX, float repeticionesTexturaY) {

		this->repeticionesTexturaX = repeticionesTexturaX;
		this->repeticionesTexturaY = repeticionesTexturaY;

		GLfloat corrTextruras[12] = { 0.f, 0.f,	1.f, 0.f,	1.f, 1.f,	0.f,1.f };

		for (int i = 0; i < 12; i++) {
			cordeneasVerticesBordes[i] = corr[i];

		}

		for (int i = 0; i < 7; i++) {
			corrdenadasTexturasBordes[i] = corrTextruras[i];
		}


		for (int i = 0; i < 4; i++) {
			normales[i * 3 + 0] = nor[0]; normales[i * 3 + 1] = nor[1];  normales[i * 3 + 2] = nor[2];
			color[i * 4 + 0] = c[0]; color[i * 4 + 1] = c[1];  color[i * 4 + 2] = c[2]; color[i * 4 + 3] = c[3];
		}

		generarVerticesIntermedios();

	}

	Rectangulo2d(GLfloat corr[4 * 3], GLfloat nor[4 * 3], GLfloat cil[4], GLfloat cir[4], GLfloat csr[4], GLfloat csl[4], float repeticionesTexturaX, float repeticionesTexturaY) {

		this->repeticionesTexturaX = repeticionesTexturaX;
		this->repeticionesTexturaY = repeticionesTexturaY;

		GLfloat corrTextruras[12] = { 0.f, 0.f,	1.f, 0.f,	1.f, 1.f,	0.f,1.f };

		for (int i = 0; i < 12; i++) {
			cordeneasVerticesBordes[i] = corr[i];

		}

		for (int i = 0; i < 7; i++) {
			corrdenadasTexturasBordes[i] = corrTextruras[i];
		}


		for (int i = 0; i < 4; i++) {
			normales[i * 3 + 0] = nor[0]; normales[i * 3 + 1] = nor[1];  normales[i * 3 + 2] = nor[2];			
		}

		color[0 * 4 + 0] = cil[0]; color[0 * 4 + 1] = cil[1];  color[0 * 4 + 2] = cil[2]; color[0 * 4 + 3] = cil[3];
		color[1 * 4 + 0] = cir[0]; color[1 * 4 + 1] = cir[1];  color[1 * 4 + 2] = cir[2]; color[1 * 4 + 3] = cir[3];
		color[2 * 4 + 0] = csr[0]; color[2 * 4 + 1] = csr[1];  color[2 * 4 + 2] = csr[2]; color[2 * 4 + 3] = csr[3];
		color[3 * 4 + 0] = csl[0]; color[3 * 4 + 1] = csl[1];  color[3 * 4 + 2] = csl[2]; color[3 * 4 + 3] = csl[3];


		generarVerticesIntermedios();

	}

	
	void renderConPuntoIntermediosYTextura( GLuint textura) {
		glBindTexture(GL_TEXTURE_2D, textura);
		for (int i = 0; i < N * N; i++) {
			glNormalPointer(GL_FLOAT, 0, normales);
			glColorPointer(4, GL_FLOAT, 0, color);
			glVertexPointer(3, GL_FLOAT, 0, verticesIntermedios+ i * 12);
			glTexCoordPointer(2, GL_FLOAT, 0, corrdenadasTexturasIntermedias+ i * 8);

			GLubyte indices[4] = {0,1,2,3};
			glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, indices);
		}
	}

	void renderConPuntoIntermediosYTexturaYColor(GLuint textura, GLfloat coloresL[16]) {
		glBindTexture(GL_TEXTURE_2D, textura);
		for (int i = 0; i < N * N; i++) {
			glNormalPointer(GL_FLOAT, 0, normales);
			glColorPointer(4, GL_FLOAT, 0, coloresL);
			glVertexPointer(3, GL_FLOAT, 0, verticesIntermedios + i * 12);
			glTexCoordPointer(2, GL_FLOAT, 0, corrdenadasTexturasIntermedias + i * 8);

			GLubyte indices[4] = { 0,1,2,3 };
			glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, indices);
		}
	}

};

#endif	




