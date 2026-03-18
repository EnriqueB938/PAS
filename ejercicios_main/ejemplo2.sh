#!/bin/bash

echo "Introduce tu edad: "
read edad
echo "La edad introducida es $edad"

read -n 4 -t 5 -s -p "Introduce la contraseña: " pass
echo ""
echo "La contraseña introducida es: $pass"

# variable=$(comando)
salida_ls=$(ls)
echo "La salida de ls es --> $salida_ls"