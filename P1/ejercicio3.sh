#!/bin/bash

# ./ejercicio3.sh FicherosEjercicio3/home

if [ $# -ne 1 ]
then
    echo "Error..."
    exit -1
fi

directorio=$1

if [ ! -d $directorio ]
then
    echo "Error...."
    exit -1
fi

salida_find=$(find $directorio -maxdepth 1 -mindepth 1)

for dir in $salida_find
do
    permisos_home=$(stat -c %a $dir)
    permisos_ssh=$(stat -c %a $dir/.ssh)
    permisos_clave=$(stat -c %a $dir/.ssh/id_rsa)

    if [ $permisos_home -ne 700 ] || [ $permisos_ssh -ne 700 ] || [ $permisos_clave -ne 600 ]
    then
        echo "revisa los permisos" 
        echo "revisa los permisos" > $dir/Desktop/aviso.txt
done