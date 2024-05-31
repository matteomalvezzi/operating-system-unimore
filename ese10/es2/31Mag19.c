#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

typedef int pipe_t[2];


typedef struct 
{
    int c1;             //PID nipote
    int c2;             //lunghezza linea letta
    char c3[250];       //linea letta
} struttura;




int main(int argc, char** argv){


    int fd;

    int pid, pidFiglio, status;

    pipe_t *pipedPF;      //array di pipe per padre-figli
    pipe_t pipedFN;       //pipe per figlio-nipote

    struttura S;



    //controllo numero parametri
	if (argc < 4)
	{
        printf("ERRORE - il numero di parametri deve essere almeno 3, parametri inseriti : %d\n", argc-1);
        exit(1);	
	}

    //controllo primo parametro
    for(unsigned i = 1; i < argc; i++){
        if ((fd = open(argv[i], O_RDONLY)) < 0) {
        printf("ERRORE - il parametro %s non è un nome assoluto di un file\n", argv[i]); 
        exit(2);
        }
    }
    int N = argc-1;

    //alloco la memoria per l'array di pipe padre-figlo
    pipedPF = malloc(sizeof(pipe_t)*N);
    if(pipedPF == NULL){
        printf("ERRORE - allocazione array di pipe non riuscita\n"); 
        exit(3);
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

            //creo la pipe figlio-nipote
            if(pipe(pipedFN) < 0) {
                printf("ERRORE - creazione della pipe figlio-nipote\n"); 
                exit(-1);
            }  

            //fork e controllo fork per nipote
            if ((pid = fork()) < 0)
            {
                printf("ERRORE - fork\n");
                exit(-1);
            }

            //nipote
            if(pid == 0){
                //chiudo la pipe di lettura
                close(pipedFN[0]);

                //ridirigo lo standard output sulla pipe
                close(1);
                if(dup(pipedFN[1]) == -1){
                    printf("ERRORE - duplicazione pipeline\n");
                    exit(-1);
                }

                //non c'è bisogno di ridirigere lo standard input visto che gli passiamo direttamente il nome file come parametro

                //faccio il sort
                execlp("sort", "sort", "-f", argv[i+1], (char *)0);

                //in caso di errore
                exit(-1);
            }

            //figli

            //chiudo la pipe in scrittura
            close(pipedFN[1]);
            
            //setto c3

            //leggo dalla pipeline solo la prima linea (carattere per carattere fin che non trovo \n) NO REALLOC DATO CHE S.linea E' FISSATO A 250
            int lenght =0;
            while(read(pipedFN[0], &(S.c3[lenght]), 1)){

                if(S.c3[lenght] == '\n'){
                    lenght++;               //la lunghezza della linea in S deve contare il terminatore
                    break;
                }else
                    lenght++;
            
            }

            //setto c2
            S.c2 = lenght;

            //setto c1
            S.c1 = pid;

            //scrivo sulla pipeline PF la struttura
            write(pipedPF[i][1], &S, sizeof(S));

            //aspetto il nipote
            if ((pidFiglio=wait(&status)) < 0)
            {
                printf("ERRORE - wait nipote del figlio INDICE = %d\n", i);
                exit(-1);
            }
            if ((status & 0xFF) != 0)
                printf("ERRORE - Nipote con PID = %d terminato in modo anomalo\n", pidFiglio);
            else
            {	
                int ret=(int)((status >> 8) & 0xFF);
                printf("DEBUG - Nipote con PID = %d terminato con valore di ritorno = %d\n", pidFiglio, ret);
            }
            //il figlio returna al padre la lunghezza della linea SENZA terminatore
            exit(S.c2-1);
        }
    }


    //padre

    //chiudo i lati delle pipe che non utilizzo
    for(unsigned j = 0; j < N; j++){
        close(pipedPF[j][1]);       //scrittura
    }

    //leggo da ogni pipe padre-figlio i dati ricevuti dai figli
    for(unsigned j = 0; j < N; j++){
        
        int pipe_ret = read(pipedPF[j][0], &S, sizeof(S));
        printf("lettura padre figlio = %d\n", pipe_ret);
        //controllo che la lettura sia andata a buon fine
        if(pipe_ret != 0){
            
            //aggiungo il terminatore di stringa
            S.c3[S.c2] = '\0';
            printf("Il Nipote con PID = %d ha letto dal file %s la linea '%s' di lunghezza %d caratteri\n", S.c1, argv[j+1], S.c3, S.c2);

        }else{
            printf("ERRORE - lettura padre dalla pipe padre-figlio, figlio numero= %d\n", j);
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