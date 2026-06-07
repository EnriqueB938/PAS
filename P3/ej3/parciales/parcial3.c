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
#include <grp.h>

#define COLA1 "/cola1"
#define MAX_SIZE 1024


void mostrarAyuda()
{
    printf("\nUso del programa:\n");
    printf("  --psalida o -p <palabra> : Establece la palabra de salida (por defecto 'exit').\n");
    printf("  -g <numero>              : Filtro de GID. Solo muestra info completa si GID del grupo > numero.\n");
    printf("  --help o -h              : Muestra esta ayuda.\n\n");
}

int main(int argc, char **argv)
{

    char mensajeEnviado[MAX_SIZE], mensajeRecibido[MAX_SIZE];
    ssize_t bytes_read;

    bool hflag =false;
    bool gflag = false;
    char pvalue [MAX_SIZE] = "exit";
    int gvalue = 0;


    static struct option long_options[] = {
        {"help", no_argument, NULL, 'h'},
        {"psalida", required_argument, NULL, 'p'},
        {"gid", required_argument, NULL, 'g'},
        {0,0,0,0}};


    int c;
    while ((c = getopt_long(argc, argv, "hg:g:", long_options, NULL)) != -1)
    {        switch (c)
        {        
        case 'h':
            hflag = true;
            break;
        case 'g':
            gvalue = atoi(optarg);
            if(gvalue < 0)
            {
                printf("Error: El valor de GID debe ser un número entero no negativo.\n");
                mostrarAyuda();
                exit(EXIT_FAILURE);
            }
            break;
        case 'p':
            strncpy(pvalue, optarg, MAX_SIZE - 1);
            pvalue[MAX_SIZE -1] = '\0';
            break;
        case '?':
                printf("Error...\n");
                mostrarAyuda();
                exit(EXIT_FAILURE);
            

        default:
            break; 
        }
    }


    if (hflag == true)
    {
        mostrarAyuda();
        exit(EXIT_SUCCESS);
    }

    char nombreCola[200];
    sprintf(nombreCola, "%s-%s", COLA1, getenv("USER"));

    mqd_t mq_cola;
    struct mq_attr attr;

    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;

    pid_t pid = fork();
    switch (pid)
    {
        case -1:
            perror("Error al crear el proceso hijo");
            exit(EXIT_FAILURE); 
        
        case 0:

        mq_cola = mq_open(nombreCola, O_CREAT | O_WRONLY, 0644, &attr);

        if (mq_cola == (mqd_t)-1)
        {
            perror("Error al abrir la cola en el hijo");
            exit(EXIT_FAILURE);
        }

        printf("[Hijo]: El descriptor de la cola es: %d\n", (int)mq_cola);

        do
        {
            printf("[Hijo]: Introduce el nombre del grupo a buscar (o '%s' para salir): ", pvalue);
            scanf("%s", mensajeEnviado);

            //Cola coge el mensaje y lo enviaal padre

            if (mq_send(mq_cola, mensajeEnviado, strlen(mensajeEnviado) + 1, 0) != 0)
            {
                perror("[Hijo] Error al enviar el mensaje");
                break;
            }

            //Comprobamso que el mensaje enviado es igual a lo que nos dan por linea de argumentos

            if (strcmp(mensajeEnviado, pvalue) == 0)
            {
                printf("[Hijo]: Palabra de salida detectada. Finalizando bucle.\n");
                break;
            }

        } while (true);


        if (mq_close(mq_cola) == (mqd_t)-1)
        {
            perror("[Hijo] Error al cerrar la cola");
            exit(-1);
        }

        exit(EXIT_SUCCESS);

        default:

        mq_cola = mq_open(nombreCola, O_CREAT | O_RDONLY, 0644, &attr);

        if (mq_cola == (mqd_t)-1)
        {
            perror("[Padre] Error al abrir la cola para lectura.");
            exit(-1);
        }
        printf("[Padre]: El descriptor de la cola es: %d\n", (int)mq_cola);

        do
        {
            // Lee un mensaje del hijo
            bytes_read = mq_receive(mq_cola, mensajeRecibido, MAX_SIZE, NULL);
            if (bytes_read < 0)
            {
                perror("[Padre] Error al recibir el mensaje");
                break;
            }

            // Comprobar palabra de salida
            if (strcmp(mensajeRecibido, pvalue) == 0)
            {
                printf("[Padre]: Palabra de salida recibida. Finalizando bucle.\n");
                break;
            }

            printf("\n[Padre]: Buscando información del grupo '%s'...\n", mensajeRecibido);

            struct group *info_grupo = getgrnam(mensajeRecibido);

            if (info_grupo == NULL)
            {
                printf("[Padre]: El grupo no existe. Seleccionando grupo del usuario actual...\n");
                struct passwd *info_usuario = getpwuid(getuid());
                info_grupo = getgrgid(info_usuario->pw_gid);
            }

            // Mostrar la información dependiendo del argumento -g
            if (!gflag || (gflag && info_grupo->gr_gid > (gid_t)gvalue))
            {
                printf("[Padre]: --- INFORMACIÓN COMPLETA ---\n");
                printf("Nombre del grupo: %s\n", info_grupo->gr_name);
                printf("GID del grupo:    %u\n", info_grupo->gr_gid);
                printf("Miembros:         ");
                
                if (info_grupo->gr_mem[0] == NULL) {
                    printf("(Sin miembros listados)\n");
                } else {
                    for (int i = 0; info_grupo->gr_mem[i] != NULL; i++) {
                        printf("%s ", info_grupo->gr_mem[i]);
                    }
                    printf("\n");
                }
                printf("-----------------------------------\n\n");
            }
            else
            {
                printf("[Padre]: --- INFORMACIÓN REDUCIDA ---\n");
                printf("Nombre del grupo: %s\n", info_grupo->gr_name);
                printf("-----------------------------------\n\n");
            }

        } while (true);


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

    if (mq_close(mq_cola) == (mqd_t)-1)
        {
            perror("Error al cerrar la cola en el servidor");
            exit(-1);
        }

        // Eliminar la cola del servidor
        if (mq_unlink(nombreCola) == (mqd_t)-1)
        {
            perror("Error al eliminar la cola del servidor");
            exit(-1);
        }

        exit(EXIT_SUCCESS);
    }

}