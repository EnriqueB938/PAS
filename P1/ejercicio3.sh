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

# Añadido '-type d' al final para que ignore el index.html
salida_find=$(find $directorio -maxdepth 1 -mindepth 1 -type d) #-maxdepth 1 limita la búsqueda a la carpeta actual (no entra en subcarpetas) 
# y -mindepth 1 excluye de los resultados a la propia carpeta de origen.

for dir in $salida_find
do

# Un 'if' para asegurar que solo hace el stat si la clave existe
if [ -f "$dir/.ssh/id_rsa" ]
    then
    permisos_home=$(stat -c %a $dir)
    permisos_ssh=$(stat -c %a $dir/.ssh)
    permisos_clave=$(stat -c %a $dir/.ssh/id_rsa)

    if [ $permisos_home -ne 700 ] || [ $permisos_ssh -ne 700 ] || [ $permisos_clave -ne 600 ]
    #numero en octal de los permisos:
    # 700 el dueño puede hacer todo 
    # 600 el dueño lee y escribe
    then
    usuario=$(basename $dir)
     echo "El usuario $usuario tiene una clave privada de ssh en $dir/.ssh/id_rsa que no está protegida. La clave debe ser accesible únicamente por el propietario."        
     echo "revisa los permisos" > $dir/Desktop/aviso.txt
    fi
fi
done

