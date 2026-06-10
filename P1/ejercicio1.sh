#!/bin/bash

if [ $# -ne 1 ]
then
    echo "Error..."
    exit -1
fi

if [ ! -d $1 ]
then
    echo "Error no es un directorio."
    exit -1
fi

directorio=$1


fichero=$(find $directorio -type f | head -n 1) 
lineas=$(cat $fichero | wc -l) 


for f in $(find "$directorio" -type f)
do
    lineas_fichero=$(cat "$f" | wc -l)
    if [ "$lineas_fichero" -ne "$lineas" ]
    then
        echo "Error: El fichero $f no tiene el mismo número de líneas."
        exit -1
    fi
done


for ((i=1;$i<=$lineas;i=$i+1))  
do
    salida_find=$(find $directorio -type f) 
    suma=0
    for alumno in $salida_find 
    do
        asiste=$(cat $alumno | head -n $i | tail -n 1) 
        let suma=$suma+$asiste 
    done

    echo "Asistieron $suma personas a la sesión $i."
done



