#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>
#include <getopt.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_SIZE 500

void mostrarAyuda()
{
    printf("Esto es la ayuda.\n");
}

int main(int argc, char **argv)
{
    char *ivalue = NULL;
    char mensajeEnviado[MAX_SIZE];
    char mensajeRecibido[MAX_SIZE];
    int bytes_leidos;
    struct passwd *usuario = NULL;

    static struct option long_options[] = {
        {"userinfo", required_argument, NULL, 'i'},
        {0, 0, 0, 0}};

    int c;
    while ((c = getopt_long(argc, argv, "i:", long_options, NULL)) != -1)
    {
        switch (c)
        {
        case 'i':
            ivalue = optarg;
            break;

        case '?':
            if (optopt == 'i')
            {
                mostrarAyuda();
                exit(EXIT_FAILURE);
            }

        default:
            printf("Opción no reconocida.\n");
            mostrarAyuda();
            abort();
            break;
        }
    }

    int tuberia[2];
    if (pipe(tuberia) != 0)
    {
        printf("Error en pipe()\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    switch (pid)
    {
    case -1:
        printf("Error en fork()\n");
        exit(EXIT_FAILURE);

    case 0:
        // Hijo
        if (close(tuberia[1]) != 0)
        {
            printf("Error al cerrar tuberia[1] en el hijo.\n");
            exit(EXIT_FAILURE);
        }

        printf("[HIJO]: cerrado el extremo de escritura de la tubería.\n");

        bytes_leidos = read(tuberia[0], mensajeRecibido, MAX_SIZE);

        if (bytes_leidos == -1)
        {
            printf("\n[HIJO]: ERROR al leer de la tubería...\n");
            exit(EXIT_FAILURE);
        }
        else if (bytes_leidos == 0)
        {
            printf("[HIJO]: Detecto que mi padre ha cerrado la tuberia...\n");
        }
        else
        {
            printf("[HIJO]: recibido el mensaje: %s\n", mensajeRecibido);
            usuario = getpwnam(mensajeRecibido);
            if (usuario == NULL)
            {
                printf("[HIJO]: usuario %s no encontrado, se mostrá la información del usuario %s\n", mensajeRecibido, getenv("USER"));
                usuario = getpwnam(getenv("USER"));
            }
            printf("[HIJO]: Usuario: %s, UID: %d, GID: %d, Home: %s\n", usuario->pw_name, usuario->pw_uid, usuario->pw_gid, usuario->pw_dir);
        }

        if (close(tuberia[0]) != 0)
        {
            printf("Error al cerrar tuberia[1] en el hijo.\n");
            exit(EXIT_FAILURE);
        }

        printf("[HIJO]: cerrado el extremo de lectura de la tubería.\n");

        exit(EXIT_SUCCESS);

    default:
        // Padre
        if (close(tuberia[0]) != 0)
        {
            printf("Error al cerrar tuberia[0] en el padre.\n");
            exit(EXIT_FAILURE);
        }

        printf("[PADRE]: cerrado el extremo de lectura de la tubería.\n");

        if (ivalue == NULL)
        {
            strcpy(mensajeEnviado, getenv("USER"));
        }
        else
        {
            strcpy(mensajeEnviado, ivalue);
        }

        if (write(tuberia[1], mensajeEnviado, strlen(mensajeEnviado)) != strlen(mensajeEnviado))
        {
            printf("Error al escribir desde el padre en la tubería.\n");
            exit(EXIT_FAILURE);
        }

        printf("[PADRE]: escrito en la tubería el mensaje: %s\n", mensajeEnviado);

        if (close(tuberia[1]) != 0)
        {
            printf("Error al cerrar tuberia[1] en el padre.\n");
            exit(EXIT_FAILURE);
        }

        printf("[PADRE]: cerrado el extremo de escritura de la tubería.\n");

        break;
    }

    // Espera del proceso padre el hijo
    pid_t flag;
    int status;
    while ((flag = wait(&status)) > 0)
    {
        if (WIFEXITED(status))
        {
            printf("Proceso Padre, Hijo con PID %ld finalizado, status = %d\n", (long int)flag, WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            printf("Proceso Padre, Hijo con PID %ld finalizado al recibir la señal %d\n", (long int)flag, WTERMSIG(status));
        }
    }
    if (flag == (pid_t)-1 && errno == ECHILD)
    {
        printf("Proceso Padre %d, no hay mas hijos que esperar. Valor de errno = %d, definido como: %s\n", getpid(), errno, strerror(errno));
    }
    else
    {
        printf("Error en la invocacion de wait o waitpid. Valor de errno = %d, definido como: %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}