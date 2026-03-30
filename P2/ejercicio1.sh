#!/bin/bash

#grep filtra y el sed y/o modifica

if [ $# -ne 1 ]
then
    echo "Error"
    exit -1
fi

fichero=$1

#Apartado 1
echo "1) Años de los libros publicados entre 1950 y 2020:"
cat $fichero | grep -E "^Año: (19[5-9][0-9]|20[0-1][0-9]|2020)"
#Apartado 2
echo "2) Precios superiores a 20 euros: "
cat $fichero | grep -o -E "[2-9][0-9],[0-9][0-9]€" | grep -v "20,00€"
#Apartado 3
echo "3) Número de libros por género: "
cat $fichero | grep -o -E "\[[^]]+\]" | sort | uniq -c | sed -r 's/^ *([0-9]+) \[(.*)\]/\2 aparece \1 veces/'
#Apartado 4
echo "4) Palabras de al menos 8 caracteres que empiezan por consonante y terminan por vocal: "
cat $fichero | grep -o -E "[bcdfghjklmnpqrstvwxyzBCDFGHJKLMNPQRSTVWXYZ][a-záéíóú]{6,}[aeiou]"
#Apartado 5
echo "5) Líneas de autor cuyo nombre o apellido contiene una doble l: "
cat $fichero | grep -E "^Autor: .*[Ll]l.*"
#Apartado 6
echo "6) Títulos de libros con más de tres palabras: "
cat $fichero | grep -E "^Título: [^ ]+( [^ ]+){3,}"
#Apartado 7
echo "7) Títulos de libros cuyo precio termina en ,99€: "
cat $fichero | grep -B 5 ",99€" | grep -E "^Título: "
#Apartado 8
echo "8) Número de libros con año de publicación anterior a 2000: "
cat $fichero | grep -E "^Año: 1[0-9]{3}" | wc -l
#Apartado 9
echo "9) Líneas con dos o más palabras con mayúsculas consecutivas:"
cat $fichero | grep -E "^.+ [A-Z][[:alpha:]]+ [A-Z][[:alpha]]"
#Apartado 10
echo "10) Géneros con una palabra compuesta: "
cat $fichero | grep -o -E "\[[^]]+-[^]]+\]" | sort | uniq
