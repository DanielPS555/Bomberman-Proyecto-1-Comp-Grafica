#include "hud.h"
#include "cmath"
#include "string"
#include "chrono"
#include <thread>


using namespace std;

using Clock = std::chrono::steady_clock;
using this_thread::sleep_for; //ToDo Eliminar
using chrono::time_point;
using chrono::duration_cast;
using chrono::milliseconds;
using chrono::seconds;

float ajustarDim(float tamObjetivo) {
	return tamObjetivo * 0.277; //ToDo sacar hardcode
}

Hud::Hud(SDL_Renderer* r, SDL_Window* w) {


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
	win = w;

	fuente = loadFont("letra3.ttf", 50);

	millSec = 0;
	puntos = 0;
	level = 1;
	numeroEnemigosRestantes = 0;
	numeroVidasRestantes = 0;

	textureIconoHeart = inicializarTexturaPng("assets/heartIcon.png");
	textureIconoEnemy = inicializarTexturaPng("assets/enemyIcon.png");


	float w_2 = width / 2.0f;
	float h_2 = heigth / 2.0f;


	
	float wIcono = 12.0f;

	float xIconoHeart = w_2 * 0.75f, yIconoHeart = h_2 *0.855f;

	GLfloat posicionesIconoHeart[12] = {xIconoHeart				, yIconoHeart				, -PROFUNDIDAD_HUD,
										xIconoHeart + wIcono	, yIconoHeart				, -PROFUNDIDAD_HUD,
										xIconoHeart + wIcono	, yIconoHeart + wIcono		, -PROFUNDIDAD_HUD,
										xIconoHeart				, yIconoHeart + wIcono		, -PROFUNDIDAD_HUD,
	};
	
	GLfloat colores[3] = { 1.f,1.f,1.f };
	GLfloat normal[3]  = { 0.f,0.f,1.f };
	
	iconoHeart = new Rectangulo2d<1>(posicionesIconoHeart, normal, colores);
	
	
	float xIconoEnemy = w_2 * 0.92f, yIconoEnemy = h_2 * 0.855f;
	GLfloat posicionesIconoEnemy[12] = { xIconoEnemy			, yIconoEnemy			, -PROFUNDIDAD_HUD,
										 xIconoEnemy + wIcono	, yIconoEnemy			, -PROFUNDIDAD_HUD,
										 xIconoEnemy + wIcono	, yIconoEnemy + wIcono  , -PROFUNDIDAD_HUD,
										 xIconoEnemy			, yIconoEnemy + wIcono	, -PROFUNDIDAD_HUD,
	};
	iconoEnemy = new Rectangulo2d<1>(posicionesIconoEnemy, normal, colores);
}

Hud::~Hud() {
	TTF_CloseFont(fuente);
	free(renderer);
	free(iconoEnemy);
	free(iconoHeart);
}


void Hud::aumentoTiempo(long deltaAumento) {
	millSec += deltaAumento;
}


void Hud::render() {

	renderCajaSupIsq();
	renderCajaSupCentro();
	renderCajaSupDer();

	renderTextTiempo();

	renderTextPuntosYLevel();

	renderTextVidas();

	if (modoMuerteActivado) {
		renderPantallaMuerte();
	}
}

void Hud::renderTextPuntosYLevel() {

	float w_2 = width / 2.0f;
	float h_2 = heigth / 2.0f;

	std::string puntosSrt = "XP " + std::to_string(puntos);
	renderTextConSombra(0.4f, puntosSrt, -w_2*0.985f, h_2 * 1.01 , false, { 255, 255, 255 });

	std::string levelSrt = "Level " + std::to_string(level);
	renderTextConSombra(0.25f, levelSrt, -w_2 * 0.985f, h_2 * 0.91f, false, { 241, 255, 69 });
}

