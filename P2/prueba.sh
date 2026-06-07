#!/bin/bash


# Filtrar los usuarios cuyo nombre de usuario termina en 'a' o 'e'. Mostrar de dichos usuarios
# el nombre de usuario, el uid, el gid y el nombre de su grupo

ficheroTemporal=$(mktemp)

cat /etc/passwd | grep -E "^.+[ae]:x:.+:.+:.*:.+:.*$" | sed -rne "s/(^.+):x:(.+):(.+):.*:.+:.*$/\1:\2:\3/p" > $ficheroTemporal

while read linea
do
    usuario=$(echo $linea | cut -d ':' -f 1)
    uid=$(echo $linea | cut -d ':' -f 2)
    gid=$(echo $linea | cut -d ':' -f 3)

    grupo=$(cat /etc/group | grep -E "^.+:x:$gid:.*$" | cut -d ':' -f 1)

    echo "Usuario: $usuario, UID: $uid, GID: $gid, Grupo: $grupo"
done < $ficheroTemporal

rm $ficheroTemporal