class jugador{
private:
	float posXEnMapa, posYEnMapa, posZEnMapa;
	float anguloActualEnMapa; //Angulo con respecto al eje x
public:
	jugador(float posXEnMapaInicial, float posYEnMapaInicial, float posZEnMapaInicial, float anguloInicial);

	~jugador();

	// ToDo queda refactoriacion por realizar
	void trasladar(float deltaTiempo,
		bool isMoviendoArriba,
		bool isMoviendoDerecha,
		bool isMoviendoAbajo,
		bool isMoviendoIsquierda,
		float deltaAngulo);


	float getPosicionXEnMapa();

	float getPosicionYEnMapa();

	float getPosicionZEnMapa();

	float getAnguloActualEnMapa();

};

