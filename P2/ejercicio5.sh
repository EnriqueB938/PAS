#!/bin/bash

function convertirMascara()
{
    numeros=$(echo $1 | tr '.' ' ')
    suma=0
    for numero in $numeros
    do
        let suma=$suma+$(echo "obase=2; ibase=10; $numero" | bc | grep -E -o "1" | wc -l)
    done
    echo $suma
}

interfaces=$(ifconfig | grep -E "^[[:alpha:]]" | cut -d " " -f 1 | tr -d ':')

for interfaz in $interfaces
do
    #Obtengo la ip que está a la derecha de inet
    ip=$(ifconfig $interfaz | grep "inet " | tr -s " " | cut -d " " -f 3 | cut -d ":" -f 2)
    #obtengo la máscara
    mascara=$(ifconfig $interfaz | grep "inet " | tr -s " " | cut -d ":" -f 4)
    broadcast=$(ifconfig $interfaz | grep "inet " | tr -s " " | cut -d " " -f 7)
    mascara_buena=$(convertirMascara $mascara)
    #obtengo si el cable está conectado o no
    cable=$(ethtool $interfaz 2> /dev/null | grep -E "Link detected: yes")
    if [ -z "$cable" ]
    then
        cable="No"
    else
        cable="Sí"
    fi
    
    #Muestro la info
    echo "Interfaz: $interfaz"
    echo -e "\t->Dirección IP: $ip"
    echo -e "\t->Máscara de red: $mascara_buena bits"
    echo -e "\t->Cable conectado: $cable"

done

#otros valores del interfaz
# mtu=$(ifconfig $interfaz | grep -E "mtu" | cut -d " " -f 5)
# rx=$( ifconfig $interfaz | grep -E "RX" | tr -s ' ' | cut -d ' ' -f 6), tx cambiar grep -E "TX"
