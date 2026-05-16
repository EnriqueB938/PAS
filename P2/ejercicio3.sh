#!/bin/bash

if [ $# -lt 1 ]
then
    echo "Por favor, proporciona el archivo de acceso"
    exit -1
fi

if [ $# -lt 2 ]
then
    echo "Por favor, pon "repite-si" o "repite-no""
    exit -1
fi


if [ ! -f $1 ]
then 
    echo "Error: El archivo $1 no exite"
    exit -1
fi

fichero=$1
# Usamos sed -rn para capturar la IP (bolsa 1) y la URL antes de HTTP (bolsa 2)
datos=$(grep -E " 200 " "$1" | sed -rne 's/^([0-9.]+) .* ".* (.*) HTTP.*/\1 \2/p' | sort)
#datos=$(grep -E " 200 " "$fichero" | sed -rne 's/^([0-9.]+) .* "(GET|POST) (.*) HTTP.*/\1 \2 \3/p' | sort)


if [ "$2" != "repite-si" ] && [ "$2" != "repite-no" ]
then
    echo "Modo no válido. Usa 'repite-si' o 'repite-no'."
    exit -1
fi
if [ "$2" == "repite-si" ]
then
    echo "$datos"
else
    # repite-no: uniq -c pone el número de repeticiones al principio de la línea.
    # Usamos sed para capturar ese número (bolsa 1) y el texto IP+URL (bolsa 2)
    # y lo reorganizamos al formato "IP URL -> X veces"
    echo "$datos" | uniq -c | sed -rn 's/^ *([0-9]+) (.*)/\2 -> \1 veces/p'
fi

