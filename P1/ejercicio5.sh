#!/bin/bash

if [ $# -ne 1 ]
then
    echo "Error en línea de argumentos, debes introducir un directorio como argumento."
    exit -1
fi

if [ ! -d $1 ]
then
    echo "Error, lo que has introducido no es un directorio."
    exit -1
fi

directorio=$1
fichero=$(mktemp)

#find $directorio -type f -name "*.txt" > $fichero

find $directorio > $fichero

while read linea
do
    if [ -f $linea ]
    then
    nombre=$(basename $linea)
    ruta=$(realpath $linea)
    # longitud=${#ruta}
    bytes=$(stat -c %s $linea)
    permisos=$(stat -c %A $linea)
    # propietario=$(stat -c "User: %U (%u) | Group: %G (%g)" $linea)
    fecha=$(stat -c %Y $linea)
    # inodo=$(stat -c %i $linea)


    echo -e "$nombre\t$ruta\t$fecha\t$bytes\t$permisos"
    fi
    # Usamos -k3n para decirle a sort que ordene numéricamente (-n) usando la tercera columna (-k3), que es la de la fecha
done < "$fichero" | sort -k3n # sort | tail -n lo q sea

rm "$fichero"



