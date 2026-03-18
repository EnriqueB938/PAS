#!/bin/bash

#Implementar un script que reciba por línea de argumentos el nombre de un directorio.
#Se debe mostrar, solo de los ficheros, el nombre del fichero (sin ruta), el número de 
#letras que tiene el nombre del fichero y la cadena de permisos del fichero. Se debe
#mostrar todo separado por ";"  
#La salida debe estar ordenada por el número de letras descendentemente.

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

# variable = $(comando)
fichero=$(mktemp) #Archivo temporal

find $directorio > $fichero

while read linea
do
    if [ -f $linea ]
    then
        nombre=$(basename $linea)
        letras=$(echo -n $nombre | wc -m)
        permisos=$(stat -c %A $linea)

        echo "$nombre;$letras;$permisos"
    fi
done < $fichero | sort -t ";" -k 2 -n -r  #Funcion Sort que ordena (-t separa), (-k = columna), (-n = numerico), (-r = reversa = descendente)

rm $fichero



#COMANDOS INTERESANTES:
#wc
#sort
#mktemp
#stat
#find, tmb podemos usarla asi find -typed (carpeta)       find -name "*out"

#tar (Comprimir o archivar)
#ping (Pasamos una IP o dominio, velocidad de la ruta del servidor)
#cat (Muestra fichero)
#head -n (10 primeras lineas del fichero)
#tail -n (10 primeras lineas del fichero)
#date (Hora actual)

#EJERCICIOS:

#En el ejercicio 2 (c,z,f del comando tar)
#Ejercicio 3 (muy facil)
#Ejercicio 4 (captura)
