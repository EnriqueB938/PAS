/*
    El proceso padre genera dos números aleatorios flotantes y envía al hijo una cadena siguiendo este formato:
        "X;Y;Z"
        siendo X el número 1 generado, Y el número 2 generado y Z la suma.
    El proceso hijo lee la suma recibida y la imprime por pantalla.
*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define MAX_SIZE 1024

int main(int argc, char **argv)
{
    int fildes[2]; // La tubería
    float numero1Escrito = 0, numero2Escrito = 0, sumaEscrita = 0;
    float numero1Leido = 0, numero2Leido = 0, sumaLeida = 0;
    char cadenaEscrita[MAX_SIZE];
    char cadenaLeida[MAX_SIZE];
    srand(time(NULL));
    int bytesLeidos;

    // Creamos y conectamos a la tubería
    int status = pipe(fildes);
    if (status == -1)
    {
        perror("Error en pipe");
        exit(EXIT_FAILURE);
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
        // Como el hijo no necesita escribir, cierra el extremo de escritura
        if (close(fildes[1]) == -1)
        {
            perror("[HIJO]: Error en close(fildes[1])");
            exit(EXIT_FAILURE);
        }

        // El hijo lee de la tubería el número escrito por el proceso padre
        bytesLeidos = read(fildes[0], cadenaLeida, MAX_SIZE);
        if (bytesLeidos == -1)
        {
            printf("\n[HIJO]: ERROR al leer de la tubería...\n");
            exit(EXIT_FAILURE);
        }
        else if (bytesLeidos == 0)
        {
            printf("[HIJO]: Detecto que mi padre ha cerrado la tuberia...\n");
        }
        else
        {
            sscanf(cadenaLeida, "%f;%f;%f", &numero1Leido, &numero2Leido, &sumaLeida);
            printf("[HIJO]: se ha leído de la tubería un %.2f, un %.2f y un %.2f\n", numero1Leido, numero2Leido, sumaLeida);
        }
        // Como el hijo cierra el extremo de lectura
        if (close(fildes[0]) == -1)
        {
            perror("[HIJO]: Error en close(fildes[1])");
            exit(EXIT_FAILURE);
        }

        // Finaliza el proceso hijo
        exit(EXIT_SUCCESS);
        break;

    default:
        /* PADRE */

        // Genera dos números aleatorios flotantes
        numero1Escrito = ((float)rand() / RAND_MAX) * 10;
        numero2Escrito = ((float)rand() / RAND_MAX) * 10;
        sumaEscrita = numero1Escrito + numero2Escrito;
        printf("[PADRE]: la suma de %.2f y %.2f es %.2f.\n", numero1Escrito, numero2Escrito, sumaEscrita);

        // Guardamos dentro de la cadena, lo que se va a enviar al otro proceso
        sprintf(cadenaEscrita, "%f;%f;%f", numero1Escrito, numero2Escrito, sumaEscrita);

        // Como el padre no necesita leer, cierra el extremo de lectura
        if (close(fildes[0]) == -1)
        {
            perror("[PADRE]: Error en close");
            exit(EXIT_FAILURE);
        }

        // El padre escribe en la tubería el número que enviará al hijo
        status = write(fildes[1], cadenaEscrita, strlen(cadenaEscrita));
        if (status != strlen(cadenaEscrita))
        {
            printf("\n[PADRE]: ERROR al escribir en la tubería...\n");
            exit(EXIT_FAILURE);
        }

        // El padre cierra el extremo de escritura ya que ha terminado de escribir
        if (close(fildes[1]) == -1)
        {
            perror("[PADRE]: Error en close");
            exit(EXIT_FAILURE);
        }

        break;
    }

    // Espera del proceso padre el hijo
    pid_t flag;
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