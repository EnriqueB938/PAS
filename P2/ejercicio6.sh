#!/bin/bash

ficheroAuxiliar=$(mktemp) # aquí guardo las 20 últimas líneas de .bash_history
ficheroAuxiliar2=$(mktemp) # aquí guardo el comando seguido de los argumentos

cat ~/.bash_history | tail -n 20 > $ficheroAuxiliar

while read linea
do
    comando=$(echo $linea | cut -d " " -f 1)
    argumentos=$(echo $linea | grep -Eo " " | wc -l)

    echo "$comando $argumentos" >> $ficheroAuxiliar2
done < $ficheroAuxiliar

comandos=$(cat $ficheroAuxiliar2 | cut -d " " -f 1 | sort | uniq) #guardo solo los comandos sin repetir

for comando in $comandos
do
    #max_frecuente=""
    #max_veces=0
    veces_ejecutado=$(cat $ficheroAuxiliar2 | grep -E "$comando" | wc -l)
    max_argumentos=$(cat $ficheroAuxiliar2 | grep -E "$comando" | sort | tail -n 1 | cut -d " " -f 2)

    echo "Comando: $comando"
    echo "  -> Veces ejecutado: $veces_ejecutado"
    echo "  -> Máximo número de argumentos: $max_argumentos"
done

