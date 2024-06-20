#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

typedef int pipe_t[2];

int main(int argc, char** argv){


    int fd;

    int pid, pidFiglio, status;

    char linea[250];

    pipe_t *pipedPF;      //array di pipe per comunicazione padre --> figlio
    pipe_t *pipedFP;      //array di pipe per comunicazione figlio --> padre



    //controllo numero parametri
	if (argc < 2)
	{
        printf("ERRORE - il numero di parametri deve essere almeno 2, parametri inseriti : %d\n", argc-1);
        exit(1);	
	}

    int N = argc-1;

    //alloco la memoria per l'array di pipe padre-figlo
    pipedPF = malloc(sizeof(pipe_t)*N);
    pipedFP = malloc(sizeof(pipe_t)*N);
    if(pipedPF == NULL){
        printf("ERRORE - allocazione array di pipe non riuscita\n"); 
        exit(2);
    }
    if(pipedFP == NULL){
        printf("ERRORE - allocazione array di pipe non riuscita\n"); 
        exit(3);
    }

    //genero N pipe padre-figlio
    for(unsigned i = 0; i < N; i++){
        
        //creo la pipe padre-figlio
        if(pipe(pipedPF[i]) < 0) {
            printf("ERRORE - creazione della pipe padre-figlio\n"); 
            exit(3);
        } 
        if(pipe(pipedFP[i]) < 0) {
            printf("ERRORE - creazione della pipe figlio-padre\n"); 
            exit(4);
        } 
    }


    //genero i N processi
    for(unsigned n =0; n < N; n++){


        //fork e controllo fork per figlio
        if ((pid = fork()) < 0)
        {
            printf("ERRORE - fork\n");
            exit(4);
        }

        //figlio
        if (pid == 0){

            //chiudo le pipe inutilizzate per il carattere
            for(unsigned j = 0; j < N; j++){
                close(pipedFP[j][0]);       //lettura
                close(pipedPF[j][1]);       //scrittura

                if(j != n){     //scrittura/lettura diversa dalla propria
                    close(pipedFP[j][1]);
                    close(pipedPF[j][0]);
                }                  

            }

            //apro il file relativo al figlio
            if ((fd = open(argv[n+1], O_RDONLY)) < 0) {
                printf("ERRORE - non riesco ad aprire il file F%d\n", n); 
                exit(5);
            }

            //leggo un carattere alla volta per ottenere l'ultima linea
            int cc = 0;

            while( read(fd, &(linea[cc]), 1) != 0 ){
                if(linea[cc] == '\n') cc=0;
                else cc++;
            }
            linea[cc+1] = '\0';

            //estraggo il primo carattere della linea
            char fchar = linea[0];

            //in linea ho l'ultima linea e in fchar ho il primo carattere della linea, lo invio al padre
            write(pipedFP[n][1], &(fchar), 1);

            //leggo il segnale per lo stampaggio
            char ssig;
            int sret = read(pipedPF[n][0], &ssig, 1);
            if(sret == 0){
                printf("ERRORE - non riesco a leggere il segnale per lo stampaggio dal figlio di indice %d\n", n); 
                exit(-1);
            }
            //se è affermativo procedo con lo stampaggio
            if(ssig == 1){
                printf(" ----- OUTPUT -----\n figlio di INDICE : %d \n con PID : %d \n avente come ULTIMA LINEA : \"%s\" \n e come MINIMO CARATTERE : %c\n", n, getpid(), linea, fchar); 
                exit(1);
            }
            exit(0);
        }
    }


    //padre

    //chiudo i lati delle pipe che non utilizzo
    for(unsigned j = 0; j < N; j++){
        close(pipedFP[j][1]);       //scrittura
        close(pipedPF[j][0]);       //lettura
    }

    //leggo i caratteri inviati dai figli
    char car;
    char min;
    int pmin;

    for(unsigned j = 0; j < N; j++){
        
        //leggo il carattere
        int rret = read(pipedFP[j][0], &car, 1);
        if(rret == 0){
            printf("ERRORE - non riesco a leggere il carattere inviato dal figlio di indice %d\n", j); 
            exit(5);
        }

        //se è minimo lo sostituisco al vecchio minimo
        if(j==0 || car < min){
            min = car;
            pmin = j;
        }
    }

    //in pmin ho il processo che mi ha restituito il minimo, invio il segnale di stampare 1 stampa e 0 non stampare
    char print= 1;
    char not_print= 0;
    for(unsigned j = 0; j < N; j++){

        if(j == pmin){
            write(pipedPF[j][1], &print, 1);
        }else{
            write(pipedPF[j][1], &not_print, 1);
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