void Hud::renderTextVidas() {
	float w_2 = width / 2.0f;
	float h_2 = heigth / 2.0f;

	std::string vidasStr= std::to_string(numeroVidasRestantes);
	renderTextConSombra(0.6f, vidasStr, w_2 * 0.68f, h_2 * 1.01, false, { 255, 255, 255 });

	std::string enemigosStr =  std::to_string(numeroEnemigosRestantes);
	renderTextConSombra(0.6f, enemigosStr, w_2 * 0.85f, h_2 * 1.01, false, { 255, 255, 255 });

	

	iniciliarRenderVertexArray();

	glPushMatrix();
	glTranslatef(-0.3f, 0.3f, 0.0f);

	float negro[12] = { 0.0f, 0.0f, 0.0f , 0.0f, 0.0f, 0.0f , 0.0f, 0.0f, 0.0f , 0.0f, 0.0f, 0.0f };
	
	iconoHeart->renderConPuntoIntermediosYTexturaYColor(textureIconoHeart, negro);
	iconoEnemy->renderConPuntoIntermediosYTexturaYColor(textureIconoEnemy, negro);

	glPopMatrix();

	iconoHeart->renderConPuntoIntermediosYTextura(textureIconoHeart);
	iconoEnemy->renderConPuntoIntermediosYTextura(textureIconoEnemy);
	
	finalizarRenderVertexArray();

}

void Hud::renderTextConSombra(float resizeFont, std::string texto, float bordeSupIsqX,
	float bordeSupIsqY, bool centrarAntesDeTrasladar, SDL_Color color) {

	renderText(resizeFont, texto, bordeSupIsqX - 0.5f, bordeSupIsqY + 0.5f, centrarAntesDeTrasladar, { 0, 0, 0 });
	renderText(resizeFont, texto, bordeSupIsqX , bordeSupIsqY, centrarAntesDeTrasladar, color);

}

void Hud::renderPantallaMuerte() {

	float w_2 = width / 2;
	float h_2 = heigth / 2;

	SDL_Event evento;

	time_point<Clock> beginLastFrame = Clock::now();
	milliseconds tiempoTranscurridoUltimoFrame;

	float opacidad = (millSec <= tiempoInicioAnimacionMuerte + TIEMPO_ANIMACION_MUERTE)
		? OPACIDAD_PANTALLA_MUERTE * ((millSec - tiempoInicioAnimacionMuerte) / TIEMPO_ANIMACION_MUERTE)
		: OPACIDAD_PANTALLA_MUERTE;

	cout << millSec << " " << tiempoInicioAnimacionMuerte + TIEMPO_ANIMACION_MUERTE << " " << opacidad << "\n";

	renderTextConSombra(0.8f, "HAS MUERTO", 0.0f, 0.0f, true, {25, 25, 255});


	glColor4f(1, 0, 0, opacidad);
	glBegin(GL_QUADS);
	glVertex3f(-w_2, -h_2, -PROFUNDIDAD_HUD);
	glVertex3f(w_2, -h_2, -PROFUNDIDAD_HUD);
	glVertex3f(w_2, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2, h_2, -PROFUNDIDAD_HUD);
	glEnd();

	if (millSec > tiempoInicioAnimacionMuerte + TIEMPO_ANIMACION_MUERTE) {
		modoMuerteActivado = false;
	}

}

void Hud::renderTextTiempo() {
	//CONFICTO ENTRE OPENGL Y EL RENDERER
		
	
	float w_2 = width / 2.0f;
	float h_2 = heigth / 2.0f;


	int numeroHoras		= (millSec / 3600000 ) % 100; //Numero de horas
	int numeroMinutos	= (millSec /   60000) % 60 ; //Numero de minutos
	int numeroSegundos	= (millSec /    1000) % 60 ; //Numero segundos

	std::string numeroHorasStr = std::to_string(numeroHoras);
	if (numeroHoras < 10) {
		numeroHorasStr = "0" + numeroHorasStr;
	}

	std::string numeroMinutosStr = std::to_string(numeroMinutos);
	if (numeroMinutos < 10) {
		numeroMinutosStr = "0" + numeroMinutosStr;
	}

	std::string numeroSegundosStr = std::to_string(numeroSegundos);
	if (numeroSegundos < 10) {
		numeroSegundosStr = "0" + numeroSegundosStr;
	}

	std::string tiempo = numeroHorasStr + " " + numeroMinutosStr + " " + numeroSegundosStr;

	renderTextConSombra(0.7f, tiempo, 0.0f, h_2 * 0.96f, true, { 255, 255, 255 });

}

