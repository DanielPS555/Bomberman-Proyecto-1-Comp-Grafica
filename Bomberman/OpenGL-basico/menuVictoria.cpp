#include "menuVictoria.h"

victoryMenu::victoryMenu(int w, int h, SDL_Renderer* r)
{
	this->w = w;
	this->h = h;
	this->fin = false;
	this->cont = false;
	cursorIndex = 0;
	font = loadFont("letra3.ttf", 48);

	renderer = r;

	int p_inicial_x = w / 2 - 350;
	int p_inicial_y = 550;
	int distancia_y = 50;

	datos info[2] = {
		{ font, "FIGHT ONWARD", p_inicial_x , p_inicial_y + distancia_y, renderer },
		{ font, "ESCAPE THIS TORMENT", w / 2 , p_inicial_y + distancia_y, renderer},
	};

	menuItems = new MenuItem[2];

	for (int i = 0; i < 2; i++) {
		menuItems[i] = createMenuItem(info[i]);
	}

	elecciones = new int[2];
	elecciones[0] = 1;
	elecciones[1] = 1;

	SDL_Surface* backgroundSurface = SDL_LoadBMP("bomberman.bmp");
	SDL_Surface* manualSurface = SDL_LoadBMP("instrucciones.bmp");

	backgroundImage = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
	manual = SDL_CreateTextureFromSurface(renderer, manualSurface);
	SDL_FreeSurface(backgroundSurface); // Ya no necesitamos la superficie
	SDL_FreeSurface(manualSurface);
}

MenuItem victoryMenu::createMenuItem(datos dato)
{
	MenuItem  menuItem;
	menuItem.text = dato.text;

	SDL_Surface* textSurface = TTF_RenderText_Blended(font, dato.text.c_str(), { 100, 255, 25 });

	menuItem.rect = { dato.x, dato.y, textSurface->w, textSurface->h };
	menuItem.texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	if (!menuItem.texture) {
		SDL_FreeSurface(textSurface);
		return menuItem;
	}

	SDL_FreeSurface(textSurface);
	return menuItem;
}

void victoryMenu::drawMenuItem(SDL_Renderer* renderer, const MenuItem& menuItem)
{
	SDL_RenderCopy(renderer, menuItem.texture, nullptr, &menuItem.rect);
}

void victoryMenu::drawTriangleCursor(SDL_Renderer* renderer, int x, int y, int size)
{
	int halfSize = size / 2;
	SDL_RenderDrawLine(renderer, x - halfSize, y - halfSize, x + halfSize, y);
	SDL_RenderDrawLine(renderer, x + halfSize, y, x - halfSize, y + halfSize);
	SDL_RenderDrawLine(renderer, x - halfSize, y + halfSize, x - halfSize, y - halfSize);
}

void victoryMenu::render()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Color negro
	SDL_RenderClear(renderer);
	// Renderizar los items del menú

	for (int i = 0; i < 2; i++) {
		if (elecciones[i]) {
			drawMenuItem(renderer, menuItems[i]);

		}
	}


	SDL_Rect imageRect = { w / 2 - 389, 100, 0, 0 };
	SDL_Rect manualRect = { 0 , h - 100, w + 200, 0 };
	SDL_QueryTexture(backgroundImage, NULL, NULL, &imageRect.w, &imageRect.h);
	SDL_RenderCopy(renderer, backgroundImage, NULL, &imageRect); // Dibujar en toda la pantalla

	SDL_QueryTexture(manual, NULL, NULL, &manualRect.w, &manualRect.h);
	SDL_RenderCopy(renderer, manual, NULL, &manualRect); // Dibujar en toda la pantalla

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	drawTriangleCursor(renderer, menuItems[cursorIndex - 1].rect.x - 30, menuItems[cursorIndex - 1].rect.y + menuItems[cursorIndex - 1].rect.h / 2 + 5, 20);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	drawTriangleCursor(renderer, menuItems[cursorIndex].rect.x - 30, menuItems[cursorIndex].rect.y + menuItems[cursorIndex].rect.h / 2 + 5, 20);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	drawTriangleCursor(renderer, menuItems[cursorIndex + 1].rect.x - 30, menuItems[cursorIndex + 1].rect.y + menuItems[cursorIndex + 1].rect.h / 2 + 5, 20);


	SDL_RenderPresent(renderer);
}

int victoryMenu::eventHandler(SDL_Event evento)
{
	configuraciones* conf = configuraciones::getInstancia();
	switch (evento.type) {
	case SDL_QUIT:
		fin = true;
		return 0;
		break;
	case SDL_KEYDOWN:
		switch (evento.key.keysym.sym) {
		
		case SDLK_ESCAPE:
			return 0;
			break;

		case SDLK_w:
		case SDLK_UP:
			if (cursorIndex > 0) {
				cursorIndex--;
			}
			break;

		case SDLK_s:
		case SDLK_DOWN:
			if (cursorIndex < 1) {
				cursorIndex++;
			}
			break;
		case SDLK_RETURN:
			switch (cursorIndex) {
			case 0:
				this->cont = true;
				return 0;
				break;
			case 1:
				this->fin = true;
				return 0;
				break;

			}
		}
		return 1;
	}
}

bool victoryMenu::isFinal()
{
	bool aux = this->fin;
	this->fin = false;
	return aux;
}

bool victoryMenu::weFight()
{
	bool aux = this->cont;
	this->cont = false;
	return aux;
}
