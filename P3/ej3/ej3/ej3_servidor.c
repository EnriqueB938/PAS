#include "common.h"
#include <errno.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

void funcionLog(char *mensaje)
{
    int resultado;
    char nombreFichero[100];
    char mensajeAEscribir[300];
    time_t t;
    FILE *fLog;

    sprintf(nombreFichero, "log-servidor.txt");
    if (fLog == NULL)
    {
        fLog = fopen(nombreFichero, "at");
        if (fLog == NULL)
        {
            perror("Error abriendo el fichero de log");
            exit(1);
        }
    }

    t = time(NULL);
    struct tm *p = localtime(&t);
    strftime(mensajeAEscribir, 1000, "[%Y-%m-%d, %H:%M:%S]", p);

    sprintf(mensajeAEscribir, "%s ==> %s\n", mensajeAEscribir, mensaje);

    resultado = fputs(mensajeAEscribir, fLog);
    if (resultado < 0)
        perror("Error escribiendo en el fichero de log");

    fclose(fLog);
    fLog = NULL;
}

int main(int argc, char **argv)
{

    char mensajeLog[MAX_SIZE];
    char nombreCola1[100];
    sprintf(nombreCola1, "%s-%s", COLA1, getenv("USER"));
    printf("[Servidor]: El nombre de la cola 1 es: %s\n", nombreCola1);
    sprintf(mensajeLog, "[Servidor]: El nombre de la cola 1 es: %s\n", nombreCola1);
    funcionLog(mensajeLog);
    char nombreCola2[100];
    sprintf(nombreCola2, "%s-%s", COLA2, getenv("USER"));
    printf("[Servidor]: El nombre de la cola 2 es: %s\n", nombreCola2);
    sprintf(mensajeLog, "[Servidor]: El nombre de la cola 2 es: %s\n", nombreCola2);
    funcionLog(mensajeLog);

    mqd_t mq_cola1, mq_cola2;
    struct mq_attr attr;
    attr.mq_maxmsg = 10;        
    attr.mq_msgsize = MAX_SIZE; 

    mq_cola1 = mq_open(nombreCola1, O_CREAT | O_RDONLY, 0644, &attr);
    if (mq_cola1 == (mqd_t)-1)
    {
        perror("Error al abrir la cola 1 en el servidor.");
        funcionLog("Error al abrir la cola 1 en el servidor.");
        exit(-1);
    }
    printf("[Servidor]: El descriptor de la cola 1 es: %d\n", (int)mq_cola1);

    mq_cola2 = mq_open(nombreCola2, O_CREAT | O_WRONLY, 0644, &attr);
    if (mq_cola2 == (mqd_t)-1)
    {
        perror("Error al abrir la cola 2 en el servidor.");
        exit(-1);
    }
    printf("[Servidor]: El descriptor de la cola 2 es: %d\n", (int)mq_cola2);

    char mensajeRecibido[MAX_SIZE], mensajeEnviado[MAX_SIZE];
    int must_stop = 0;
    do
    {
        ssize_t bytes_read;
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

        if (strcmp(mensajeRecibido, MSG_STOP) == 0)
        {
            must_stop = 1;
        }

        if (must_stop == 0)
        {
            printf("[Servidor]: recibido el mensaje: %s\n", mensajeRecibido);
            sprintf(mensajeLog, "[Servidor]: recibido el mensaje: %s\n", mensajeRecibido);
            funcionLog(mensajeLog);
            int cont = strlen(mensajeRecibido);
            sprintf(mensajeEnviado, "Número de caracteres recibidos: %d", cont);

            if (mq_send(mq_cola2, mensajeEnviado, MAX_SIZE, 0) != 0)
            {
                perror("Error al enviar el mensaje en el servidor por la cola 2");
                exit(-1);
            }
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
}