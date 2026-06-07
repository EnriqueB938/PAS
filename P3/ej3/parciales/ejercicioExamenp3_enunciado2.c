#include <errno.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>
#include <pwd.h>

#define COLA1 "/cola1"
#define COLA2 "/cola2"
#define MAX_SIZE 1024

void mostrarAyuda()
{
    printf("Ayudaaaaaaa\n");
}

int main(int argc, char **argv)
{
    char mensajeEnviado[MAX_SIZE], mensajeRecibido[MAX_SIZE];
    ssize_t bytes_read;

    // Creación de banderas
    bool hflag = false;
    char *svalue = NULL;

    // Vector de opciones cortas y largas
    static struct option long_options[] = {
        {"shell", required_argument, NULL, 's'},
        {"help", no_argument, NULL, 'h'},
        {0, 0, 0, 0}};

    // Activación de banderas
    int c;
    while ((c = getopt_long(argc, argv, "hs:", long_options, NULL)) != -1)
    {
        switch (c)
        {
        case 'h':
            hflag = true;
            break;

        case 's':
            svalue = optarg;
            break;

        case '?':
            if (optopt == 's')
            {
                printf("Error...\n");
                mostrarAyuda();
                exit(EXIT_FAILURE);
            }

        default:
            break;
        }
    }

    // Control de errores de opciones incompatibles
    // No hay

    if (hflag == true)
    {
        mostrarAyuda();
        exit(EXIT_SUCCESS);
    }

    // Nombre para la cola
    char nombreCola1[100];
    sprintf(nombreCola1, "%s-%s", COLA1, getenv("USER"));
    printf("[Padre]: El nombre de la cola 1 es: %s\n", nombreCola1);
    char nombreCola2[100];
    sprintf(nombreCola2, "%s-%s", COLA2, getenv("USER"));
    printf("[Padre]: El nombre de la cola 2 es: %s\n", nombreCola2);

    // Crear y conectarse a la/s cola/s
    mqd_t mq_cola1, mq_cola2;
    struct mq_attr attr;
    // Inicializar los atributos de la cola
    attr.mq_maxmsg = 10;        // Maximo número de mensajes
    attr.mq_msgsize = MAX_SIZE; // Maximo tamaño de un mensaje

    // Funcionalidad
    pid_t pid = fork();
    switch (pid)
    {
    case -1:
        fprintf(stderr, "Error en fork()\n");
        exit(-1);
    case 0:
        // Funcionalidad del hijo
        mq_cola1 = mq_open(nombreCola1, O_CREAT | O_RDONLY, 0644, &attr);
        if (mq_cola1 == (mqd_t)-1)
        {
            perror("Error al abrir la cola 1 en el servidor.");
            exit(-1);
        }
        printf("[Hijo]: El descriptor de la cola 1 es: %d\n", (int)mq_cola1);

        mq_cola2 = mq_open(nombreCola2, O_CREAT | O_WRONLY, 0644, &attr);
        if (mq_cola2 == (mqd_t)-1)
        {
            perror("Error al abrir la cola 2 en el servidor.");
            exit(-1);
        }
        printf("[Hijo]: El descriptor de la cola 2 es: %d\n", (int)mq_cola2);

        // Lee un mensaje del cliente de la cola 1
        bytes_read = mq_receive(mq_cola1, mensajeRecibido, MAX_SIZE, NULL);
        if (bytes_read < 0)
        {
            perror("Error al recibir el mensaje");
            exit(-1);
        }

        if (mensajeRecibido[strlen(mensajeRecibido) - 1] == '\n')
        {
            mensajeRecibido[strlen(mensajeRecibido) - 1] = '\0'; // Elimino el \n leído
        }

        struct passwd *usuario = getpwnam(mensajeRecibido);

        if (usuario == NULL)
        {
            usuario = getpwnam(getenv("USER"));
        }

        if (svalue != NULL)
        {
            if (strcmp(usuario->pw_shell, svalue) == 0)
            {
                strcpy(mensajeEnviado, usuario->pw_gecos);
            }
            else
            {
                strcpy(mensajeEnviado, "No coincidencia de la shell");
            }
        }
        else
        {
            if (strcmp(usuario->pw_shell, "/bin/sh") == 0)
            {
                strcpy(mensajeEnviado, usuario->pw_gecos);
            }
            else
            {
                strcpy(mensajeEnviado, "No coincidencia de la shell");
            }
        }

        if (mq_send(mq_cola2, mensajeEnviado, MAX_SIZE, 0) != 0)
        {
            perror("Error al enviar el mensaje en el servidor por la cola 2");
            exit(-1);
        }

        // Cerrar la cola del servidor
        if (mq_close(mq_cola1) == (mqd_t)-1)
        {
            perror("Error al cerrar la cola 1 en el servidor");
            exit(-1);
        }
        if (mq_close(mq_cola2) == (mqd_t)-1)
        {
            perror("Error al cerrar la cola 2 en el servidor");
            exit(-1);
        }

        exit(EXIT_SUCCESS);

    default:
        // Funcionalidad del padre
        mq_cola1 = mq_open(nombreCola1, O_CREAT | O_WRONLY, 0644, &attr);
        if (mq_cola1 == (mqd_t)-1)
        {
            perror("Error al abrir la cola 1 en el servidor.");
            exit(-1);
        }
        printf("[Padre]: El descriptor de la cola 1 es: %d\n", (int)mq_cola1);

        mq_cola2 = mq_open(nombreCola2, O_CREAT | O_RDONLY, 0644, &attr);
        if (mq_cola2 == (mqd_t)-1)
        {
            perror("Error al abrir la cola 2 en el servidor.");
            exit(-1);
        }
        printf("[Padre]: El descriptor de la cola 2 es: %d\n", (int)mq_cola2);

        // Padre escribe el usuario al hijo
        printf("[Padre]: Introduce el nombre de usuario que deseas buscar: \n");
        scanf("%s", mensajeEnviado);
        // Envía el mensaje al cliente por la cola 2 con el número de caracteres
        if (mq_send(mq_cola1, mensajeEnviado, MAX_SIZE, 0) != 0)
        {
            perror("Error al enviar el mensaje en el servidor por la cola 2");
            exit(-1);
        }

        // Padre lee la info que le manda el hijo
        // Lee un mensaje del cliente de la cola 1
        bytes_read = mq_receive(mq_cola2, mensajeRecibido, MAX_SIZE, NULL);
        if (bytes_read < 0)
        {
            perror("Error al recibir el mensaje");
            exit(-1);
        }

        if (mensajeRecibido[strlen(mensajeRecibido) - 1] == '\n')
        {
            mensajeRecibido[strlen(mensajeRecibido) - 1] = '\0'; // Elimino el \n leído
        }

        printf("[Padre]: Mensaje recibido por parte del hijo: %s\n", mensajeRecibido);

        pid_t flag;
        int status;
        while ((flag = wait(&status)) > 0)
        {
            if (WIFEXITED(status))
            {
                printf("Proceso Padre, Hijo con PID %ld finalizado, status = %d\n", (long int)flag, WEXITSTATUS(status));
            }
            else if (WIFSIGNALED(status))
            { // Para seniales como las de finalizar o matar
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

        // Cerrar la cola del servidor
        if (mq_close(mq_cola1) == (mqd_t)-1)
        {
            perror("Error al cerrar la cola 1 en el servidor");
            exit(-1);
        }
        if (mq_close(mq_cola2) == (mqd_t)-1)
        {
            perror("Error al cerrar la cola 2 en el servidor");
            exit(-1);
        }

        // Eliminar la cola del servidor
        if (mq_unlink(nombreCola1) == (mqd_t)-1)
        {
            perror("Error al eliminar la cola 1 del servidor");
            exit(-1);
        }
        if (mq_unlink(nombreCola2) == (mqd_t)-1)
        {
            perror("Error al eliminar la cola 2 del servidor");
            exit(-1);
        }

        exit(EXIT_SUCCESS);
    }

    exit(EXIT_SUCCESS);
}