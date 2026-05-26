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
    bool aflag = false;
    bool mflag = false;
    bool sflag = false;
    char *uvalue = NULL;
    char *gvalue = NULL;

    /*
        ACTIVACIÓN DE BANDERAS SEGÚN LAS OPCIÓN/ES RECIBIDAS
    */
    // Crear un vector de opciones
    static struct option long_options[] = {
        //  {<nombre largo>, <recibe/no recibe argumento>, NULL, <nombre corto>}
        {"help", no_argument, NULL, 'h'},
        {"active", no_argument, NULL, 'a'},
        {"maingroup", no_argument, NULL, 'm'},
        {"allgroups", no_argument, NULL, 's'},
        {"user", required_argument, NULL, 'u'},
        {"group", required_argument, NULL, 'g'},
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

        case 'a':
            aflag = true;
            break;

        case 'm':
            mflag = true;
            break;

        case 's':
            sflag = true;
            break;
        case 'u':
            uvalue = optarg;
            break;
        case 'g':
            gvalue = optarg;
            break;

        case '?':
            // Aquí entro si he pasado una opción con argumentos sin argumentos.
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

    /*
        -m solo aparece con --user o --active → incompatible con -g y -s
        -u y -a nunca aparecen juntos → incompatibles entre sí
        -u y -g nunca aparecen juntos → incompatibles entre sí
        -s aparece siempre solo → incompatible con todo lo demás
        -h se ignora cualquier combinación → no hay que validar nada
    */
    if (uvalue != NULL && gvalue != NULL)
    {
        printf("Error, las opcines -u/--usuario y -g/--grupo son incompatibles.\n");
        exit(EXIT_FAILURE);
    }

    if (uvalue != NULL && aflag)
    {
        printf("Error, las opcines -u/--usuario y -a/--active son incompatibles.\n");
        exit(EXIT_FAILURE);
    }

    if (mflag && gvalue != NULL)
    {
        printf("Error, las opcines --maingroup, solo se puede acompañar a  --user o --active.\n");
        exit(EXIT_FAILURE);
    }

    if (mflag && sflag)
    {
        printf("Error: --maingroup solo puede acompañar a --user o --active.\n");
        exit(EXIT_FAILURE);
    }

    if (sflag && (uvalue || gvalue || aflag))
    {
        printf("Error: --allgroups es incompatible con el resto de opciones.\n");
        exit(EXIT_FAILURE);
    }

    /* Sin opciones → equivale a -a -m */

    if (!hflag && !aflag && !mflag && !sflag && uvalue == NULL && gvalue == NULL)
    {
        aflag = true;
        mflag = true;
    }

    //    AQUÍ VA EL PROGRAMA(FUNCIONALIDAD)
    if (hflag == true)
    {
        printf("************ AYUDA DEL PROGRAMA **************\n");
        printf("-h/--help --> para mostrar la ayuda\n");
        printf("-u/--user (<nombre>|<uid>) --> Información sobre el usuario\n");
        printf("-a/--active  --> Información sobre el usuario actual\n");
        printf("-m/--maingroup  --> Además de info de usuario, imprimir la info de su grupo principal\n");
        printf("-g/--group (<nombre>|<gid>) --> Además de info de usuario, imprimir la info de su grupo principal\n");
        printf("-s/--allgroups (<nombre>|<gid>) --> Además de info de usuario, imprimir la info de su grupo principal\n");
        printf("....\n");
        printf("***********************************************\n");
        exit(EXIT_SUCCESS);
    }

    if (uvalue != NULL)
    {
        struct passwd *usuario = NULL;
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
            exit(EXIT_FAILURE);
        }

        printf("Usuario:\n");
        printf("  Nombre:                    %s\n", usuario->pw_gecos);
        printf("  Login:                     %s\n", usuario->pw_name);
        printf("  Password:                  %s\n", usuario->pw_passwd);
        printf("  UID:                       %d\n", usuario->pw_uid);
        printf("  Home:                      %s\n", usuario->pw_dir);
        printf("  Shell:                     %s\n", usuario->pw_shell);
        printf("  Número de grupo principal: %d\n", usuario->pw_gid);

        if (mflag == true)
        {
        }

        if (gvalue != NULL)
        {
            struct group *grupo = NULL;
            char *endptr;
            long int GID = strtol(gvalue, &endptr, 10);
            if ((*gvalue) != '\0' && (*endptr) == '\0')
            {
                // Es un número
                grupo = getgrgid(GID);
            }
            else
            {
                // Es una cadena
                grupo = getgrnam(gvalue);
            }
            if (grupo == NULL)
            {
                char *nombreUsuarioActual = getenv("USER");
                struct passwd *usuarioActual = getpwnam(nombreUsuarioActual);
                struct group *grupoActual = getgrgid(usuarioActual->pw_gid);
                printf("Mostrando los miembros del grupo %d:\n", usuarioActual->pw_gid);
                for (int i = 0; grupoActual->gr_mem[i] != NULL; i++)
                {
                    printf("%s\n", grupoActual->gr_mem[i]);
                }
            }
            else
            {
                printf("Mostrando los miembros del grupo %s:\n", gvalue);
                for (int i = 0; grupo->gr_mem[i] != NULL; i++)
                {
                    printf("%s\n", grupo->gr_mem[i]);
                }
            }
        }
    }
}