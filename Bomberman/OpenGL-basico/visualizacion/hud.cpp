#include "hud.h"
#include "cmath"

float ajustarDim(float tamObjetivo) {
	return tamObjetivo * 0.277; //ToDo sacar hardcode
}

Hud::Hud(SDL_Renderer* r) {


	// Utilizo calculos via pitagoras y thales

	/*float mitad_x_pantalla = SCREEN_WIDTH / 2;
	float mitad_y_pantalla = SCREEN_HEIGHT / 2;
	float diagonal_original = sqrt( pow(mitad_x_pantalla, 2) + pow(mitad_y_pantalla, 2));

	float razon_resize = PROFUNDIDAD_HUD / diagonal_original;

	width = SCREEN_WIDTH * razon_resize;
	heigth = SCREEN_HEIGHT * razon_resize;

	*/

	width = ajustarDim(SCREEN_WIDTH);   // ToDo sacar hardcode
	heigth = ajustarDim(SCREEN_HEIGHT); // ToDo sacar hardcode

	renderer = r;

	fuente_tiempo = loadFont("letra3.ttf", 48);


}

Hud::~Hud() {
	TTF_CloseFont(fuente_tiempo);
	free(renderer);
}





void Hud::render() {

	renderCajaSupIsq();
	renderCajaSupCentro();
	renderCajaSupDer();

	renderTextTiempo();

}



void Hud::renderTextTiempo() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // Color negro
	SDL_RenderClear(renderer);

	float w_2 = width	/ 2.0f;
	float h_2 = heigth	/ 2.0f;

	std::string tiempo = "00:00:00";

	SDL_Surface* textSurface = TTF_RenderText_Blended(fuente_tiempo, tiempo.c_str(), {255, 255, 255});
	SDL_Rect rect = { 0.0f, 0.0f, textSurface->w, textSurface->h };
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);

	SDL_RenderCopy(renderer, texture, nullptr, &rect);

	SDL_RenderPresent(renderer);

	/*
	float w_2 = width / 2.0f;
	float h_2 = heigth / 2.0f;

	std::string tiempo = "00:00:00";

	SDL_Surface* textSurface = TTF_RenderText_Blended(fuente_tiempo, tiempo.c_str(), { 255, 255, 255 });

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);

	bool textureIsNull = texture == NULL;
	std::cout << textureIsNull << "\n";
	
	float w;
	float h;

	iniciliarRenderVertexArray();

	

	std::cout << SDL_GL_BindTexture(texture, &w, &h);
	std::cout << " " << w << " " << h << " " << "\n";
	//glBindTexture(GL_TEXTURE_2D, id);

	/*glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, -PROFUNDIDAD_HUD);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, -PROFUNDIDAD_HUD);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(100.0f, 100.0f, -PROFUNDIDAD_HUD);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 100.0f, -PROFUNDIDAD_HUD);
	glEnd();

	finalizarRenderVertexArray(); */



}


