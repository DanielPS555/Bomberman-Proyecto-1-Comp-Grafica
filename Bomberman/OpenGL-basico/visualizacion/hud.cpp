#include "hud.h"
#include "cmath"

float ajustarDim(float tamObjetivo) {
	return tamObjetivo * 0.275; //ToDo sacar hardcode
}

Hud::Hud() {


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

	float w_2 = width / 2;
	float h_2 = heigth / 2;

	std::cout << h_2 << std::endl;

	reloj  = {
		{
			-w_2 * 0.15f, h_2 * 0.78f, -PROFUNDIDAD_HUD,
			 w_2 * 0.15f, h_2 * 0.78f, -PROFUNDIDAD_HUD,
			 w_2 * 0.15f, h_2 * 0.95f, -PROFUNDIDAD_HUD,
			-w_2 * 0.15f, h_2 * 0.95f, -PROFUNDIDAD_HUD,
		} 
		,{0,0,1}																						  //Norma
		,{ 1.,1.,1.,		 1.,1.,1.,              1.,1.,1.,					    1.,1.,1.         }	  // colores	
		,{0,1,2,3}																						  // indices
		,{0.0,0.0,	0.0,1.0,	1.0,1.0,	1.0,0.0}
	};

	texturaReloj = inicializarTexturaPng("assets/reloj.png");

}





void Hud::render() {
	iniciliarRenderVertexArray();

	renderRectangulo2d(reloj, texturaReloj);
	//renderRectangulo2d(superfice);

	finalizarRenderVertexArray();
}