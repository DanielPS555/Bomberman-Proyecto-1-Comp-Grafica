#pragma once
#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include "FreeImage.h"
#include <stdio.h>
#include <conio.h>
#include <GL/glu.h>
#include <vector>
#include "mathVector.h"
#include "shapes/Rectangulo2d.h"
#include "jugador.h"
#include "visualizacion/modoVisualizacion.h"


#ifndef PARTICULAS
#define PARTICULAS

struct ParticleProps
{
	mathVector Position;
	mathVector Velocity, VelocityVariation;

	float ColorBegin[4], ColorEnd[4];
	float SizeBegin, SizeEnd, SizeVariation;
	float LifeTime = 1.0f;
	float weight;
	GLuint textura;
	float distancia_centro;
};

class particleGenerator {
	private:	
		struct Particle
		{
			mathVector Position;
			mathVector Velocity, VelocityVariation;
			float ColorBegin[4], ColorEnd[4];
			float Rotation;
			float SizeBegin, SizeEnd;
			float weight;

			float LifeTime = 2000;
			float LifeRemaining = 0.0f;
			GLuint textura;
			bool Active = false;
			float distancia_centro;

		};
		

		std::vector<Particle> m_ParticlePool;
		uint32_t m_PoolIndex = 999;
		float life;

		float grav;

		jugador* player;
		modoVisualizacion* modoV;

		GLuint textura;


		//GLuint m_QuadVA = 0;

		//std::unique_ptr<GLCore::Utils::Shader> m_ParticleShader;
		//GLint m_ParticleShaderViewProj, m_ParticleShaderTransform, m_ParticleShaderColor;

		 
	public:
		particleGenerator(float grav, float life, jugador* play, modoVisualizacion* m);

		void timer(float deltaT);
		void Emit(const ParticleProps& particleProps);

		void render();

		void prepareRender();
		void finishRender();
};



#endif