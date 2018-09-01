Trabajo Pr�ctico N�mero 2
Redes Back-Propagation

Autor: Rodrigo Ramele
Materia: Redes Neuronales Primer Cuatrimestre 2003

Plataforma Implementaci�n:

Lenguaje: ANSI-C
Plataforma: IBM-PC i586 Linux Mandrake 7.1
Gcc: gcc version 2.95.3 19991030 (prerelease)

Descripci�n:
-----------

La red se construye dinamicamente en base a los parametros del archivo
de configuraci�n.  La estructura que se arma es un array con las dimensiones
de cada layer, considerando a la primera posicion como el tama�o del vector de
entrada N.  Para la entrada se agrega una posici�n m�s para el umbral, que se
fija siempre en -1.  Se genera un array de matrices de pesos sin�ticos, una matriz
para cada layer.  Los pesos sinapticos se inicializan con valores entre -1 y 1 y la
funcion de activacion que se utiliza es la tangente hiperb�lica.
Los patrones del trainning set se leen tambi�n de un archivo de configuraci�n que se
especifica en el archivo de configuraci�n principal.  
Una vez aprendidos los patrones, los mismos se prueban para verificar si la red los
aprendi� adecuadamente.


Sin�psis:
---------
./bin/Perceptron [archivo de configuracion inicial]

Arma las estrucutras necesarias seg�n el archivo de configuraci�n, entrena a 
la red con los patrones, y verifica que los mismos hayan sido bien aprendidos.


Par�metros del archivo de configuraci�n:
---------------------------------------

*layer.size		Cantidad de layers de neuronas de la red (sin contar las entradas)
*layer.0
*layer.1
*layer.n		Cantidad de neuronas por layer.
*log.filename	Nombre del archivo de logging para ver informacion del error cuadratico medio.
*pattern.filename	Archivo de configuraci�n donde se encuentran los patrones del trainning set.
*reply.factor	Cantidad de iteraciones (aprendizaje de un patr�n) donde no se altera ning�n peso sin�ptico
*accuracy		Umbral m�nimo para comparar la variaci�n de un peso sin�ptico para considerar aprendizaje (relacionado con el par�metro anterior)
*delta.li		Factor sobre los valores de la retropropagaci�n de los errores.
*delta.weight	Factor de aprendizaje.
*showOutputFx	Parametro que indica si muestra el valor de la funcion que se intenta aproximar.

Par�metros sobre el archivo de configuraci�n de los patrones:
------------------------------------------------------------

*pattern.size	Cantidad de patrones en el trainning Set
*pattern.in.0.0
*pattern.in.0.1
*pattern.in.i.j	Valor de entrada del patron muestra i, sobre la posici�n j
*pattern.out.i.j	Valor de salida del patron muestra i, sobre la posicion j

Archivos de configuraci�n:
-------------------------

layerperceptron.and	Archivo de configuraci�n para simular una AND de 2 entradas con 2-1
layerperceptron.xor	Archivo de configuraci�n para simular una XOR de 4 entradas con 4-4-1
layerperceptron.fx	Archivo de configuraci�n para simular la f(x,y,z)=sin(x)+cos(y)+z con 3-5-1

Dumps:
------

En ./dumps/ se encuentran las salidas de los procesos ejecutados:

salida.and.log		Salida de los valores aprendidos para una f. l�gica AND de 2 entradas.
salida.xor.log		Salida de los valores aprendidos para una f. l�gica XOR de 4 entradas.
salida.fx.log		Salida de los valores aprendidos para una fxyz = sin(x) + cos(x) + z.
layerperceptron.fx.log	Salida del error cuadratico medio sobre el valor de la fxyz y la salidas de la red.
convergencia.fx.xls	Planilla EXCEL con el grafico de convergencia a cero del error cuadr�tico medio para la simulaci�n de fxyz
grafico.fx.xls		Planilla EXCEL con el gr�fico de regresi�n de los valores obtenidos por la fxyz y los obtenidos como salidas de la red.
