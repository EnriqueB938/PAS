#!/bin/bash

if [ $# -ne 1 ]
then
    echo "Error"
    exit -1
fi

fichero=$1


cat $fichero | sed -r \-e '/^ *$/d' \-e '/^[-=_]+$/d' \-e 's/^(Autor|Año|Precio|Género)/| -> \1/'