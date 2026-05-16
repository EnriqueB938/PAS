#!/bin/bash

if [ $# -ne 1 ]; then
  echo "Debe proporcionar el archivo como argumento."
  exit 1
fi

archivo="$1"

while read linea
do
    echo $linea | grep -E 'Título'
    echo $linea | sed -rne 's/(Autor: .+)/| -> Autor: \1/p'
    echo $linea | sed -rne 's/(Año: .+)/| -> Año: \1/p'
    echo $linea | sed -rne 's/(Precio: .+)/| -> Precio: \1/p'
    echo $linea | sed -rne 's/\[Género: (.+)\]/| -> Género: \1/p'
done < "$archivo"