#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

typedef int pipe_t[2];

int endarray(int *end, int lenght){
    for (unsigned j = 0; j < lenght; j++){
        if(end[j] == 0) return 0;
    }
    return 1;
}


int main(int argc, char** argv){


    int fd, fdN;

    int pid, pidFiglio, status;

    pipe_t *pipedPF;      //array di pipe per padre-figlio (N pipe per N file)



    //controllo numero parametri
	if (argc < 4)
	{
        printf("ERRORE - il numero di parametri deve essere almeno 3, parametri inseriti : %d\n", argc-1);
        exit(1);	
	}

    //controllo parametri
    for(unsigned i = 1; i < argc; i++){
        if ((fd = open(argv[i], O_RDONLY)) < 0) {
        printf("ERRORE - il parametro %s non è un nome assoluto di un file\n", argv[i]); 
        exit(2);
        }
    }
    int N = argc-2;

    //apro il file FN+1
    if ((fdN = open(argv[N+1], O_WRONLY)) < 0) {
        printf("ERRORE - non riesco ad aprire l'ultimo file\n ovvero il file F%d", N+1); 
        exit(3);
    }

    //alloco la memoria per l'array di pipe padre-figlo
    pipedPF = malloc(sizeof(pipe_t)*N);
    if(pipedPF == NULL){
        printf("ERRORE - allocazione array di pipe non riuscita\n"); 
        exit(4);
    }

    //genero N pipe padre-figlio
    for(unsigned i = 0; i < N; i++){
        
        //creo la pipe padre-figlio
        if(pipe(pipedPF[i]) < 0) {
            printf("ERRORE - creazione della pipe padre-figlio\n"); 
            exit(4);
        } 
    }


    //genero i N processi
    for(unsigned i =0; i < N; i++){


        //fork e controllo fork per figlio
        if ((pid = fork()) < 0)
        {
            printf("ERRORE - fork\n");
            exit(4);
        }

        //figlio
        if (pid == 0){

            //chiudo le pipe inutilizzate (lettura di tutte e scrittura solo di quelle non utilizzate)
            for(unsigned j = 0; j < N; j++){
                close(pipedPF[j][0]);       //lettura
                if(j != i)                  //scrittura diversa dalla propria
                    close(pipedPF[j][1]);
            }


            //apro il file relativo al figlio leggo due caratteri alla volta
            if ((fd = open(argv[i+1], O_RDONLY)) < 0) {
                printf("ERRORE - non riesco ad aprire il file F%d\n", i); 
                exit(5);
            }

            //leggo due caratteri alla volta e li invio sulla pipeline
            int counter = 0;
            char* chread = malloc(sizeof(char)*2);
            
            int ret_read = 1;
            while(ret_read != 0){
                
                //provo a leggere 2 caratteri, se poi è dispari e ne leggo solo 1 fa lo stesso, ne scriverò solo 1
                ret_read = read(fd, chread, sizeof(char)*2);

                //scrivo sulla pipeline PF i due caratteri
                write(pipedPF[i][1], chread, ret_read);
                counter+=ret_read;
            }
            
            exit(counter);
        }
    }


    //padre

    //chiudo i lati delle pipe che non utilizzo
    for(unsigned j = 0; j < N; j++){
        close(pipedPF[j][1]);       //scrittura
    }

    //leggo a 2 a 2 i caratteri per ogni figlio quindi da ogni pipe padre-figlio

    //creo un array che mi tenga traccia delle pipe già finite di leggere e lo metto tutto a 0=ancora da leggere 1=già letto
    int *endread = malloc(sizeof(int)*N);
    memset(endread, 0, sizeof(int)*N);

    //mi posiziono alla fine del file così è come scrivere in append
    lseek(fdN, 0L, SEEK_END);
    char cread[2];

    //la funzione restituisce 1 quando tutti i valori dell'array sono a 1
    while(endarray(endread, N) != 1){

        for(unsigned j = 0; j < N; j++){
            
            //leggo dalla relativa pipe
            int pipe_ret = read(pipedPF[j][0], cread, sizeof(char)*2);

            //se la lettura è buona lo scrivo nell'ultimo file, se no dichiaro che la pipe è stata tutta letta
            if(pipe_ret != 0){

                //ATTENZIONE: quando scrivo sul file devo usare pipe_ret e non 2 perchè se per caso ne ho letto solo 1 non ha senso che ne scrivo 2
                write(fdN, cread, pipe_ret);
            }else{
                endread[j] = 1;
            }

        }


    }

    //Il padre aspetta i figli
    for (unsigned j = 0; j < N; j++){
        if ((pidFiglio=wait(&status)) < 0)
        {
            printf("ERRORE - wait figlio del padre INDICE = %d\n", j);
            exit(5);
        }
        if ((status & 0xFF) != 0)
            printf("ERRORE - Figlio con PID = %d terminato in modo anomalo\n", pidFiglio);
        else
        {	
            int ret=(int)((status >> 8) & 0xFF);
            printf("Il Figlio con PID = %d ha returnato = %d\n", pidFiglio, ret);
        }
        
    }

    exit(0);
}