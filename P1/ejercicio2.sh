#!/bin/bash

if [ $# -ne 4 ]
then
    echo "Argumentos incorrectos. Uso: $0 <directorio_origen> <directorio_destino> <compresion> <sobreescribir>"
    exit -1
fi

directorioOrigen=$1
directorioDestino=$2
compresion=$3
sobreescribir=$4

if [ ! -d $directorioOrigen ]
then
    echo "Error, $directorioOrigen no existe o no es un directorio"
    exit -1
fi

if [ ! -d $directorioDestino ]
then
    mkdir $directorioDestino
fi

directoriOrigenSinBarra=$(basename $directorioOrigen)
fecha=$(date +%Y%m%d)
nombreFinal=$directoriOrigenSinBarra"_"$USER"_"$fecha".tar"

if [ $compresion -eq 1 ]
then
    nombreFinal=$nombreFinal".tar" #esto lo he cambiado antes ponia ".gz"
fi

if [ -e $directorioDestino/$nombreFinal ]
then
    echo "Ya se ha realizado esta copia hoy ($directorioDestino/$nombreFinal)."

    if [ $sobreescribir -eq 0 ]
    then
        echo "No se sobreescribirá la copia."
        exit 0
    else
        #si quiero sobreescribir
        rm $directorioDestino/$nombreFinal
    fi
fi

#gestion de q los ficheros se guarden con una antiguedad < q x segundos
# find "$directorioDestino" -type f -name "*.tar.gz" -mmin +5 -delete
# find "$directorioDestino" -type f -name "*.tar.gz" -mmin -5 > "$fichero"
#creo la copia

if [ $compresion -eq 1 ]
then
    tar -czf $nombreFinal $directorioOrigen
    # c=crear, z=comprimir(gzip), f=fichero //COMPRIMIR

else
    tar -cf $nombreFinal $directorioOrigen
    # c=crear, f=fichero //ARCHIVAR
fi
# tar -czf "$directorioDestino/$nombreFinal" "$directorioOrigen"
# echo "Copia realizada en $directorioDestino/$nombreFinal."
mv $nombreFinal $directorioDestino
echo "Copia realizada en $directorioDestino/$nombreFinal."

# numaleatorio=$(((RANDOM % 900)+100))