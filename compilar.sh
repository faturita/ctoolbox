#Script compilador de C.
#Usa gcc y g++

# ------------------ Analisis de los paramtros.
if test $# -eq 0
then
    echo -e "\aScript compilador de C.\ncompilar --help: Ayuda detallada.\nEscriba: $ compilar <nombre-archivo-fuente> (opciones).\n"
    exit 1
fi

if test $1 = "--help"
then
    echo "Script compilador de C."
    echo "Usese:  compilar <nombre-archivo-fuente> (opciones)"
    echo "" 
    echo "	+     Compilar con g++"
    echo ""
    exit 
fi


# ---------------- Analisis de la existencia del archivo fuente
archivofte=$1
if test ! -f $archivofte
then
    echo -e "\aError de ejecucion. El archivo fuente no existe.\n"
    exit 1
fi

# ----------------- Armado del nombre del archivo destino.
archivodest=`ls $archivofte | cut -f1 -d.`
archivodest=`echo "$archivodest.out"`

# ----------------- Opciones de compiladores
if test $# -ge "2"
then
    if test "$2" = "+"
    then
	COMPILADOR=g++
    fi
else
    COMPILADOR=gcc
fi

if test $COMPILADOR = "g++"
then
    echo Compilando con g++....
    g++ $archivofte -o $archivodest $3 $4 $5 $6 
else
    echo Compilando con gcc....
    gcc $archivofte -o $archivodest $2 $3 $4 $5 $6
fi

 
