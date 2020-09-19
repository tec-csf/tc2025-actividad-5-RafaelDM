/*
Rafael Díaz Medina A01024592 Actividad 5 Testigos
Me base en su programa de Pipe de el repositorio que nos compartio
También menciono que esta vez trabaje con Edgar García mi equipo de la materia
No logre volver a abrir el proceso despues de cerrarlo y cuando no lo cierro mi programa deja de funcionar
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void proteccion(int, char);
void abandono(int, char);

int main(int argc, char const *argv[]){
    char * number;
    char testigo= 'T';
    int i = 1;
    pid_t *pid;
    int count=0;

	number =(char *) malloc(sizeof number);
	printf("Numero de hijos a crear: ");
	scanf("%s", number);
	printf("\n");
	int children = atoi(number);

    int tuberia[2*children];
    for (int i = 0; i < children; i++){
        pipe(&tuberia[2*i]);
    }

    while (i <= children){
        *pid = fork();

        if (*pid == 0){
            //--------------Hijos
            close(tuberia[(i*2 - 1)]);
            proteccion(tuberia[(i*2 - 1)], testigo);
            close(tuberia[(i*2)]);
            abandono(tuberia[(i*2)], testigo);    
        }
        else if (*pid == -1){
            //----------error----------
            printf("Hubo un error al crear proceso hijo. Numero de procesos hijos creados hasta ahora: %d\n", i + 1);
            return 1;
        }
        else{
            // Por cada hijo nuevo hacemos un break
            break;
        }
        i++;
    }
    // Cada padre espera a su hijo
    waitpid(*pid, NULL, 0);
    return 0;
}

void proteccion(int tuberia, char testigo){
    //Funcion para cuidar al testigo like in la ley y el orden jaja pero aqui si los cuidamos
    read(tuberia, &testigo, sizeof(char));
    printf("—-> Soy el proceso con PID %d  y recibí el testigo '%c', el cual tendré por 5 segundos\n", getpid(), testigo);
    sleep(5);
}

void abandono(int tuberia, char testigo){
    //Funcion para regresar al testigo
    write(tuberia, &testigo, sizeof(char));
    printf("<—- Soy el proceso con PID %d y acabo de enviar el testigo '%c'\n", getpid(), testigo);
}
