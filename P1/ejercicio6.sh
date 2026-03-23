#!/bin/bash

function generarHtml() 
{
    echo "<!DOCTYPE html>" > $1/index.html
    echo "<html>" >> $1/index.html
    echo "<body>" >> $1/index.html

    echo "<h1>Contenido de $1</h1>" >> $1/index.html

    echo "<ul>" >> $1/index.html

    for file in $(find $1 -maxdepth 1)
    do
        solo_nombre=$(basename $file)
        if [ ! -d $file ]
        then
            echo "<li>$solo_nombre</li>" >> $1/index.html
        elif [ "$solo_nombre" != "$(basename $1)" ]
        then
            dir=$(basename $file)
            echo "<li><a href=\"$dir/index.html\">$solo_nombre</a></li>" >> $1/index.html 
            generarHtml $file          
        fi
    done

    echo "</ul>" >> $1/index.html

    echo "</body>" >> $1/index.html
    echo "</html>" >> $1/index.html

    #muestro el mensaje por terminal
    echo "Se ha creado el fichero $1/index.html con el contenido del directorio $1."
}

#main
if [ $# -ne 1 ]
then
    echo "Error..."
    exit -1
fi

generarHtml $1
