/**
 * Funciones matematicas para el manejo de estructuras y redes de neuronas
 *
 *
 **/
#include <math.h>
#include <stdlib.h>
#include <time.h>

/**
 * Devuelve un numero de densidad constante entre iMin e iMax
 *
 **/
int getProb(int iMin, int iMax);

/**
 * Devuelve un float de densidad constante entre iMin e iMax
 *
 **/
float getNaturalMinMaxProb(float iMin, float iMax);

/**
 * Devuelve un numero entre 0 y 1 con densidad de probabilidad constante
 *
 **/
float getNaturalProb();

/**
 * Funcion signo
 **/
int sgn(float fVal);

/**
 * Funcion targente hiperbolica
 **/
float tanhsigmoid(float fVal);

/**
 * Funcion exponencial
 **/
float expsigmoid(float fVal);

/**
 * Inicializa la semilla de los numeros pseudoaleatorios con la hora actual.
 **/
void initRandom();