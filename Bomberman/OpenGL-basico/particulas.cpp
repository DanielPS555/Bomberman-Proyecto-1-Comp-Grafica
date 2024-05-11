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
#include "jugador.h"


particleGenerator::particleGenerator(float grav, float life, jugador* p, modoVisualizacion* m)
{
	modoV = m;
	player = p;
	m_ParticlePool.resize(1000);
	this->grav = grav;
	this->life = life;
	textura = inicializarTexturaPng("assets/f2.png");
}



void particleGenerator::Emit(const ParticleProps& particleProps)
{
	Particle& particle = m_ParticlePool[m_PoolIndex];
	particle.Active = true;
	particle.Position = particleProps.Position;
	particle.Rotation = 0;

	// Velocity
	particle.Velocity = particleProps.Velocity;
	//particle.Velocity.x += particleProps.VelocityVariation.x; //* (Random::Float() - 0.5f);
	//particle.Velocity.y += particleProps.VelocityVariation.y; //* (Random::Float() - 0.5f);
	//particle.Velocity.z += particleProps.VelocityVariation.z; //* (Random::Float() - 0.5f);

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
	particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation; //* (Random::Float() - 0.5f);
	particle.SizeEnd = particleProps.SizeEnd;
	particle.weight = particleProps.weight;
	particle.distancia_centro = particleProps.distancia_centro;

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
		//particle.Velocity.z = particle.Velocity.z * particle.weight * this->grav;
		particle.LifeRemaining -= deltaT;
		particle.Position = sumar(particle.Position, particle.Velocity);//multiplicarPorEscalar(particle.Velocity, deltaT));
		particle.Velocity = sumar(particle.Velocity, particle.VelocityVariation);
		particle.Rotation += 0 * deltaT;
	}
}

void particleGenerator::render()
{

	for (auto& particle : m_ParticlePool)
	{	
		float w = 2;
		GLfloat pos[12] = { -w, 0, -w,
							 w, 0, -w,
							 w,  0, w,
							-w,  0, w 
		};
		GLfloat colores[4] = { 249.f /256.f ,182.f /256.f,78.f/176.f,1.0f };
		//GLfloat colores[3] = { 203.f / 256.f ,53.f / 256.f,61.f / 176.f };
		GLfloat normal[3] = { 0.f,0.f,1.f };


		mathVector colorbase = {  255.f / 255.f ,255.f / 255.f,255.f / 255.f };
		mathVector colorFinal = { 245.f / 256.f ,149.f / 256.f,0.f / 255.f };
		mathVector gradient = { 203.f / 256.f ,53.f / 256.f,61.f / 176.f };

		float normalized_distance = particle.distancia_centro / 80;

		gradient = multiplicarPorEscalar(gradient, Random::Float() * 0.5);


		mathVector v =  interpolarVectores(colorbase, colorFinal,normalized_distance);
		//v = sumar(v, gradient);
		GLfloat  color[4] = { v.x,v.y,v.z,1 };
		Rectangulo2d<1>* rect = new Rectangulo2d<1>(pos, normal, colores);

		if (!particle.Active) {
			free(rect);
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
		prepareRender();

		

		mathVector posicionParticularAbsoluta = { particle.Position.x, particle.Position.y, particle.Position.z };
		mathVector posicionJugadorAbsoluta = player->getPosicionEnMapa();
		//posicionParticularAbsoluta.z = ALTURA_CAMARA_PRIMERA_PERSONA;
		

		if (modoV->getModoVis() == MODOS_VISUALIZACION_PRIMERA_PERSONA) {
			posicionJugadorAbsoluta = { posicionJugadorAbsoluta.x, posicionJugadorAbsoluta.y, ALTURA_CAMARA_PRIMERA_PERSONA };
		}
		else {
			posicionJugadorAbsoluta = { posicionJugadorAbsoluta.x, posicionJugadorAbsoluta.y, ALTURA_CAMARA_VISTA_ORIGINAL };
		}
		
		mathVector posicionJugadorRelativoAParticula = restar(posicionJugadorAbsoluta, posicionParticularAbsoluta);
		mathVector normaActualParticula = { 0.0f,	1.0f,	0.0f };
		mathVector pv = productoVectorial(normaActualParticula, posicionJugadorRelativoAParticula);

		float anguloRotacion = angulo(normaActualParticula, posicionJugadorRelativoAParticula);
		float grad = (anguloRotacion / M_PI) * 180.0f;
		

		glTranslatef(posicionParticularAbsoluta.x, posicionParticularAbsoluta.y, posicionParticularAbsoluta.z);		
		glRotatef(grad, pv.x, pv.y, pv.z);
 

		glDisable(GL_LIGHTING);
		rect->renderConPuntoIntermediosYTextura(textura);
		glEnable(GL_LIGHTING);
		finalizarRenderVertexArray();

		finishRender();
		glPopMatrix();

		free(rect);
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
	glDepthMask(true);
	glDisable(GL_BLEND);
}


