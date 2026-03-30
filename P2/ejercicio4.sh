#!/bin/bash

fichero="/etc/passwd"

#Apartado 1
echo "1. Usuarios cuyo nombre empieza con la letra 'l': "
cat $fichero | grep -E "^l" | sed -r 's/:.*//'

#Apartado 2
echo "2. Usuarios con shell válido: "
cat $fichero | grep -v -E "bin/false$|/usr/bin/nologin$" | sed -r 's/:.*//'

#Apartado 3
echo "3. UID de los usuarios cuyo directorio home no está en home: "
cat $fichero | grep -v -E 