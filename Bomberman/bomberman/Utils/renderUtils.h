#include "../shapes/shapes.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include <stdio.h>
#include <GL/glu.h>
#include "mathVector.h"

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include <stdio.h>
#include <vector>

#ifndef RENDER_UTILS
#define RENDER_UTILS

extern bool mostrarTexturas;
extern GLuint * texturaBomba;

void iniciliarRenderVertexArray();
void finalizarRenderVertexArray();

void renderRectangulo3d(retangulo3dss rec, GLuint textura);
void render3dObject(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvs, std::vector<glm::vec3> normals, std::vector<unsigned short> indices, GLuint textura);
GLuint * getTexturaBomba();
GLuint* getTexturaCaballo();
#endif