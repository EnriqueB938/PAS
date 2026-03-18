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

find $directorio > $fichero

while read linea
do
    if [ -f $linea ]
    then
    nombre=$(basename $linea)
    ruta=$(realpath $linea)
    bytes=$(stat -c %s $linea)
    permisos=$(stat -c %A $linea)
    fecha=$(stat -c %Y $linea)

    echo "$nombre    $ruta    $fecha    $bytes    $permisos"
    fi
done < $fichero | sort -n

rm $fichero