// gcc -lrt cliente.c -o cliente

#include "common.h"
#include <errno.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>

mqd_t mq_cola1, mq_cola2;

void funcionManejadora(int signal)
{
    printf("[Cliente]: recibida la señal de finalización %d\n", signal);

    if (mq_send(mq_cola1, MSG_STOP, MAX_SIZE, 0) != 0)
    {
        perror("Error al enviar el mensaje en el cliente por la cola 1");
        exit(-1);
    }

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
}

int main(int argc, char **argv)
{
    signal(SIGINT, &funcionManejadora);
    signal(SIGTERM, &funcionManejadora);

    char nombreCola1[100];
    sprintf(nombreCola1, "%s-%s", COLA1, getenv("USER"));
    printf("[Cliente]: El nombre de la cola 1 es: %s\n", nombreCola1);
    char nombreCola2[100];
    sprintf(nombreCola2, "%s-%s", COLA2, getenv("USER"));
    printf("[Cliente]: El nombre de la cola 2 es: %s\n", nombreCola2);

    mq_cola1 = mq_open(nombreCola1, O_WRONLY);
    if (mq_cola1 == (mqd_t)-1)
    {
        perror("Error al abrir la cola 1 en el Cliente.");
        exit(-1);
    }
    printf("[Cliente]: El descriptor de la cola 1 es: %d\n", (int)mq_cola1);

    mq_cola2 = mq_open(nombreCola2, O_RDONLY);
    if (mq_cola2 == (mqd_t)-1)
    {
        perror("Error al abrir la cola 2 en el Cliente.");
        exit(-1);
    }
    printf("[Cliente]: El descriptor de la cola 2 es: %d\n", (int)mq_cola2);

    char mensajeEnviado[MAX_SIZE], mensajeRecibido[MAX_SIZE];
    int must_stop = 0;
    do
    {
        printf("[Cliente]: introduce un mensaje: ");
        fgets(mensajeEnviado, MAX_SIZE, stdin);

        if (mq_send(mq_cola1, mensajeEnviado, MAX_SIZE, 0) != 0)
        {
            perror("Error al enviar el mensaje en el cliente por la cola 1");
            exit(-1);
        }

        if (strncmp(mensajeEnviado, MSG_STOP, strlen(MSG_STOP)) == 0)
        {
            must_stop = 1;
        }

        if (must_stop == 0)
        {
            ssize_t bytes_read;
            bytes_read = mq_receive(mq_cola2, mensajeRecibido, MAX_SIZE, NULL);
            if (bytes_read < 0)
            {
                perror("Error al recibir el mensaje");
                exit(-1);
            }
            printf("[Cliente]: Mensaje recibido --> %s\n", mensajeRecibido);
        }

    } while (must_stop == 0);

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

    return 0;
}