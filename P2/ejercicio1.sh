#!/bin/bash

if [ $# -ne 1 ]
then
    echo "Argumentos err ́oneos. Uso: $0 <fichero_libros.txt>"
    exit -1
fi

if [ ! -f "$1" ]
then
    echo "Error, el archivo $1 no existe"
    exit 1
fi

fichero=$1

    echo "1) Años de los libros publicados entre 1950 y 2020: "
    cat $fichero | grep -E "^Año: (19[5-9][0-9]|20[0-1][0-9]|2020)" | sed -rne 's/^Año: (.*)/\1/p'
    # Luego con el sed cojo Año: y luego un caracter o mas que son los años, y muestro solo los años \|
    echo "2) Precios superiores a 20 euros:"
    cat $fichero | grep -E "[2-9][1-9],[0-9]{2}" | sed -rne 's/.*: (.*)/\1/p'    
    #estructura del sed: s/BUSCAR/REEMPLAZAR/
    # entonces el 's es para sustituir, entonces le digo que busque un caracter o mas y : (.*:) y luego un caracter o mas y lo guardo en \1
    # y luego \1 q son los numeros es lo que muestro.
    echo "3) Número de libros por género:"
    cat $fichero | sed -rne "s/\[Género: (.+)\]/\1/p" | sort | uniq -c | sed -rne "s/ +([0-9]+) (.+)/\2 aparece \1 veces/p"
    #primero con el grep cojo los primero que literalmente ponen (\) [Género: ]
    # luego con el sed dejo solo el nombre de los generos
    # luego los ordeno alfabeticamente con el sort, y con el uniq -c se cuentan solos
    # con el ultimo sed hago que ponga literalmente lo que ponen en el enunciado
    #NOTA: en el ultimo sed pongo ^[ ]* pq el sort deja espacios en blanco que hay que coger
    echo "4) Palabras de al menos 8 caracteres que empiezan por consonante y terminan por vocal:"
    cat $fichero | tr ' ' '\n' | tr -d '[]:' | grep -E ".{8,}" | grep -Ev "^[aeiouAEIOU0-9]" | grep -E -i "[aeiou]$"
    #el tr ' ' '\n' sustituye el espacion por un salto de linea para separar las palabras
    # la v delante del grep es lo contrario, osea que si cojo las que empiezan por vocales, me coje las que no empiezan por las vocales
    # con el grep -E ".{8,}" decimos que al menos tenga un caracter, asi eliminamos los espacios y tb cogemos las palabras que al menos tengan 8 caracteres
    # tr -d , la de es de delete, asi cuando ponemos tr -d '[]:' borramos los corchetes y los :
    # grep -i, ignora entre mayuscula y minuscula, el $ es para el final de linea
    echo "5) Líneas de autor cuyo nombre o apellido contiene una doble "l":"
    cat $fichero | grep -E "\Autor:" | grep -E "\ll"
    #Primero filtro por los que empiezan por literalmente Autor:
    # Y luego filtro por los que tienen literalmente "ll"
    echo "6) Títulos de libros con m ́as de tres palabras:"
    cat $fichero | grep -E "\Título" | sed -rne 's/(Título:) (.* .* .*)/\1\2/p'
    # Primero filtro por los que primero llevan literalmente "Título"
    # Luego con el sed cojo el titulo y luego si llevan minimo 3 palabras, y los muestro tal cual
    # Asi todos los que me salen tienen como minimo 3 palabras
    echo "7) Títulos de libros cuyo precio termina en ,99€"
    cat $fichero | grep -E ",99€" -B 3 | grep -E "Título"
    # el grep -E ",99€" -B 3 , coges de las lineas que terminan por ",99" coges las 3 lineas anteriores
    echo "8) Número de libros con a ̃no de publicaci ́on anterior a 2000:"
    cat $fichero | grep -E "\Año: 19[0-9][0-9]" | wc -l
    #con el grep filtro los que empiezan por los que literalmente empiezan por Año: entre 1900-1999
    # Y luego con el wc -l cuento las lineas
    echo "9) Lıneas con dos o más palabras con may ́uscula consecutivas:"
    cat $fichero | grep -E "^.+ [A-Z][[:alpha:]]+ [A-Z][[:alpha:]]"
    #Luego un espacio,luego una mayuscula seguido de un caracter alfabetico o mas 
    #un espacio y luego otra vez lo mismo
    echo "10) Géneros con una palabra compuesta:"
    cat $fichero | grep -E "\Género: .+-.+" | sort | uniq
    #con el grep cojo los que empiezan literalmente por Género: un caracter o mas , un guion(-), y luego otro caractero o mas
    # con el sort los ordeno alfabeticamente y con el uniq no me aparecen repetidos