void Hud::renderCajaSupCentro() {
	float w_2 = width / 2;
	float h_2 = heigth / 2;

	// ---- Borde Exterior

	glPushMatrix();

	glTranslatef(0.0f, h_2, 0.0f);
	glScalef(1.05f, 1.20f, 1.0f);
	glTranslatef(0.0f, -h_2, 0.0f);

	glColor4f(0.0f, 0.0f, 0.0f, 0.10f);

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(-w_2 * 0.40f, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2 * 0.35f, h_2 * 0.86f, -PROFUNDIDAD_HUD);
	glVertex3f(w_2 * 0.35f, h_2 * 0.86f, -PROFUNDIDAD_HUD);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(w_2 * 0.35f, h_2 * 0.86f, -PROFUNDIDAD_HUD);
	glVertex3f(w_2 * 0.40f, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2 * 0.40f, h_2, -PROFUNDIDAD_HUD);
	glEnd();

	glPopMatrix();



	// ---- Borde medio

	glPushMatrix();

	glTranslatef(0.0f, h_2, 0.0f);
	glScalef(1.03f, 1.1f, 1.0f);
	glTranslatef(0.0f, -h_2, 0.0f);

	glBegin(GL_TRIANGLE_STRIP);
	glColor4f(255.f / 256.f, 247.f / 256.f, 215.f / 256.f, 1.00f);
	glVertex3f(-w_2 * 0.40f, h_2, -PROFUNDIDAD_HUD);
	glColor4f(255.f / 256.f, 247.f / 256.f, 215.f / 256.f, 1.00f);
	glVertex3f(-w_2 * 0.35f, h_2 * 0.86f, -PROFUNDIDAD_HUD);
	glColor4f(254.f / 256.f, 217.f / 256.f, 71.f / 256.f, 1.00f);
	glVertex3f(w_2 * 0.35f, h_2 * 0.86f, -PROFUNDIDAD_HUD);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glColor4f(254.f / 256.f, 217.f / 256.f, 71.f / 256.f, 1.00f);
	glVertex3f(w_2 * 0.35f, h_2 * 0.86f, -PROFUNDIDAD_HUD);
	glColor4f(254.f / 256.f, 217.f / 256.f, 71.f / 256.f, 1.00f);
	glVertex3f(w_2 * 0.40f, h_2, -PROFUNDIDAD_HUD);
	glColor4f(254.f / 256.f, 217.f / 256.f, 71.f / 256.f, 1.00f);
	glVertex3f(-w_2 * 0.40f, h_2, -PROFUNDIDAD_HUD);
	glEnd();

	glPopMatrix();

	// ---- Borde Interior

	glPushMatrix();

	glTranslatef(0.0f, h_2, 0.0f);
	glScalef(1.015f, 1.05f, 1.0f);
	glTranslatef(0.0f, -h_2, 0.0f);


	glColor4f(255.f / 256.f, 247.f / 256.f, 215.f / 256.f, 0.8f);
	glBegin(GL_TRIANGLE_STRIP);

	glVertex3f(-w_2 * 0.40f, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2 * 0.35f, h_2 * 0.86f, -PROFUNDIDAD_HUD);
	glVertex3f(w_2 * 0.35f, h_2 * 0.86f, -PROFUNDIDAD_HUD);

	glEnd();

	glBegin(GL_TRIANGLE_STRIP);

	glVertex3f(w_2 * 0.35f, h_2 * 0.86f, -PROFUNDIDAD_HUD);
	glVertex3f(w_2 * 0.40f, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2 * 0.40f, h_2, -PROFUNDIDAD_HUD);

	glEnd();

	glPopMatrix();

	//Relleno

	glColor4f(254.f / 256.f, 95.f / 256.f, 40.f / 256.f, 1.0f);

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(-w_2 * 0.40f, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2 * 0.35f, h_2 * 0.86f, -PROFUNDIDAD_HUD);
	glVertex3f(w_2 * 0.35f, h_2 * 0.86f, -PROFUNDIDAD_HUD);
	glVertex3f(w_2 * 0.40f, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2 * 0.40f, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(0.f, h_2 * 0.86f, -PROFUNDIDAD_HUD);
	glEnd();
}

void Hud::renderCajaSupIsq() {
	float w_2 = width / 2;
	float h_2 = heigth / 2;


	//Borde externo
	glPushMatrix();

	glTranslatef(-w_2, h_2, 0.0f);
	glScalef(1.05f, 1.2f, 1.0f);
	glTranslatef(w_2, -h_2, 0.0f);

	glColor4f(0.0f, 0.0f, 0.0f, 0.10f);

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(-w_2, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2, h_2 * 0.80f, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2 * 0.70f, h_2 * 0.80f, -PROFUNDIDAD_HUD);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(-w_2 * 0.60f, h_2 * 0.90f, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2 * 0.60f, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2, h_2, -PROFUNDIDAD_HUD);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(-w_2 * 0.70f, h_2 * 0.80f, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2 * 0.60f, h_2 * 0.90f, -PROFUNDIDAD_HUD);
	glEnd();

	glPopMatrix();

	//Borde Medio

	glPushMatrix();

	glTranslatef(-w_2, h_2, 0.0f);
	glScalef(1.03f, 1.1f, 1.0f);
	glTranslatef(w_2, -h_2, 0.0f);


	glBegin(GL_TRIANGLE_STRIP);
	glColor4f(255.f / 256.f, 247.f / 256.f, 215.f / 256.f, 1.00f);
	glVertex3f(-w_2, h_2, -PROFUNDIDAD_HUD);
	glColor4f(255.f / 256.f, 247.f / 256.f, 215.f / 256.f, 1.00f);
	glVertex3f(-w_2, h_2 * 0.80f, -PROFUNDIDAD_HUD);
	glColor4f(254.f / 256.f, 217.f / 256.f, 71.f / 256.f, 1.00f);
	glVertex3f(-w_2 * 0.70f, h_2 * 0.80f, -PROFUNDIDAD_HUD);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(-w_2 * 0.60f, h_2 * 0.90f, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2 * 0.60f, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2, h_2, -PROFUNDIDAD_HUD);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(-w_2 * 0.70f, h_2 * 0.80f, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2 * 0.60f, h_2 * 0.90f, -PROFUNDIDAD_HUD);
	glEnd();

	glPopMatrix();

	//Borde interno 

	glPushMatrix();

	glTranslatef(-w_2, h_2, 0.0f);
	glScalef(1.02f, 1.05f, 1.0f);
	glTranslatef(w_2, -h_2, 0.0f);

	glColor4f(255.f / 256.f, 247.f / 256.f, 215.f / 256.f, 0.8f);

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(-w_2, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2, h_2 * 0.80f, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2 * 0.70f, h_2 * 0.80f, -PROFUNDIDAD_HUD);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(-w_2 * 0.60f, h_2 * 0.90f, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2 * 0.60f, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2, h_2, -PROFUNDIDAD_HUD);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(-w_2 * 0.70f, h_2 * 0.80f, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2 * 0.60f, h_2 * 0.90f, -PROFUNDIDAD_HUD);
	glEnd();

	glPopMatrix();

	glBegin(GL_TRIANGLE_STRIP);

	glColor4f(254.f / 256.f, 95.f / 256.f, 40.f / 256.f, 1.0f);
	glVertex3f(-w_2, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2, h_2 * 0.80f, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2 * 0.70f, h_2 * 0.80f, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2 * 0.60f, h_2 * 0.90f, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2 * 0.60f, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2 * 0.70f, h_2 * 0.80f, -PROFUNDIDAD_HUD);

	glEnd();
}

void Hud::renderCajaSupDer() {
	float w_2 = width / 2;
	float h_2 = heigth / 2;

	//Borde exterior

	glPushMatrix();

	glTranslatef(w_2, h_2, 0.0f);
	glScalef(1.05f, 1.2f, 1.0f);
	glTranslatef(-w_2, -h_2, 0.0f);

	glColor4f(0.0f, 0.0f, 0.0f, 0.10f);

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(w_2, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(w_2, h_2 * 0.80f, -PROFUNDIDAD_HUD);
	glVertex3f(w_2 * 0.70f, h_2 * 0.80f, -PROFUNDIDAD_HUD);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(w_2 * 0.60f, h_2 * 0.90f, -PROFUNDIDAD_HUD);
	glVertex3f(w_2 * 0.60f, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(w_2, h_2, -PROFUNDIDAD_HUD);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(w_2 * 0.70f, h_2 * 0.80f, -PROFUNDIDAD_HUD);
	glVertex3f(w_2, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(w_2 * 0.60f, h_2 * 0.90f, -PROFUNDIDAD_HUD);
	glEnd();

	glPopMatrix();


	//Borde medio

	glPushMatrix();

	glTranslatef(w_2, h_2, 0.0f);
	glScalef(1.03f, 1.1f, 1.0f);
	glTranslatef(-w_2, -h_2, 0.0f);



	glBegin(GL_TRIANGLE_STRIP);
	glColor4f(255.f / 256.f, 247.f / 256.f, 215.f / 256.f, 1.00f);
	glVertex3f(w_2, h_2, -PROFUNDIDAD_HUD);

	glColor4f(254.f / 256.f, 217.f / 256.f, 71.f / 256.f, 1.00f);
	glVertex3f(w_2, h_2 * 0.80f, -PROFUNDIDAD_HUD);

	glColor4f(255.f / 256.f, 247.f / 256.f, 215.f / 256.f, 1.00f);
	glVertex3f(w_2 * 0.70f, h_2 * 0.80f, -PROFUNDIDAD_HUD);
	glEnd();


	glColor4f(255.f / 256.f, 247.f / 256.f, 215.f / 256.f, 1.00f);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(w_2 * 0.60f, h_2 * 0.90f, -PROFUNDIDAD_HUD);
	glVertex3f(w_2 * 0.60f, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(w_2, h_2, -PROFUNDIDAD_HUD);
	glEnd();


	glColor4f(255.f / 256.f, 247.f / 256.f, 215.f / 256.f, 1.00f);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(w_2 * 0.70f, h_2 * 0.80f, -PROFUNDIDAD_HUD);
	glVertex3f(w_2, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(w_2 * 0.60f, h_2 * 0.90f, -PROFUNDIDAD_HUD);
	glEnd();

	glPopMatrix();



	//Borde interior

	glPushMatrix();

	glTranslatef(w_2, h_2, 0.0f);
	glScalef(1.015f, 1.05f, 1.0f);
	glTranslatef(-w_2, -h_2, 0.0f);

	glColor4f(255.f / 256.f, 247.f / 256.f, 215.f / 256.f, 0.8f);

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(w_2, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(w_2, h_2 * 0.80f, -PROFUNDIDAD_HUD);
	glVertex3f(w_2 * 0.70f, h_2 * 0.80f, -PROFUNDIDAD_HUD);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(w_2 * 0.60f, h_2 * 0.90f, -PROFUNDIDAD_HUD);
	glVertex3f(w_2 * 0.60f, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(w_2, h_2, -PROFUNDIDAD_HUD);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(w_2 * 0.70f, h_2 * 0.80f, -PROFUNDIDAD_HUD);
	glVertex3f(w_2, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(w_2 * 0.60f, h_2 * 0.90f, -PROFUNDIDAD_HUD);
	glEnd();

	glPopMatrix();


	glBegin(GL_TRIANGLE_STRIP);

	glColor4f(254.f / 256.f, 95.f / 256.f, 40.f / 256.f, 1.0f);
	glVertex3f(w_2, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(w_2, h_2 * 0.80f, -PROFUNDIDAD_HUD);
	glVertex3f(w_2 * 0.70f, h_2 * 0.80f, -PROFUNDIDAD_HUD);
	glVertex3f(w_2 * 0.60f, h_2 * 0.90f, -PROFUNDIDAD_HUD);
	glVertex3f(w_2 * 0.60f, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(w_2, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(w_2 * 0.70f, h_2 * 0.80f, -PROFUNDIDAD_HUD);

	glEnd();
}