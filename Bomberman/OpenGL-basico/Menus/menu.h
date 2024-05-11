#pragma once
#include "SDL.h"
#include <stdio.h>
#include <vector>
#include "../util.h"
#include <iostream>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "../renderUtils.h"
#include "../configuraciones.h"

struct MenuItem {
	std::string text;
	SDL_Rect rect;
	SDL_Texture* texture;
};

struct datos {
	TTF_Font* font;
	const std::string& text;
	int x;
	int y;
	SDL_Renderer* renderer;
};

class menu {

private:
	MenuItem * menuItems;
	SDL_Renderer* renderer;
	SDL_Texture* backgroundImage;
	SDL_Texture* manual;
	TTF_Font* font;
	int cursorIndex;
	int w, h;
	int * elecciones;
	bool fin;
	Mix_Music* ambiente_dia = Mix_LoadMUS("assets/sonido/ambiente_dia.mp3");
	Mix_Music* ambiente_nocturno = Mix_LoadMUS("assets/sonido/ambiente_nocturno.mp3");

public:
	menu(int w,int h, SDL_Renderer* r);
	MenuItem createMenuItem(datos t);
	void drawMenuItem(SDL_Renderer* renderer, const MenuItem& menuItem);
	void drawTriangleCursor(SDL_Renderer* renderer, int x, int y, int size);
	void render();
	int eventHandler(SDL_Event evento);
	bool isFinal();
};



