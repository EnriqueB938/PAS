

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int fildes[2];
    float numero1 = 0, numero2 = 0, sumaEscrita = 0, sumaLeida = 0;
    srand(time(NULL));

    int status = pipe(fildes);
    if (status == -1)
    {
        perror("Error en pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    switch (pid)
    {
    case -1:
        perror("Error en fork()\n");
        exit(EXIT_FAILURE);
        break;

    case 0:
        if (close(fildes[1]) == -1)
        {
            perror("[HIJO]: Error en close(fildes[1])");
            exit(EXIT_FAILURE);
        }

        status = read(fildes[0], &sumaLeida, sizeof(float));
        if (status != sizeof(float))
        {
            printf("\n[HIJO]: ERROR al leer de la tubería...\n");
            exit(EXIT_FAILURE);
        }
        printf("[HIJO]: se ha leído de la tubería el valor de suma: %.2f\n", sumaLeida);

        if (close(fildes[0]) == -1)
        {
            perror("[HIJO]: Error en close(fildes[1])");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_SUCCESS);
        break;

    default:

        numero1 = ((float)rand() / RAND_MAX) * 10;
        numero2 = ((float)rand() / RAND_MAX) * 10;
        sumaEscrita = numero1 + numero2;
        printf("[PADRE]: la suma de %.2f y %.2f es %.2f.\n", numero1, numero2, sumaEscrita);

        if (close(fildes[0]) == -1)
        {
            perror("[PADRE]: Error en close");
            exit(EXIT_FAILURE);
        }

        status = write(fildes[1], &sumaEscrita, sizeof(float));
        if (status != sizeof(float))
        {
            printf("\n[PADRE]: ERROR al escribir en la tubería...\n");
            exit(EXIT_FAILURE);
        }

        if (close(fildes[1]) == -1)
        {
            perror("[PADRE]: Error en close");
            exit(EXIT_FAILURE);
        }

        break;
    }

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