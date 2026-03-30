#!/bin/bash

if [ $# -ne 1 ] && [ $# -ne 2 ]
then
    echo "Uso: $0 <longitud de la cadena> [tipo de cadena]"
    exit -1
fi

tamCadena=$1

if [ $tamCadena -le 0 ]
then
    echo "Error, el primer argumento debe ser positivo."
    exit -1
fi

if [ $# -eq 2 ]
then
    tipoCadena=$2

    if [ "$tipoCadena" != "alfa" ] && [ "$tipoCadena" != "alfanum" ] && [ "$tipoCadena" != "num" ]
    then
        echo "Error, el segundo argumento debe ser alfa, alfanum o num."
        exit -1
    fi
else
    tipoCadena=""
    while :
    do
        echo "Introduce el tipo de cadena: "
        read tipoCadena

        if [ "$tipoCadena" == "alfa" ] || [ "$tipoCadena" == "num" ] || [ "$tipoCadena" == "alfanum" ]
        then
            break
        fi
    done
fi

if [ "$tipoCadena" == "alfa" ]
then
    op="a-zA-Z"
elif [ "$tipoCadena" == "alfanum" ]
then
    op="a-zA-Z0-9"
else
    op="0-9"
fi

cadena=$(tr -dc $op < /dev/urandom | head -c $tamCadena)

echo $cadena

# echo "Introduce un caracter: "
# read -t 5 -n 1 caracter

# if [ -z $caracter ]
# then
#     caracter="a"
# fi

# echo "El caracter es $caracter"