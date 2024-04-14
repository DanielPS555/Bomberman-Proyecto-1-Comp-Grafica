#include "mathVector.h"

#include <iostream>

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES  // for C
#endif 
#include <math.h>



void print(mathVector vector) {
	std::cout << vector.x << "|" << vector.y << "|" << vector.z << std::endl;
}

mathVector sumar(mathVector vec1, mathVector vec2) {
	return {
		vec1.x + vec2.x,
		vec1.y + vec2.y,
		vec1.z + vec2.z
	};	
}

mathVector restar(mathVector vec1, mathVector vec2) {
	return {
		vec1.x - vec2.x,
		vec1.y - vec2.y,
		vec1.z - vec2.z
	};
}

mathVector multiplicarPorEscalar(mathVector vec1, float escalar) {
	return {
		vec1.x * escalar,
		vec1.y * escalar,
		vec1.z * escalar
	};
}

float productoEscalar(mathVector vec1, mathVector vec2) {
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

mathVector productoVectorial(mathVector vec1, mathVector vec2) {
	return {
		vec1.y * vec2.z - vec1.z * vec2.y,
		vec1.z * vec2.x - vec1.x * vec2.z,
		vec1.x * vec2.y - vec1.y * vec2.x
	};
}

float angulo(mathVector vec1, mathVector vec2) {
	float normaA = norma(vec1);
	float normaB = norma(vec2);
	float proEsc = productoEscalar(vec1, vec2);
	return (float)acos(proEsc / (normaA * normaB));
}

float norma(mathVector vec1) {
	return sqrt(productoEscalar(vec1, vec1));
}

mathVector normalizar(mathVector vec) {
	float invertidoNorma = 1 / norma(vec);
	return multiplicarPorEscalar(vec, invertidoNorma);
}

bool isNulo(mathVector vec) {
	return vec.x == 0 && vec.y == 0 && vec.z == 0;
}

mathVector rotar(mathVector vec, float anguloEnGrado) {
	float rad = (anguloEnGrado * M_PI) / 180.0f;
	return {
		cos(rad) * vec.x - sin(rad) * vec.y,
		sin(rad) * vec.x + cos(rad) * vec.y,
		vec.z
	};
}
