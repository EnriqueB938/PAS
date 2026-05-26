#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    FILE *fich = fopen("/etc/group", "r");
    if (fich == NULL)
    {
        printf("Error al abrir el fichero /etc/group.\n");
        exit(EXIT_FAILURE);
    }

    char lineaLeida[1024];
    while (fgets(lineaLeida, 1024, fich))
    {
        char *resto = lineaLeida;
        char *nombreGrupo = strtok_r(resto, ":", &resto);
        char *pass = strtok_r(resto, ":", &resto);
        char *GID = strtok_r(resto, ":", &resto);
        char *miembros = resto;
        printf("Leido el nombre de grupo %s, pass: %s, GID: %s, Miembros: %s\n", nombreGrupo, pass, GID, miembros);
    }

    fclose(fich);
}