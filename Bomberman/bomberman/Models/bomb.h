
#include "SDL.h"
#include "SDL_opengl.h"
#include "../../Utils/renderUtils.h"
#include <iostream>
#include "FreeImage.h"
#include <stdio.h>
#include <conio.h>
#include <GL/glu.h>
#include "../../Utils/mathVector.h"

#ifndef BOMBA
#define BOMBA

class bomba {
	private:
		int posXEnMapa, posYEnMapa;
		float alcanze; //tamaño de la explocion
		retangulo3dss vertices;
		GLuint * textura; 
		float life;

		std::vector<unsigned short> indices;
		std::vector<glm::vec3> verticess;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;

		float scale;
		int isAumentando;

	public:
		bomba(float posXEnMapa, float posYEnMapa, float alcanze, float dirAct);

		bool timer(float deltaT);
		float** explosion_trigg(float** destruct);
		void render();
		int getYenMapa();
		int getXenMapa();
		int getAlcanze();

		bool danioBomba(mathVector posJugador, float** destruct);
};

#endif