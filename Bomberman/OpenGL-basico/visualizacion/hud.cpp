#include "hud.h"
#include "cmath"

float ajustarDim(float tamObjetivo) {
	return tamObjetivo * 0.277; //ToDo sacar hardcode
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

	/*header = {
		{
			-w_2, h_2 * 0.80f, -PROFUNDIDAD_HUD,
			 w_2, h_2 * 0.80f, -PROFUNDIDAD_HUD,
			 w_2, h_2		 , -PROFUNDIDAD_HUD,
			-w_2, h_2		 , -PROFUNDIDAD_HUD,
		} 
		,{0,0,1}																						  //Norma
		,{  0.15f,0.15f,0.15f,0.65f,
			0.45f,0.45f,0.45f,0.30f,     
			0.45f,0.45f,0.45f,0.30f,		
			0.15f,0.15f,0.15f,0.65f
		}	  // colores	
		,{0,1,2,3}																						  // indices
		,{0.0,0.0,	0.0,1.0,	1.0,1.0,	1.0,0.0}
	};*/


	header = {
	{
		-w_2, h_2 * 0.80f, -PROFUNDIDAD_HUD,
		 w_2, h_2 * 0.80f, -PROFUNDIDAD_HUD,
		 w_2, h_2		 , -PROFUNDIDAD_HUD,
		-w_2, h_2		 , -PROFUNDIDAD_HUD,
	}
	,{0,0,1}																						  //Norma
	,{ 1.0f,1.0f,1.0f,1.f,
		1.0f,1.0f,1.0f,1.f,
		1.0f,1.0f,1.0f,1.f,
		1.0f,1.0f,1.0f,1.f
	}	  // colores	
	,{0,1,2,3}																						  // indices
	,{0.0,0.0,	0.0,1.0,	1.0,1.0,	1.0,0.0}
	};


	toolBox = {
		{
			 w_2 * 0.70f,	 -h_2			 , -PROFUNDIDAD_HUD,
			 w_2		,	-h_2			 , -PROFUNDIDAD_HUD,
			 w_2		,	 -h_2 * 0.60f	 , -PROFUNDIDAD_HUD,
			 w_2 * 0.70f,	-h_2 * 0.60f	 , -PROFUNDIDAD_HUD,
		}
		,{0,0,1}																						  //Norma
		,{  0.15f,0.15f,0.15f,0.65f,
			0.45f,0.45f,0.45f,0.30f,
			0.45f,0.45f,0.45f,0.30f,
			0.15f,0.15f,0.15f,0.65f
		}	  // colores	
		,{0,1,2,3}																						  // indices
		,{0.0,0.0,	1.0,0.0,	1.0,1.0,	0.0,1.0}
	};

}





void Hud::render() {

	float w_2 = width / 2;
	float h_2 = heigth / 2;

	// Marco de vidas y enemigos

	glColor3f(246 / 256.f, 174 / 256.f, 45 / 256.f);
	
	glPushMatrix();

	glTranslatef(0.0f, h_2, 0.0f);
	glScalef(1.04f, 1.1f, 1.0f);
	glTranslatef(0.0f, -h_2, 0.0f);

	glBegin(GL_TRIANGLE_STRIP);

	glColor3f(246 / 256.f, 174 / 256.f, 45 / 256.f);
	glVertex3f(-w_2 * 0.40f, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2 * 0.35f, h_2 * 0.86f, -PROFUNDIDAD_HUD);
	glVertex3f(w_2 * 0.35f, h_2 * 0.86f, -PROFUNDIDAD_HUD);
	glVertex3f(w_2 * 0.40f, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2 * 0.40f, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(0.f, h_2 * 0.86f, -PROFUNDIDAD_HUD);

	glEnd();
	glPopMatrix();

	glBegin(GL_TRIANGLE_STRIP);

	glColor3f(242 / 256.f, 100 / 256.f, 25 / 256.f);
	glVertex3f(-w_2 * 0.40f, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2 * 0.35f, h_2 * 0.86f, -PROFUNDIDAD_HUD);
	glVertex3f(w_2 * 0.35f, h_2 * 0.86f, -PROFUNDIDAD_HUD);
	glVertex3f(w_2 * 0.40f, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2 * 0.40f, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(0.f, h_2 * 0.86f, -PROFUNDIDAD_HUD);

	glEnd();

	
	

	// Marco de Tiempo

	glPushMatrix();

	glTranslatef(-w_2, h_2, 0.0f);
	glScalef(1.04f, 1.1f, 1.0f);
	glTranslatef(w_2, -h_2, 0.0f);

	glBegin(GL_TRIANGLE_STRIP);

	glColor3f(246 / 256.f, 174 / 256.f, 45 / 256.f);
	glVertex3f(-w_2, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2, h_2 * 0.80f, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2 * 0.70f, h_2 * 0.80f, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2 * 0.60f, h_2 * 0.90f, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2 * 0.60f, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2 * 0.70f, h_2 * 0.80f, -PROFUNDIDAD_HUD);

	glEnd();
	glPopMatrix();

	glBegin(GL_TRIANGLE_STRIP);

	glColor3f(242 / 256.f, 100 / 256.f, 25 / 256.f);
	glVertex3f(-w_2, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2, h_2 * 0.80f, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2 * 0.70f, h_2 * 0.80f, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2 * 0.60f, h_2 * 0.90f, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2 * 0.60f, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(-w_2 * 0.70f, h_2 * 0.80f, -PROFUNDIDAD_HUD);

	glEnd();



	// Marco de puntos


	glPushMatrix();

	glTranslatef(w_2, h_2, 0.0f);
	glScalef(1.04f, 1.1f, 1.0f);
	glTranslatef(-w_2, -h_2, 0.0f);
	

	glBegin(GL_TRIANGLE_STRIP);

	glColor3f(246 / 256.f, 174 / 256.f, 45 / 256.f);
	glVertex3f(w_2, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(w_2, h_2 * 0.80f, -PROFUNDIDAD_HUD);
	glVertex3f(w_2 * 0.70f, h_2 * 0.80f, -PROFUNDIDAD_HUD);
	glVertex3f(w_2 * 0.60f, h_2 * 0.90f, -PROFUNDIDAD_HUD);
	glVertex3f(w_2 * 0.60f, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(w_2, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(w_2 * 0.70f, h_2 * 0.80f, -PROFUNDIDAD_HUD);

	glEnd();

	glPopMatrix();

	glBegin(GL_TRIANGLE_STRIP);

	glColor3f(242 / 256.f, 100 / 256.f, 25 / 256.f);
	glVertex3f(w_2, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(w_2, h_2 * 0.80f, -PROFUNDIDAD_HUD);
	glVertex3f(w_2 * 0.70f, h_2 * 0.80f, -PROFUNDIDAD_HUD);
	glVertex3f(w_2 * 0.60f, h_2 * 0.90f, -PROFUNDIDAD_HUD);
	glVertex3f(w_2 * 0.60f, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(w_2, h_2, -PROFUNDIDAD_HUD);
	glVertex3f(w_2 * 0.70f, h_2 * 0.80f, -PROFUNDIDAD_HUD);

	glEnd();


	
	
}