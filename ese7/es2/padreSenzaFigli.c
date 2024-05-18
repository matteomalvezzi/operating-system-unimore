#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>


int main(){

    printf("PID processo corrente : %d\n", getpid());

    int status;
    int pidFiglio = wait(&status);

    if(pidFiglio < 0){
        printf("ERRORE  wait : %d\n", pidFiglio);
        printf("VALORE status : %d\n", status);
        exit(1);
    }
    exit(0);
}