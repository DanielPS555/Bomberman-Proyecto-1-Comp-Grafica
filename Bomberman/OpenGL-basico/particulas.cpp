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
}



void particleGenerator::Emit(const ParticleProps& particleProps)
{
	Particle& particle = m_ParticlePool[m_PoolIndex];

	particle.prop = particleProps;
	particle.active = true;
	particle.lifeRemaining= particleProps.lifeTime;
	
	m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
	
}

void particleGenerator::timer(float deltaT)
{	
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.active)
			continue;

		if (particle.lifeRemaining<= 0.0f)
		{
			particle.active = false;
			continue;
		}
		
		particle.lifeRemaining -= deltaT;
		particle.prop.position = sumar(particle.prop.position, multiplicarPorEscalar(particle.prop.velocity, deltaT / 1000.f)); // 1000ms aplicar completamente


		mathVector parteVelocidadDisminuida = multiplicarPorEscalar(particle.prop.velocity, deltaT / 1000.f);
		mathVector parteVelocidadNoAfectada = multiplicarPorEscalar(particle.prop.velocity, 1 - deltaT / 1000.f);
		parteVelocidadDisminuida = multiplicar(parteVelocidadDisminuida, particle.prop.velocityVariation);

		particle.prop.velocity = sumar(parteVelocidadDisminuida, parteVelocidadNoAfectada); // 1000ms aplicar completamente
	}
}

void particleGenerator::render()
{

	for (auto& particle : m_ParticlePool)
	{	

		if (!particle.active) {
			continue;
		}

		float w = 1;
		GLfloat pos[12] = { -w, 0, -w,
							 w, 0, -w,
							 w,  0, w,
							-w,  0, w 
		};
		
		GLfloat normal[3] = { 0.f,0.f,1.f };


		float totalAplicable = particle.prop.lifeTime - particle.prop.t_inicio;

		float tiempoTranscurido = particle.lifeRemaining - particle.prop.t_inicio;
		if (tiempoTranscurido < 0.0f) {
			tiempoTranscurido = 0.0f;
		}

		float t = (totalAplicable - tiempoTranscurido) /totalAplicable;


		mathVector colorbase = { particle.prop.colorBegin[0], particle.prop.colorBegin[1], particle.prop.colorBegin[2] };
		mathVector colorFinal = { particle.prop.colorEnd[0], particle.prop.colorEnd[1], particle.prop.colorEnd[2] };



		mathVector v =  interpolarVectores(colorbase, colorFinal,t);

		GLfloat  color[4] = { v.x,v.y,v.z, t * particle.prop.colorEnd[3]  + (1 - t) * particle.prop.colorBegin[3]};
		Rectangulo2d<1>* rect = new Rectangulo2d<1>(pos, normal, color);



		glPushMatrix();

		iniciliarRenderVertexArray();
		prepareRender();

		mathVector posicionParticularAbsoluta = { particle.prop.position.x, particle.prop.position.y, particle.prop.position.z };
		mathVector posicionJugadorAbsoluta = player->getPosicionEnMapa();
		
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
		

		float aumento = t * particle.prop.sizeEnd + (1 - t) * particle.prop.sizeBegin;

		glTranslatef(posicionParticularAbsoluta.x, posicionParticularAbsoluta.y, posicionParticularAbsoluta.z);		
		glRotatef(grad, pv.x, pv.y, pv.z);
		glScalef(aumento, aumento, aumento);


		glDisable(GL_LIGHTING);
		rect->renderConPuntoIntermediosYTextura(particle.prop.textura);
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


