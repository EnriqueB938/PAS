#!/bin/bash

if [ $# -ne 2 ]
then
    echo "Error..."
    exit -1
fi

fichero=$1
opcion=$2

if [ $opcion != "repite-si" ] && [ $opcion != "repite-no" ]
then
    echo "Error..."
    exit -1
fi


if [ "$opcion" == "repite-si" ]
then
    cat $fichero | cut -d ' ' -f 1,7 | sort -k1
elif [ "$opcion" == "repite-no" ]
then
    cat $fichero | cut -d ' ' -f 1,7 | sort -k1 | uniq -c | sed -r 's/^ *([0-9]+) (.*)/\2 --> \1 veces/'
fi