void Hud::setPuntosYLevel(unsigned long p, unsigned int l) {
	puntos = p;
	level = l;
}

void Hud::setNumeroVidasRestantes(unsigned int numVidas) {
	numeroVidasRestantes = numVidas;
}

void Hud::setNumeroEnemigosRestantes(unsigned int numEnemigos) {
	numeroEnemigosRestantes = numEnemigos;
}


void Hud::activarPantallaMuerte() {
	modoMuerteActivado = true;
	tiempoInicioAnimacionMuerte = millSec;


}

bool Hud::isPantallaMuerteActivada() {
	return modoMuerteActivado;
}




void Hud::renderText(float resizeFont, std::string texto, float bordeSupIsqX, float bordeSupIsqY, bool centrarAntesDeTrasladar, SDL_Color color) {

	// El siguiente codigo es la escritura de los numeros, todo el credito a este post: https://stackoverflow.com/questions/25771735/creating-opengl-texture-from-sdl2-surface-strange-pixel-values

	SDL_Surface* textSurface = TTF_RenderText_Blended(fuente, texto.c_str(), color);

	int w_text = textSurface->w;
	int h_text = textSurface->h;


	GLuint ret;

	GLuint texture_format;
	Uint8 colors = textSurface->format->BytesPerPixel;
	if (colors == 4) {   // alpha
		if (textSurface->format->Rmask == 0x000000ff)
			texture_format = GL_RGBA;
		else
			texture_format = GL_BGRA;
	}
	else {             // no alpha
		if (textSurface->format->Rmask == 0x000000ff)
			texture_format = GL_RGB;
		else
			texture_format = GL_BGR;
	}


	int alignment = 8;
	while (textSurface->pitch % alignment) alignment >>= 1;
	glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

	int expected_pitch = (textSurface->w * textSurface->format->BytesPerPixel + alignment - 1) / alignment * alignment;
	if (textSurface->pitch - expected_pitch >= alignment)
		glPixelStorei(GL_UNPACK_ROW_LENGTH, textSurface->pitch / textSurface->format->BytesPerPixel);
	else glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

	GLuint idTexturaTiempo;
	glGenTextures(1, &idTexturaTiempo);
	glBindTexture(GL_TEXTURE_2D, idTexturaTiempo);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w_text, h_text, 0, GL_RGBA, GL_UNSIGNED_BYTE, textSurface->pixels);


	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);


	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

	glPushMatrix();

	glTranslated(bordeSupIsqX, bordeSupIsqY, 0.0f);
	glTranslated(0.0f, 0.0f, -PROFUNDIDAD_HUD);

	if (!centrarAntesDeTrasladar) {
		glTranslated((w_text / 2) * resizeFont, (-h_text / 2) * resizeFont, 0.0f);
	}
	
	glScaled(resizeFont, resizeFont, 0.0f);
	glRotatef(180, 1, 0, 0);
	


	iniciliarRenderVertexArray();

	glColor3f(1.0f, 1.0f, 1.0f);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-w_text / 2, -h_text / 2, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(w_text / 2, -h_text / 2, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(w_text / 2, h_text / 2, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-w_text / 2, h_text / 2, 0.0f);
	glEnd();


	finalizarRenderVertexArray();

	glPopMatrix();

	glDeleteTextures(1, &idTexturaTiempo);

	SDL_FreeSurface(textSurface);
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