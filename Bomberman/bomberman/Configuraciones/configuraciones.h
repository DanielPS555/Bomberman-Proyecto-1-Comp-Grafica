
#include "stdio.h"


#ifndef CONFIGURACIONES
#define CONFIGURACIONES

#define SCREEN_WIDTH 1500.0f
#define SCREEN_HEIGHT 800.0f

#define CAMARA_HORIZONTAL_INVERTIDA false  //Si es false, cuando muevo el mouse a la derecha, la camara rota a la izquierda
#define CAMARA_VERTICAL_INVERTIDA false    //Si es false, cuando muevo el mouse a la arriba, la camara rota hacia abajo
#define SENSIBILIDAD_CAMARA 0.3f           //Es la sensibilidad que camara al movimiento del mouse 
#define VELOCIDAD_JUEGO 1.0f


enum MODOS_ILUMINACION {
	MODOS_ILUMINACION_ATARDESER,
	MODOS_ILUMINACION_NOCHE
};


class configuraciones{
private:
	bool isCamaraHorizontalInvertida;
	bool isCamaraVerticalInvertida;

	float sensibilidadCamara;
	float velocidadJuego;

	MODOS_ILUMINACION modoIluminacion;

	configuraciones();
	~configuraciones();

public:


	configuraciones(const configuraciones& obj)= delete;

	static configuraciones* getInstancia() {
		static configuraciones* const inst = new configuraciones{}; // Mirar esta forma de crear el singleton en esta pagina: https://dev.to/pauljlucas/singletons-in-c-4ep2
																	// Evita el problema de fiasco https://isocpp.org/wiki/faq/ctors#static-init-order
		return inst;
	}

	bool getIsCamaraHorizontalInvertida();
	bool getIsCamaraVerticalInvertida();
	float getSensibilidadCamara();

	MODOS_ILUMINACION getModoIluminacion();


	void setIsCamaraHorizontalInvertida(bool);
	void setIsCamaraVerticalInvertida(bool);
	void setSensibilidadCamaraa(float);
	void setVelocidadJuego(float);
	float getVelocidadJuego();

	void setModoIluminacion(MODOS_ILUMINACION);

};

#endif 