#pragma once
#include "SDL.h"
#include <SDL_ttf.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <SDL_ttf.h>
#include "renderUtils.h"
#include "configuraciones.h"

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

public:
	menu(int w,int h, SDL_Window* win);
	MenuItem createMenuItem(datos t);
	TTF_Font* loadFont(const std::string& fontPath, int fontSize);
	void drawMenuItem(SDL_Renderer* renderer, const MenuItem& menuItem);
	void drawTriangleCursor(SDL_Renderer* renderer, int x, int y, int size);
	void render();
	int eventHandler(SDL_Event evento);
};



