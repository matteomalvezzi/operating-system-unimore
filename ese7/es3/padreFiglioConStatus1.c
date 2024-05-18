#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>


int mia_random(int n) 
{ 
    int casuale; 
    casuale = rand() % n; 
    return casuale; 
}

int main(){

    const int RANGE=100;
    int pid;
    int pidFiglio;
    int status;
    int ret;

    int pidFiglio2;
    int status2;

    srand(time(NULL));
    printf("PID processo corrente : %d\n", getpid());


	if ((pid = fork()) < 0)
	{
	    printf("ERRORE fork : %d\n", pid);
	    exit(1);
	}

    if (pid == 0){
        
        //figlio
	    printf("INFO : sono il figlio con PID = %d e PPID = %d\n", getpid(), getppid());
	    int r=mia_random(RANGE);
	    exit(r);
    }

    //padre

    pidFiglio = wait(&status);
    pidFiglio2 = wait(&status2);


    if( pid == pidFiglio ){ //wait terminata correttamente
        printf("Figlio1 terminato con PID = %d\n", pidFiglio);

    }else if (pidFiglio < 0){ //errore wait
        printf("ERRORE  wait Figlio1 : %d\n", pidFiglio);
        exit(2);

    }else{    //errore wait
        printf("ERRORE fork e wait non corrispondono\n");
        exit(3);
    }

    if( pidFiglio2 < 0){
        printf("ERRORE  wait Figlio2 : %d\n", pidFiglio2);
        exit(4);
    }

    if ((status & 0xFF) != 0 && (status2 & 0xFF) != 0){
        printf("ERRORE figli terminati inaspettatamente\n");
    }else{
        //caso corretto : il figlio è terminato correttamente e il padre lo ha aspettato
        ret=(int)((status >> 8) & 0xFF);  //visto che l'exit ritorna i segnali nei primi 8 bit faccio uno shift di 8 bit
        printf("Il figlio con PID = %d ha ritornato %d\n", pidFiglio, ret);
    }

    exit(0);
}