/*
Implementar un programa que reciba por línea de argumentos las siguientes opciones:

    · -h/--help --> muestra por pantalla un mensaje de ayuda donde explica las opciones del programa
    · -p/--par --> recibe un número como argumento y muestra por pantalla si este es par o impar
    · -u/--usuario --> recibe el nombre de un usuario y muestra su directorio home
    · -g/--grupo --> recibe el nombre de un grupo o GID del grupo y muestra los usuarios de dicho grupo

Si activas la opción h el resto de opciones se ignoran. La opción u y g son incompatibles, es decir, no se pueden
activar a la vez. Si indicas un nombre de usuario no válido, se indicará por pantalla y se mostrará la información
del usuario activo. Si se indica un nombre de grupo no válido, se indicará por pantalla y se mostrar la información del grupo
del usuario activo.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>

int main(int argc, char **argv)
{
    /*
        CREACIÓN DE BANDERAS
    */
    bool hflag = false;  // false si no se reciben por argumentos
    char *pvalue = NULL; // Se ponen a NULL si puede recibir por argumentos
    char *uvalue = NULL;
    char *gvalue = NULL;

    /*
        ACTIVACIÓN DE BANDERAS SEGÚN LAS OPCIÓN/ES RECIBIDAS
    */
    // Crear un vector de opciones
    static struct option long_options[] = {
        //  {<nombre largo>, <recibe/no recibe argumento>, NULL, <nombre corto>}
        {"help", no_argument, NULL, 'h'},      // no_argument
        {"par", required_argument, NULL, 'p'}, // required_argument
        {"usuario", required_argument, NULL, 'u'},
        {"grupo", required_argument, NULL, 'g'},
        /* Necesario para indicar el final de las opciones */
        {0, 0, 0, 0}};

    // Recorrer argv
    int c;
    while ((c == getopt_long(argc, argv, "hp:u:g:", long_options, NULL)) != -1) // se ponen todas las opciones: hpug, y luego se le pone ":" detrás, a las que reciben argumentos
    {
        switch (c)
        {
        case 'h':
            hflag = true;
            break;

        case 'p':
            pvalue = optarg; // optarg te devuelve la dir. de memoria de ese argumento
            break;

        case 'u':
            uvalue = optarg;
            break;

        case 'g':
            gvalue = optarg;
            break;

        case '?':
            // Aquí entro si he pasado una opción con argumentos sin argumentos.
            if (optopt == 'p')
            {
                printf("Error, la opción -p/--par requiere de argumento, ejemplo: ./%s -p <número>\n", argv[0]);
            }
            if (optopt == 'u')
            {
                printf("Error, la opción -u/--usuario requiere de argumento, ejemplo: ./%s -u <nombre de usuario>\n", argv[0]);
            }
            if (optopt == 'g')
            {
                printf("Error, la opción -g/--grupo requiere de argumento, ejemplo: ./%s -g <nombre de grupo>\n", argv[0]);
            }
            break;

        default:
            abort();
        }
    }

    /*
        CONTROL DE ERRORES DE LAS OPCIONES
    */
    if (uvalue != NULL && gvalue != NULL)
    {
        printf("Error, las opcines -u/--usuario y -g/--grupo son incompatibles.\n");
        exit(EXIT_FAILURE);
    }

    /*
        AQUÍ VA EL PROGRAMA (FUNCIONALIDAD)
    */
}