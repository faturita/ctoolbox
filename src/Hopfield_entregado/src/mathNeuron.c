#include "mathNeuron.h"

/*
 * Devuelve un numero al azar entre iMin e iMax.
 */
int getProb(int iMin, int iMax)
{
    return (iMin+(int)(iMax*1.0*rand()/(RAND_MAX+1.0)));
}

/**
 * P() constante entre 0 y 1
 */
float getNaturalProb()
{
    return ((float)(1.0*rand()/(RAND_MAX+1.0)));
}

/**
 * P() contante entre iMin y iMax
 */
float getNaturalMinMaxProb(float iMin, float iMax)
{
	return (iMin+(float)((iMax-iMin)*1.0*rand()/(RAND_MAX+1.0)));
}
/*
 * Devuelve el signo de fVal.
 */
int sgn(float fVal)
{
	if (fVal < 0)
		return (-1);
	else
		return (1);
}

/**
 * Devuelve el valor de la tangente hiperbolica
 */
float tanhsigmoid(float fVal) {
	return ( (exp( fVal ) - exp ( -fVal ))/(exp(fVal) + exp(-fVal)));
}

/**
 * Devuelve el valor de la exponencial
 */
float expsigmoid(float fVal) {
	return (  1.0 / (1 + exp( -fVal )));
}

/*
 * Inicializa las funciones de numeros al azar.
 */
void initRandom()
{
    time_t tim;
    time(&tim);
    srand((unsigned int)(tim));
}
