Trabajo Práctico Número 2
Redes Back-Propagation

Autor: Rodrigo Ramele
Materia: Redes Neuronales Primer Cuatrimestre 2003

Plataforma Implementación:

Lenguaje: ANSI-C
Plataforma: IBM-PC i586 Linux Mandrake 7.1
Gcc: gcc version 2.95.3 19991030 (prerelease)

Descripción:
-----------

La red se construye dinamicamente en base a los parametros del archivo
de configuración.  La estructura que se arma es un array con las dimensiones
de cada layer, considerando a la primera posicion como el tamaño del vector de
entrada N.  Para la entrada se agrega una posición más para el umbral, que se
fija siempre en -1.  Se genera un array de matrices de pesos sináticos, una matriz
para cada layer.  Los pesos sinapticos se inicializan con valores entre -1 y 1 y la
funcion de activacion que se utiliza es la tangente hiperbólica.
Los patrones del trainning set se leen también de un archivo de configuración que se
especifica en el archivo de configuración principal.  
Una vez aprendidos los patrones, los mismos se prueban para verificar si la red los
aprendió adecuadamente.


Sinápsis:
---------
./bin/Perceptron [archivo de configuracion inicial]

Arma las estrucutras necesarias según el archivo de configuración, entrena a 
la red con los patrones, y verifica que los mismos hayan sido bien aprendidos.


Parámetros del archivo de configuración:
---------------------------------------

*layer.size		Cantidad de layers de neuronas de la red (sin contar las entradas)
*layer.0
*layer.1
*layer.n		Cantidad de neuronas por layer.
*log.filename	Nombre del archivo de logging para ver informacion del error cuadratico medio.
*pattern.filename	Archivo de configuración donde se encuentran los patrones del trainning set.
*reply.factor	Cantidad de iteraciones (aprendizaje de un patrón) donde no se altera ningún peso sináptico
*accuracy		Umbral mínimo para comparar la variación de un peso sináptico para considerar aprendizaje (relacionado con el parámetro anterior)
*delta.li		Factor sobre los valores de la retropropagación de los errores.
*delta.weight	Factor de aprendizaje.
*showOutputFx	Parametro que indica si muestra el valor de la funcion que se intenta aproximar.

Parámetros sobre el archivo de configuración de los patrones:
------------------------------------------------------------

*pattern.size	Cantidad de patrones en el trainning Set
*pattern.in.0.0
*pattern.in.0.1
*pattern.in.i.j	Valor de entrada del patron muestra i, sobre la posición j
*pattern.out.i.j	Valor de salida del patron muestra i, sobre la posicion j

Archivos de configuración:
-------------------------

layerperceptron.and	Archivo de configuración para simular una AND de 2 entradas con 2-1
layerperceptron.xor	Archivo de configuración para simular una XOR de 4 entradas con 4-4-1
layerperceptron.fx	Archivo de configuración para simular la f(x,y,z)=sin(x)+cos(y)+z con 3-5-1

Dumps:
------

En ./dumps/ se encuentran las salidas de los procesos ejecutados:

salida.and.log		Salida de los valores aprendidos para una f. lógica AND de 2 entradas.
salida.xor.log		Salida de los valores aprendidos para una f. lógica XOR de 4 entradas.
salida.fx.log		Salida de los valores aprendidos para una fxyz = sin(x) + cos(x) + z.
layerperceptron.fx.log	Salida del error cuadratico medio sobre el valor de la fxyz y la salidas de la red.
convergencia.fx.xls	Planilla EXCEL con el grafico de convergencia a cero del error cuadrático medio para la simulación de fxyz
grafico.fx.xls		Planilla EXCEL con el gráfico de regresión de los valores obtenidos por la fxyz y los obtenidos como salidas de la red.
