#include "particulas.h"
#include "random.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include "FreeImage.h"
#include "chrono"
#include <stdio.h>
#include <conio.h>
#include <GL/glu.h>
#include "util.h"
#include "shapes.h"
#include "renderUtils.h"

particleGenerator::particleGenerator(float grav, float life)
{
	m_ParticlePool.resize(1000);
	this->grav = grav;
	this->life = life;
}



void particleGenerator::Emit(const ParticleProps& particleProps)
{
	Particle& particle = m_ParticlePool[m_PoolIndex];
	particle.Active = true;
	particle.Position = particleProps.Position;
	particle.Rotation = 0;

	// Velocity
	particle.Velocity = particleProps.Velocity;
	particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
	particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);
	particle.Velocity.z += particleProps.VelocityVariation.z * (Random::Float() - 0.5f);

	// Color
	particle.ColorBegin[0] = particleProps.ColorBegin[0];
	particle.ColorBegin[1] = particleProps.ColorBegin[1];
	particle.ColorBegin[2] = particleProps.ColorBegin[2];
	particle.ColorBegin[3] = particleProps.ColorBegin[3];
	particle.ColorEnd[0] = particleProps.ColorEnd[0];
	particle.ColorEnd[1] = particleProps.ColorEnd[1];
	particle.ColorEnd[2] = particleProps.ColorEnd[2];
	particle.ColorEnd[3] = particleProps.ColorEnd[3];

	particle.LifeTime = particleProps.LifeTime;
	particle.LifeRemaining = particleProps.LifeTime;
	particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
	particle.SizeEnd = particleProps.SizeEnd;
	particle.weight = particleProps.weight;

	m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
}

void particleGenerator::timer(float deltaT)
{	
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active)
			continue;

		if (particle.LifeRemaining <= 0.0f)
		{
			particle.Active = false;
			continue;
		}
		particle.Velocity.z = particle.Velocity.z * particle.weight * this->grav;
		particle.LifeRemaining -= deltaT;
		particle.Position = sumar(particle.Position ,multiplicarPorEscalar(particle.Velocity, deltaT));
		particle.Rotation += 0 * deltaT;
	}
}

void particleGenerator::render()
{

	retangulo2dss<1> rect = {
	 {-0.5f, -0.5f, 0.0f,
	   0.5f, -0.5f, 0.0f,
	  -0.5f, 0.5f, 0.0f,
	   0.5f, 0.5f, 0.0f}    // vertices
	,{0,0,1}				//Norma
	,{ 1.,1.,1.}	  // colores	
	,{0,1,2,3}																						  // indices
	,{0.0,0.0,0.0,1.0,1.0,1.0,1.0,0.0}
	};

	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active) {
			continue;
		}

		// Fade away particles
		//float life = particle.LifeRemaining / particle.LifeTime;
		//glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
		//color.a = color.a * life;

		//float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

		// Render
		//glm::mat4 transform = glm::translate(glm::mat4(1.0f), { particle.Position.x, particle.Position.y, particle.position.z })
		//	* glm::rotate(glm::mat4(1.0f), particle.Rotation, { 0.0f, 0.0f, 1.0f })
		//	* glm::scale(glm::mat4(1.0f), { size, size, 1.0f });
		//glUniformMatrix4fv(m_ParticleShaderTransform, 1, GL_FALSE, glm::value_ptr(transform));
		//glUniform4fv(m_ParticleShaderColor, 1, glm::value_ptr(color));
		//glBindVertexArray(m_QuadVA);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		glPushMatrix();

		iniciliarRenderVertexArray();
		glTranslatef(particle.Position.x, particle.Position.y, particle.Position.z);
		glScalef(particle.SizeBegin, 0, particle.SizeBegin);

		renderRectangulo2d(rect, particle.textura);
		finalizarRenderVertexArray();

		glPopMatrix();
	}
}

void particleGenerator::prepareRender()
{
	
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC2_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glDepthMask(false); 
}

void particleGenerator::finishRender()
{
}


