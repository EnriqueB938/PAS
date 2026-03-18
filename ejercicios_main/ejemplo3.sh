#!/bin/bash

#comparativa cadenas
cadena="Enrique"
if [ $cadena == "Enrique" ]
#-n tiene algun caracter de la cadena
#-z si la cantidad de caracteres de la cadena es 0

#comparativa de numeros
edad=5
if [ $edad -eq 0 ]

#comprobacion de ficheros
fichero=

if [ -e $fichero ] #comprobacion de que existe
if [ -d $fichero ] #comprobacion de que sea una carpeta
if [ -f $fichero ] #comprobacion de un fichero normal, es decir, que no es un directorio
if [ -r $fichero ] #comprobacion de permisos de lectura
if [ -w $fichero ] #comprobacion de permisos de escritura
if [ -x $fichero ] #comprobacion de permisos de ejecución


if [ $edad -ge 12 ] && [ $edad -le 17 ]
then

fi

for variable in lista
do

done


for x in 1 7 Angel 24
do

done


#Implementar un script que reciba por línea de argumentos el nombre de un directorio.
#Si el directorio no existe, se debe crear.
#Además se debe mostrar por pantalla cuántos ficheros hay en el directorio actual
#Aqui el nombre del ejecutable no cuenta como argumento.

if [ $# -ne 1 ] #Comprueba que haya un solo argumento
then
    echo "Error, debes introducir un argumento obligatoriamente."
    exit -1
fi

if [ ! -d $1 ] #Compruebe que lo que has pasado es un directorio
then
    echo "$1 no existe, se proceder a crear el directorio"
    mkdir $1
else
    echo "$1 ya existe"
fi

cont=0
salida_ls=$(ls)

for x in $salida_ls
do
    if [ -f $x ]
    then
        let cont=$cont+1
    fi
done

echo "En el directorio actual hay $cont ficheros."

#Comando mas importante (stat), son las estadisticas de un fichero  EJEMPLO: stat -c %A ejemplo4.sh
#Comnado wc, nos permite contar cualquier cosa