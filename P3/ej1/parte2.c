/*
Implementar un programa que reciba por línea de argumentos las siguientes opciones:

    · -h/--help --> muestra por pantalla un mensaje de ayuda donde explica las opciones del programa
    · -p/--par --> recibe un número como argumento y muestra por pantalla si este es par o impar
    · -u/--usuario --> recibe el nombre de un usuario o el UID de un usuario y muestra su directorio home
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
#include <getopt.h> //Esto para las opciones
#include <pwd.h>    // Esto para los usuarios
#include <grp.h>    //Esto para los grupos
#include <sys/types.h>

int main(int argc, char **argv)
{
    /*
        CREACIÓN DE BANDERAS
    */
    bool hflag = false;
    char *pvalue = NULL;
    char *uvalue = NULL;
    char *gvalue = NULL;

    /*
        ACTIVACIÓN DE BANDERAS SEGÚN LAS OPCIÓN/ES RECIBIDAS
    */
    // Crear un vector de opciones
    static struct option long_options[] = {
        //  {<nombre largo>, <recibe/no recibe argumento>, NULL, <nombre corto>}
        {"help", no_argument, NULL, 'h'},
        {"par", required_argument, NULL, 'p'},
        {"usuario", required_argument, NULL, 'u'},
        {"grupo", required_argument, NULL, 'g'},
        /* Necesario para indicar el final de las opciones */
        {0, 0, 0, 0}};

    // Recorrer argv
    int c;
    while ((c = getopt_long(argc, argv, "hp:u:g:", long_options, NULL)) != -1)
    {
        switch (c)
        {
        case 'h':
            hflag = true;
            break;

        case 'p':
            pvalue = optarg;
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
    // if (hflag == true)
    // {
    //     uvalue = NULL;
    //     pvalue = NULL;
    //     gvalue = NULL;
    // }

    /*
        AQUÍ VA EL PROGRAMA (FUNCIONALIDAD)
    */
    if (hflag == true)
    {
        printf("************ AYUDA DEL PROGRAMA **************\n");
        printf("-h/--help --> para mostrar la ayuda\n");
        printf("-p/--par <número> --> para saber si dicho número es par o impar\n");
        printf("....\n");
        printf("***********************************************\n");
        exit(EXIT_SUCCESS);
    }

    if (pvalue != NULL)
    {
        printf("Opción -p/--par activada.\n");
        printf("Comprobando si %s es par o impar...\n", pvalue); // El nº por defecto es una cadena
        int numero = atoi(pvalue);                               // como el nº es una cadena, hay que pasarlo a entero
        if (numero % 2 == 0)
        {
            printf("%d es par\n", numero);
        }
        else
        {
            printf("%d es impar\n", numero);
        }
    }

    if (uvalue != NULL)
    {
        struct passwd *usuario = NULL; // como el fichero /etc/passwd/ tiene muchas cosas, entonce lo ponemos como struct
        char *endptr;
        long int UID = strtol(uvalue, &endptr, 10);
        if ((*uvalue) != '\0' && (*endptr) == '\0')
        {
            // Es un número
            usuario = getpwuid(UID);
        }
        else
        {
            // Es una cadena
            usuario = getpwnam(uvalue);
        }
        if (usuario == NULL)
        {
            // El usuario no existe
            char *nombreUsuarioActivo = getenv("USER");
            printf("Error, el usuario %s no existe. Mostrando información del usuario %s...\n", uvalue, nombreUsuarioActivo);
            usuario = getpwnam(nombreUsuarioActivo);
            printf("El home del usuario %s es %s.\n", nombreUsuarioActivo, usuario->pw_dir);
        }
        else
        {
            printf("El home del usuario %s es %s.\n", uvalue, usuario->pw_dir);
        }
    }

    if (gvalue != NULL)
    {
        char *endptr;
        long int GID = strtol(gvalue, &endptr, 10);
        if ((*gvalue) != '\0' && (*endptr) == '\0')
        {
            // Es un número
        }
        else
        {
            // Es una cadena
        }
    }
}