#pragma once
#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include "FreeImage.h"
#include <stdio.h>
#include <conio.h>
#include <GL/glu.h>
#include "mathVector.h"
#include "particulas.h"
#include <vector>
#include "renderUtils.h"
#include "random.h"
#include "shapes.h"

#ifndef EXPLOCION
#define EXPLOCION

class explocion {
private:
	float life; // Remaining life of the particle. if < 0 : dead and unused.
	float** rango;
	GLuint texturaFuego;
	GLuint texturaHumo;
	ParticleProps expPart;

public:
	explocion(GLuint texturaf, GLuint texturaH, float** rango);
	~explocion();
	
	bool timer(float deltaT);
	void generateExplocion(int cant, particleGenerator * pSistem);
};



#endif