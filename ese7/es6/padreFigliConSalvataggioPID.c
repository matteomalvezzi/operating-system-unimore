#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

int mia_random(int n) 
{ 
    int casuale; 
    casuale = rand() % n; 
    return casuale; 
}

int main(int argc, char **argv){

    int N;
    int *pid;
    int pidFiglio;
    int status;
    int ret;

    // controllo che il numero di parametri sia corretto 1 + 1
    if (argc != 2) {
        printf("Errore: Necessari esattamente 1 parametro per %s e invece argc=%d\n", argv[0], argc); 
        exit(1);
    }

    //controllo che il primo parametro sia un intero strettamente minore di 155
    N=atoi(argv[1]);
	if (N <= 0 || N >= 155) 
	{ 	
		printf("Errore: Il secondo parametro %s non e' un numero strettamente maggiore di 0\n", argv[2]); 
		exit(2); 
	}

    printf("PID processo corrente : %d\nnumero di figli da creare N = %d\n", getpid(), N);
    srand(time(NULL));
    
    //allochiamo l'array dinamico
    pid = (int *)malloc(N*sizeof(int));
    if (pid == NULL){
        printf("ERRORE allocazione array di pid\n");
        exit(3);
    }

    //creo N processi figli
    for(int i=0; i < N; i++){
        
        pid[i] = fork();   //creo il processo figlio e lo metto nell'array

        if(pid[i] < 0){    //controllo che la creazione si andata a buon fine
            printf("ERRORE fork : %d al processo numero : %d\n", pid[i], i);
	        exit(4);
        }

        if (pid[i] == 0){
            //figlio
            int random = mia_random(100+i);
            printf("INFO : sono il figlio numero = %d con PID = %d\n",i, getpid());
            exit(random);
        }

    }


    //aspetto N processi figli
    for (unsigned i = 0; i < N; i++)
    {
        //padre
        pidFiglio = wait(&status);

        if (pidFiglio < 0){ //errore wait
            printf("ERRORE wait figlio con PID = %d\n", pidFiglio);
            exit(5);
        }

        if ((status & 0xFF) != 0){  //controllo che il figlio sia terminato correttamente
            printf("ERRORE figlio con PID = %d terminato inaspettatamente\n", pidFiglio);
            exit(6);

        }else{
            //caso corretto : il figlio è terminato correttamente e il padre lo ha aspettato
            
            ret=(int)((status >> 8) & 0xFF);    //prendo il valore di ritorno

            for (unsigned j=0; j < N; j++){     //prendo l'indice del figlio
                if (pidFiglio == pid[j])
                {
                    printf("figlio numero %d con PID =  %d terminato con valore di ritorno = %d\n",j, pidFiglio, ret);
                    break;
                }
            }   
        }

    }
    
    exit(0);
}