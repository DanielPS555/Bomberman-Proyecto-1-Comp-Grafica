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

#endif 


