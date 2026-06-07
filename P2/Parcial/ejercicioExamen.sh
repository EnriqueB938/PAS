# Ejercicio Práctica 2 Parcial PAS: un script en Bash que reciba como parámetros un valor 
# numérico de céntimos a buscar, un valor de céntimos para sustituir (opcional) y la ruta de 
# un archivo, debiendo comprobar obligatoriamente que dicho archivo exista y cuente con la 
# extensión .txt. El fichero contiene un catálogo estructurado en bloques de información de 
# libros (Título, Autor, Precio y Género entre corchetes), separados por líneas vacías y 
# caracterizados por un espaciado irregular y el uso mixto de etiquetas en inglés y 
# español (Tittle, Author, Price). El script debe eliminar las líneas vacías del archivo y 
# procesar los bloques para mostrar por pantalla exclusivamente aquellos libros cuyos céntimos 
# coincidan con el primer argumento pasado al script; si se proporciona el argumento opcional 
# de sustitución, el precio original deberá ser actualizado con los nuevos céntimos indicados. 
# Para el formato de salida deberá encargarse de reducir los espacios múltiples a uno solo, 
# traducir cualquier etiqueta en inglés al español, suprimir los corchetes que envuelven al género 
# y tabular (\t) las líneas correspondientes al Autor, Precio y Género. Por último, tras procesar 
# todo el fichero, el script debe imprimir el número total de libros que han sido extraídos y
#  mostrados por pantalla.

if [ $# -ne 2 ] && [ $# -ne 3 ]
then
    echo "Error en línea de argumentos..."
    exit -1
fi

fichero=$1
precioBuscar=$2
precioSustituir=$3

if [ ! -e $fichero ]
then
    echo "Error, el fichero $fichero no existe"
    exit -1
fi

ficheroTemporal=$(mktemp)
ficheroTemporal2=$(mktemp)
ficheroTemporal3=$(mktemp)
ficheroTemporal4=$(mktemp)

#Eliminar las líneas vacías
cat $fichero | grep . > $ficheroTemporal

#Filtrar solo los libros que emparejen con el precio
cat $ficheroTemporal | grep -E -A 1 -B 3 --no-group-separator ",$precioBuscar€" > $ficheroTemporal2

#Traduzco lo que venga en inglés al español de España
cat $ficheroTemporal2 | sed -re "s/Tittle:/Título:/" | sed -re "s/Author:/Autor:/" | sed -re "s/Year:/Año:/" | sed -re "s/Price:/Precio:/" > $ficheroTemporal3

#Elimino los espacios seguidos innecesarios
cat $ficheroTemporal3 | sed -re "s/ {2,}/ /"  > $ficheroTemporal4

#Si he pasado el precio a sustituir, lo sustituyo
if [ ! -z "$precioSustituir" ]
then
    cat $ficheroTemporal4 | sed -re "s/,..€/,$precioSustituir€/"
else
    cat $ficheroTemporal4  
fi

#Muestro cuántos libros se han procesado
lineas=$(cat $ficheroTemporal4  | grep -E "Título:" | wc -l)
echo "Se han procesado $lineas libros"