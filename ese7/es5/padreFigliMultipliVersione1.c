#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char **argv){

    int N;
    int pid;
    int pidFiglio;
    int status;
    int ret;

    // controllo che il numero di parametri sia corretto 1 + 1
    if (argc != 2) {
        printf("Errore: Necessari esattamente 1 parametro per %s e invece argc=%d\n", argv[0], argc); 
        exit(1);
    }

    //controllo che il primo parametro sia un intero strettamente minore di 255
    N=atoi(argv[1]);
	if (N <= 0 || N >= 255) 
	{ 	
		printf("Errore: Il secondo parametro %s non e' un numero strettamente maggiore di 0\n", argv[2]); 
		exit(2); 
	}

    printf("PID processo corrente : %d\nnumero di figli da creare N = %d\n", getpid(), N);

    //creo N processi figli
    for(int i=0; i < N; i++){
        
        pid = fork();   //creo il processo figlio

        if(pid < 0){    //controllo che la creazione si andata a buon fine
            printf("ERRORE fork : %d al processo numero : %d\n", pid, i);
	        exit(3);
        }

        if (pid == 0){
            //figlio
            printf("INFO : sono il figlio numero = %d con PID = %d e PPID = %d\n",i, getpid(), getppid());
            exit(i);
        }
        
        //padre
        pidFiglio = wait(&status);  //aspetto il figlio

        if( pid == pidFiglio ){ //wait terminata correttamente

            if ((status & 0xFF) != 0){  //controllo che il figlio sia terminato correttamente
                printf("ERRORE figlio numero %d terminato inaspettatamente\n", i);
                exit(4);

            }else{
                //caso corretto : il figlio è terminato correttamente e il padre lo ha aspettato
                ret=(int)((status >> 8) & 0xFF);
                printf("figlio numero %d terminato con PID = %d\n", ret, pidFiglio);
            }
            

        }else if (pidFiglio < 0){ //errore wait
            printf("ERRORE wait figlio numero %d : pidFiglio = %d\n",i, pidFiglio);
            exit(5);

        }else{    //errore wait
            printf("ERRORE fork e wait non corrispondono per il figlio numero %d\n", i);
            exit(6);
        }

    }
    exit(0);
}