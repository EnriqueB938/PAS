/*
    El proceso padre genera dos números aleatorios flotantes y envía al hijo la suma de estos.
    El proceso hijo lee la suma recibida y la imprime por pantalla.
*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <mqueue.h>
#include <getopt.h>
#include <stdbool.h>
#include <sys/types.h>
#include <pwd.h>

#define MAX_SIZE 1024
#define COLA1 "/cola1"

int main(int argc, char **argv)
{
    srand(time(NULL));

    // Crear y conectarse a la/s cola/s
    mqd_t mq_cola;
    struct mq_attr attr;
    // Inicializar los atributos de la cola
    attr.mq_maxmsg = 10;        // Maximo número de mensajes
    attr.mq_msgsize = MAX_SIZE; // Maximo tamaño de un mensaje

    char mensajeEnviadoHijo[MAX_SIZE];
    char mensajeRecibidoPadre[MAX_SIZE];
    int parar = 0;
    char *uvalue = NULL;
    char *pvalue = NULL;
    bool hflag = false;

    char nombreCola[100];
    sprintf(nombreCola, "%s-%s", COLA1, getenv("USER"));
    printf("[Servidor]: El nombre de la cola 1 es: %s\n", nombreCola);

    // Opciones por línea de argumentos
    int c;

    // OPCIONES LARGAS
    static struct option long_options[] = {
        //  {<nombre largo>, <recibe/no recibe argumento>, NULL, <nombre corto>}
        {"help", no_argument, NULL, 'h'},
        {"psalida", required_argument, NULL, 'p'},
        /* Necesario para indicar el final de las opciones */
        {0, 0, 0, 0}};

    while ((c = getopt_long(argc, argv, "hp:u:", long_options, NULL)) != -1)
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

        case '?':
            if (optopt == 'p')
            {
                printf("Error, -p/--psalida requiere de un argumento.\n");
            }
            if (optopt == 'u')
            {
                printf("Error, -u requiere de un argumento.\n");
            }
            exit(EXIT_FAILURE);
            // abort();

        default:
            printf("Opción no reconocida.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Configuro el mensaje de salida
    char mensajeSalida[100];
    if (pvalue == NULL)
    {
        strcpy(mensajeSalida, "quit");
    }
    else
    {
        strcpy(mensajeSalida, pvalue);
    }

    // Creo el proceso hijo
    pid_t pid = fork();
    switch (pid)
    {
    case -1:
        /* ERROR */
        perror("Error en fork()\n");
        exit(EXIT_FAILURE);
        break;

    case 0:
        /* HIJO */
        mq_cola = mq_open(nombreCola, O_CREAT | O_WRONLY, 0644, &attr);
        if (mq_cola == (mqd_t)-1)
        {
            perror("Error al abrir la cola 1 en el servidor.");
            exit(-1);
        }
        printf("[Hijo]: El descriptor de la cola es: %d\n", (int)mq_cola);

        do
        {
            printf("[Hijo]: introduce el nombre de un usuario: ");
            scanf("%s", mensajeEnviadoHijo);

            if (mq_send(mq_cola, mensajeEnviadoHijo, MAX_SIZE, 0) != 0)
            {
                perror("Error al enviar el mensaje en el servidor por la cola 2");
                exit(-1);
            }

            if (strcmp(mensajeEnviadoHijo, mensajeSalida) == 0)
            {
                parar = 1;
            }

        } while (parar == 0);

        if (mq_close(mq_cola) == (mqd_t)-1)
        {
            perror("Error al cerrar la cola 1 en el servidor");
            exit(-1);
        }

        // Finaliza el proceso hijo
        exit(EXIT_SUCCESS);
        break;

    default:
        /* PADRE */
        mq_cola = mq_open(nombreCola, O_CREAT | O_RDONLY, 0644, &attr);
        if (mq_cola == (mqd_t)-1)
        {
            perror("Error al abrir la cola 1 en el servidor.");
            exit(-1);
        }
        printf("[Hijo]: El descriptor de la cola es: %d\n", (int)mq_cola);

        do
        {
            ssize_t bytes_read;
            bytes_read = mq_receive(mq_cola, mensajeRecibidoPadre, MAX_SIZE, NULL);
            if (bytes_read < 0)
            {
                perror("Error al recibir el mensaje");
                exit(-1);
            }

            if (mensajeRecibidoPadre[strlen(mensajeRecibidoPadre) - 1] == '\n')
            {
                mensajeRecibidoPadre[strlen(mensajeRecibidoPadre) - 1] = '\0'; // Elimino el \n leído
            }

            if (strcmp(mensajeRecibidoPadre, mensajeSalida) != 0)
            {
                struct passwd *usuario = getpwnam(mensajeRecibidoPadre);

                if (usuario == NULL)
                {
                    usuario = getpwnam(getenv("USER"));
                }

                if (uvalue == NULL)
                {
                    printf("Muestro toda la info de %s...\n", usuario->pw_name);
                }
                else
                {
                    if (usuario->pw_uid < atoi(uvalue))
                    {
                        printf("Muestro toda la info de %s...\n", usuario->pw_name);
                    }
                    else
                    {
                        printf("Muestro toda solo lo que pidan de %s...\n", usuario->pw_name);
                    }
                }
            }
            else
            {
                parar = 1;
            }
        } while (parar == 0);

        if (mq_close(mq_cola) == (mqd_t)-1)
        {
            perror("Error al cerrar la cola 1 en el servidor");
            exit(-1);
        }

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

    if (mq_unlink(nombreCola) == (mqd_t)-1)
    {
        perror("Error al eliminar la cola 1 del servidor");
        exit(-1);
    }

    exit(EXIT_SUCCESS);
}