#!/bin/bash

#grep filtra y el sed y/o modifica

if [ $# -ne 1 ]
then
    echo "Error"
    exit -1
fi

fichero=$1

#Apartado 1

#Apartado 2

#Apartado 3

#Apartado 4

#Apartado 5

#Apartado 6

#Apartado 7

#Apartado 8

#Apartado 9
echo "Líneas con dos o más palabras con mayúsculas consecutivas:"
cat $fichero | grep -E "^,+ [A-Z][[:alpha]]+ [A-Z][[:alpha]]+"

