#ifndef MATH_VECTOR
#define MATH_VECTOR

struct mathVector {
		float x;
		float y;
		float z;
};




mathVector sumar(mathVector, mathVector);
mathVector restar(mathVector, mathVector);
mathVector multiplicarPorEscalar(mathVector, float);

float productoEscalar(mathVector, mathVector);

mathVector productoVectorial(mathVector, mathVector);
float angulo(mathVector, mathVector);

float norma(mathVector);

mathVector normalizar(mathVector);

bool isNulo(mathVector);

void print(mathVector);

mathVector rotar(mathVector, float anguloEnGrado);

// El grado de interpolacion entre el inicio de inicio y fin lo `marca t`, donde 0 es el inicio y 1 es el final
mathVector interpolarVectores(mathVector inicio, mathVector fin, float t);

#endif 


