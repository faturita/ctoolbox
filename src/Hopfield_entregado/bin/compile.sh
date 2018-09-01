gcc ../src/parameter.c ../src/gridParser.c ../src/HopfieldNet.c ../src/mathNeuron.c -o HopfieldNet /usr/lib/libm.so
gcc ../src/mathNeuron.c ../src/Ising.c -o Ising /usr/lib/libm.so
gcc ../src/parameter.c ../src/gridParser.c ../src/HopfieldNet84.c ../src/mathNeuron.c -o HopfieldNet84 /usr/lib/libm.so 
gcc ../src/parameter.c ../src/Perceptron.c ../src/arrayView.c ../src/mathNeuron.c ../src/logger.c -o Perceptron /usr/lib/libm.so
