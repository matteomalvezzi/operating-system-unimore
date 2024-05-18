#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char **argv){


    int fd;    
    char Cx;    //parametro carattere
    int N;      //numero di file
    int* F;     //array di file

    int pid;
    int pidFiglio;
    int status;
    int ret;

    // controllo che il numero di parametri sia corretto (N+1) + 1
    if (argc < 4) {
        printf("Errore: Necessari almeno 3 parametri per %s e invece argc=%d\n", argv[0], argc); 
        exit(1);
    }

    //controllo che i primi N parametri siano nomi di file
    N=argc-2;
    F=malloc(N*sizeof(int));
    for(unsigned i = 1; i < argc-1; i++){
        
        if ((fd = open(argv[i], O_RDONLY)) < 0) {
        
            printf("Errore: il parametro %s deve essere un nome di un file\n", argv[i]); 
            exit(2);
        }
        F[i-1] = fd;
    }

    //controllo che l'ultimo parametro sia un singolo carattere
    if (strlen(argv[argc-1]) != 1)
	{ 	
		printf("Errore: L'ultimo parametro %s non e' un singolo carattere\n", argv[argc-1]); 
		exit(3); 
	}
    Cx = argv[argc-1][0];

    //creo N processi figli
    for(int i=0; i < N; i++){
        
        pid = fork();   //creo il processo figlio

        if(pid < 0){    //controllo che la creazione si andata a buon fine
            printf("ERRORE fork : %d al processo numero : %d\n", pid, i);
	        exit(4);
        }

        if (pid == 0){
            //figlio
            char c;
            unsigned tot = 0;
            while(read(F[i], &c, 1)){
                if(c == Cx) tot++; 
            }
            exit(tot);
        }

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
            printf("figlio numero %d con PID =  %d ha letto il file %s e ha trovato = %d caratteri \"%c\"\n",i, pidFiglio, argv[i+1], ret, Cx);
        }

    }
    
    exit(0);
}