#pragma once
#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include "FreeImage.h"
#include <stdio.h>
#include <conio.h>
#include <GL/glu.h>
#include <vector>
#include "../Utils/mathVector.h"
#include "../shapes/Rectangulo2d.h"
#include "../Models/jugador.h"
#include "../visualizacion/modoVisualizacion.h"


#ifndef PARTICULAS
#define PARTICULAS

struct ParticleProps
{
	mathVector position;
	mathVector velocity, velocityVariation;

	bool useLight7;

	GLfloat colorBegin[4], colorEnd[4];
	float sizeBegin, sizeEnd;
	float lifeTime;
	mathVector origen;
	GLuint textura;
	
	float t_inicio; //Inicio de la interpolacion para el color y el size

};

class particleGenerator {
	private:	
		struct Particle
		{
			ParticleProps prop;

			float lifeRemaining = 0.0f;
			bool active = false;
		};
		

		std::vector<Particle> m_ParticlePool;
		Particle * particuloConLuz;
		uint32_t m_PoolIndex = 1999;
		float life;

		float grav;

		jugador* player;
		modoVisualizacion* modoV;


		//GLuint m_QuadVA = 0;

		//std::unique_ptr<GLCore::Utils::Shader> m_ParticleShader;
		//GLint m_ParticleShaderViewProj, m_ParticleShaderTransform, m_ParticleShaderColor;

		 
	public:
		particleGenerator(float grav, float life, jugador* play, modoVisualizacion* m);

		void timer(float deltaT);
		void Emit(const ParticleProps& particleProps);

		void render();
		void renderLight();

		void prepareRender();
		void finishRender();
};



#endif