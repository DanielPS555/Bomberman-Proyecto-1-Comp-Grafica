
#include "SDL.h"
#include <stdio.h>
#include <vector>
#include "../util.h"
#include <iostream>
#include <SDL_ttf.h>
#include "../renderUtils.h"
#include "../configuraciones.h"
#include "menu.h"


#ifndef D_gameOverClass
#define D_gameOverClass


class GameOverMenuF {
private:
	MenuItem* menuItems;
	SDL_Renderer* renderer;
	SDL_Texture* backgroundImage;
	SDL_Texture* manual;
	TTF_Font* font;
	int cursorIndex;
	int w, h;
	int* elecciones;
	bool fin, restart;

public:
	GameOverMenuF(int w, int h, SDL_Renderer* r);
	MenuItem createMenuItem(datos t);
	void drawMenuItem(SDL_Renderer* renderer, const MenuItem& menuItem);
	void drawTriangleCursor(SDL_Renderer* renderer, int x, int y, int size);
	void render();
	int eventHandler(SDL_Event evento);
	bool isFinal();
	bool weResist();
};
#endif // !1




