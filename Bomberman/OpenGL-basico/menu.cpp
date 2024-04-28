#include "menu.h"
#include "SDL_opengl.h"
#include <tuple>
menu::menu(int w,int h, SDL_Renderer* r){
	this->w = w;
	this->h = h;
	cursorIndex = 0;
	font = loadFont("letra3.ttf", 48);

	renderer = r;

	int p_inicial_x = w / 2 - 350;
	int p_inicial_y = 550;
	int distancia_y = 50;
	
	datos info[11] = {
		{ font, "START", p_inicial_x , p_inicial_y, renderer }, 
		{ font, "WIREFRAME ON", p_inicial_x, p_inicial_y + distancia_y, renderer }, 
		{font, "TEXTURES ON", p_inicial_x, p_inicial_y + 2 * distancia_y, renderer},
		{ font, "PLAY VELOCITY 1x", w / 2, p_inicial_y, renderer },
		{ font, "FACETADO", w / 2, p_inicial_y + distancia_y, renderer },
		{ font, "QUIT", w / 2, p_inicial_y + 2 * distancia_y, renderer },
		{ font, "WIREFRAME OFF", p_inicial_x, p_inicial_y + distancia_y, renderer },
		{font, "TEXTURES OFF", p_inicial_x, p_inicial_y + 2 * distancia_y, renderer},
		{ font, "PLAY VELOCITY 2x", w / 2, p_inicial_y, renderer },
		{ font, "PLAY VELOCITY 3x", w / 2, p_inicial_y, renderer },
		{ font, "INTERPOLADO", w / 2, p_inicial_y +  distancia_y, renderer },


	};

	menuItems = new MenuItem[11];

	for (int i = 0; i < 11;i++) {
		menuItems[i] = createMenuItem(info[i]);
	}

	elecciones = new int[11];
	elecciones[0] = 1;
	elecciones[1] = 0;
	elecciones[2] = 1;
	elecciones[3] = 1;
	elecciones[4] = 1;
	elecciones[5] = 1;
	elecciones[6] = 1;
	elecciones[7] = 0;
	elecciones[8] = 0;
	elecciones[9] = 0;
	elecciones[10] = 0;

	SDL_Surface* backgroundSurface = SDL_LoadBMP("bomberman.bmp");
	SDL_Surface*manualSurface = SDL_LoadBMP("instrucciones.bmp");

	backgroundImage = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
	manual = SDL_CreateTextureFromSurface(renderer, manualSurface);
	SDL_FreeSurface(backgroundSurface); // Ya no necesitamos la superficie
	SDL_FreeSurface(manualSurface);

}

MenuItem menu::createMenuItem(datos dato) {
	
	MenuItem  menuItem;
	menuItem.text = dato.text;

	SDL_Surface* textSurface = TTF_RenderText_Blended(font, dato.text.c_str(), { 255, 255, 255 });

	menuItem.rect = { dato.x, dato.y, textSurface->w, textSurface->h };
	menuItem.texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	if (!menuItem.texture) {
		SDL_FreeSurface(textSurface);
		return menuItem;
	}

	SDL_FreeSurface(textSurface);
	return menuItem;
}


void menu::drawMenuItem(SDL_Renderer* renderer, const MenuItem& menuItem) {
	SDL_RenderCopy(renderer, menuItem.texture, nullptr, &menuItem.rect);
}


void menu::drawTriangleCursor(SDL_Renderer* renderer, int x, int y, int size) {
	int halfSize = size / 2;
	SDL_RenderDrawLine(renderer, x - halfSize, y - halfSize, x + halfSize, y);
	SDL_RenderDrawLine(renderer, x + halfSize, y, x - halfSize, y + halfSize);
	SDL_RenderDrawLine(renderer, x - halfSize, y + halfSize, x - halfSize, y - halfSize);
}

void menu::render() {

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Color negro
	SDL_RenderClear(renderer);
	// Renderizar los items del menú

	for (int i = 0; i < 11; i++) {
		if (elecciones[i]) {
			drawMenuItem(renderer, menuItems[i]);

		}
	}
	

	SDL_Rect imageRect = { w / 2 - 389, 100, 0, 0 };
	SDL_Rect manualRect = { 0 , h-100, w + 200, 0 };
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

int menu::eventHandler(SDL_Event evento) {
	configuraciones* conf = configuraciones::getInstancia();
	switch (evento.type) {
	case SDL_KEYDOWN:
		switch (evento.key.keysym.sym) {
		case SDL_QUIT:
			//ToDo implementar
			break;
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
			if (cursorIndex < 5) {
				cursorIndex++;
			}
			break;
		case SDLK_RETURN:
			switch (cursorIndex) {
			case 0:
				return 0;
				break;
			case 1:
				if (elecciones[1] == 1) {
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					elecciones[6] = 1;
					elecciones[1] = 0;
				}
				else {
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					elecciones[1] = 1;
					elecciones[6] = 0;
				}
				break;
			case 2:
				if (elecciones[2] == 1) {
					mostrarTexturas = false;
					elecciones[7] = 1;
					elecciones[2] = 0;
				}
				else {
					mostrarTexturas = true;
					elecciones[2] = 1;
					elecciones[7] = 0;
				}
				break;
			case 3:
				if (elecciones[3] == 1) {
					conf->setVelocidadJuego(1.5f);
					elecciones[3] = 0;
					elecciones[8] = 1;
					elecciones[9] = 0;
				}
				else if(elecciones[8]==1){
					conf->setVelocidadJuego(2.f);
					elecciones[3] = 0;
					elecciones[8] = 0;
					elecciones[9] = 1;
				}
				else {
					conf->setVelocidadJuego(1.f);
					elecciones[3] = 1;
					elecciones[8] = 0;
					elecciones[9] = 0;
				}
				break;
			case 4:
				if (elecciones[4] == 1) {
					glShadeModel(GL_SMOOTH);
					elecciones[4] = 0;
					elecciones[10] = 1;
				}
				else {
					glShadeModel(GL_FLAT);
					elecciones[4] = 1;
					elecciones[10] = 0;
				}
				break;
			}

		}
	}
	return 1;
}