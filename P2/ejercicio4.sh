#!/bin/bash

echo "1. Usuarios cuyo nombre empieza con la letra 'l':"
#  dos letras iguales seguidas nombre=$(echo "$user" | grep -E "([a-zA-Z])\1")
cat /etc/passwd | grep -E "^l" | cut -d ":" -f 1
#cat /etc/passwd | grep -E "^l" | sed -rne "s/(^.+):x:.+:.+:.*:.+:.*$/\1/p"
#cat /etc/passwd | sed -rne "s/(^l.+):x:.+:.+:.*:.+:.*$/\1/p"

echo "2. Usuarios con shell válido:"
cat /etc/passwd | grep -vE "(/bin/false|/usr/bin/nologin)$" | cut -d: -f1

echo "3. UID de los usuarios cuyo directorio home no está en home:"
cat /etc/passwd | grep -Ev "/home/" | cut -d ":" -f 3

echo "4. Usuarios con un GID mayor que 1000:"
# cat /etc/passwd | sed -rne "s/(^.+):x:....+:.+:.*:.+:.*$/\1/p"
# cat /etc/passwd | sed -rne "s/(^.+):x:[0-9]{4,}:.+:.*:.+:.*$/\1/p"
cat /etc/passwd | cut -d ":" -f 1,4 | grep -E ":[0-9]{4,}$" | grep -v ":1000$" | cut -d ":" -f 1

echo "5. Usuarios y su UID con una ',' en su gecos:"
cat /etc/passwd | cut -d ":" -f 1,3,5 | grep -E ".*:.*:.*," | cut -d ":" -f 1,2 | sed -rne 's/:/, /p'
#usuario:*:UID:GID:GECOS:HOME:SHELL
#gecos es el unico que puede estar vacío

#para encontrar del grupo primario
#grupo=$(cat /etc/group | grep ":$gid:" | cut -d ":" -f 1)
