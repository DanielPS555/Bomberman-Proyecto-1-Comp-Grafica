#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include <stdio.h>
#include <GL/glu.h>
#include "Rectangulo2d.h"

#ifndef DEF_Rectangulo3d
#define DEF_Rectangulo3d

template<std::size_t N> // Numero de particiones por cara
class Rectangulo3d{
	
private:

	Rectangulo2d<N>* caraSuperior;
	Rectangulo2d<N>* caraInferior;
	
	Rectangulo2d<N>* caraEste;
	Rectangulo2d<N>* caraOeste;
	
	Rectangulo2d<N>* caraNorte;
	Rectangulo2d<N>* caraSur;
	

public:


	//    v4----- v7
	//   /|      /|
	//  v3------v2|
	//  | |     | |
	//  | |v5---|-|v6
	//  |/      |/
	//  v0------v1
	Rectangulo3d(GLfloat vertices[8][3], GLfloat color[3]) {

		GLfloat posCaraSur[12] = {
			vertices[0][0], vertices[0][1], vertices[0][2],
			vertices[1][0], vertices[1][1], vertices[1][2],
			vertices[2][0], vertices[2][1], vertices[2][2],
			vertices[3][0], vertices[3][1], vertices[3][2],
		};
		GLfloat normalSur[3] = { 0.0f ,-1.0f ,0.0f };

		GLfloat posCaraOeste[12] = {
			vertices[3][0], vertices[3][1], vertices[3][2],
			vertices[4][0], vertices[4][1], vertices[4][2],
			vertices[5][0], vertices[5][1], vertices[5][2],
			vertices[0][0], vertices[0][1], vertices[0][2]
		};
		GLfloat normalOeste[3] = { -1.0f, 0.0f, 0.0f };

		GLfloat posCaraSuperior[12] = {
			vertices[3][0], vertices[3][1], vertices[3][2],
			vertices[2][0], vertices[2][1], vertices[2][2],
			vertices[7][0], vertices[7][1], vertices[7][2],
			vertices[4][0], vertices[4][1], vertices[4][2]
		};
		GLfloat normalSuperior[3] = { 0.0f, 0.0f, 1.0f };

		GLfloat posCaraNorte[12] = {
			vertices[5][0], vertices[5][1], vertices[5][2],
			vertices[4][0], vertices[4][1], vertices[4][2],
			vertices[7][0], vertices[7][1], vertices[7][2],
			vertices[6][0], vertices[6][1], vertices[6][2]
		};
		GLfloat normalNorte[3] = { 0.0f, 1.0f, 0.0f};

		GLfloat posCaraEste[12] = {
			vertices[1][0], vertices[1][1], vertices[1][2],
			vertices[6][0], vertices[6][1], vertices[6][2],
			vertices[7][0], vertices[7][1], vertices[7][2],
			vertices[2][0], vertices[2][1], vertices[2][2],
		};
		GLfloat normalEste[3] = { 1.0f ,0.0f, 0.0f };

		GLfloat posCaraInferior[12] = {
			vertices[0][0], vertices[0][1], vertices[0][2],
			vertices[5][0], vertices[5][1], vertices[5][2],
			vertices[6][0], vertices[6][1], vertices[6][2],
			vertices[1][0], vertices[1][1], vertices[1][2]
		};
		GLfloat normalInferior[3] = { 0.0f, 0.0f, -1.0f };

		caraSuperior = new Rectangulo2d<N>(posCaraSuperior, normalSuperior, color);
		caraInferior = new Rectangulo2d<N>(posCaraInferior, normalInferior, color);
		caraEste	 = new Rectangulo2d<N>(posCaraEste, normalEste, color);
		caraOeste	 = new Rectangulo2d<N>(posCaraOeste, normalOeste, color );
		caraNorte	 = new Rectangulo2d<N>(posCaraNorte, normalNorte, color );
		caraSur		 = new Rectangulo2d<N>(posCaraSur, normalSur, color );
	}

	~Rectangulo3d() {
		free(caraSuperior);
		free(caraInferior);

		free(caraEste);
		free(caraOeste);

		free(caraNorte);
		free(caraSur);
	}

	void render(GLuint textura) {
		caraSuperior->renderConPuntoIntermediosYTextura(textura);
		caraInferior->renderConPuntoIntermediosYTextura(textura);
		caraEste->renderConPuntoIntermediosYTextura(textura);
		caraOeste->renderConPuntoIntermediosYTextura(textura);
		caraNorte->renderConPuntoIntermediosYTextura(textura);
		caraSur->renderConPuntoIntermediosYTextura(textura);
	}

};

#endif