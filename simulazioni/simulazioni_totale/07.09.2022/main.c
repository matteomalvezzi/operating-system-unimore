#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>
#define PERM 0644

typedef int pipe_t[2];


int main(int argc, char** argv){


    int fd;

    int pid, pidFiglio, status;

    pipe_t *pipedFF;  //array di pipe per la comunicazione figlio(i) ---> figlio(i+1)

    int fcreato;

    //controllo numero parametri
	if (argc < 3)
	{
        printf("ERRORE - il numero di parametri deve essere almeno 2, parametri inseriti : %d\n", argc-1);
        exit(1);	
	}
    int N = argc-1;

    //creo il file
	if ((fcreato=creat("MALVEZZI", PERM)) < 0)
	{
        printf("ERRORE - creazione del file MALVEZZI fallita\n"); 
        exit(2);
	}

    //alloco la memoria per l'array di pipe
    pipedFF = (pipe_t *) malloc(sizeof(pipe_t) * N);
    if(pipedFF == NULL){
        printf("ERRORE - allocazione array di pipe figlio-figlio non riuscita\n"); 
        exit(2);
    }

    //genero N pipe figlio-figlio
    for(unsigned n = 0; n < N; n++){
        
        //creo la pipe figlio-figlio
        if(pipe(pipedFF[n]) < 0) {
            printf("ERRORE - creazione della pipe figlio-figlio\n"); 
            exit(3);
        }
    }


    //alloco la memoria per l'array cur (lo faccio prima delle fork così è disponibile sia per il padre che per tutti i figli)
    char *cur = (char *) malloc(sizeof(char)*N);
    if(cur == NULL){
        printf("ERRORE - allocazione array di caratteri fallita\n"); 
        exit(4);
    }

    //genero i N processi
    for(unsigned n =0; n < N; n++){


        //fork e controllo fork per figlio
        if ((pid = fork()) < 0)
        {
            printf("ERRORE - fork\n");
            exit(6);
        }

        // ---------- figlio ----------
        if (pid == 0){

            //chiudo le pipe inutilizzate
            for(unsigned j = 0; j < N; j++){    
                if (j != n){        //chiudo le scritture diverse dalla propria
                    close(pipedFF[j][1]);
                }                   //chiudo le letture (se sono il primo o se le letture sono diverse da quella successiva)
                if ( (n == 0) || (j != n-1)){
                    close(pipedFF[j][0]);
                }
            }
            
            //apro il file relativo al figlio
            if ((fd = open(argv[n+1], O_RDONLY)) < 0) {
                printf("ERRORE - non riesco ad aprire il file F%d\n", n); 
                exit(-1);
            }

            //leggiamo tutti i caratteri del file
            
            char car;
            int ncar = 0;
            while( read(fd, &car, 1) != 0){
                
                ncar++;
                //controlliamo che il carattere corrente sia di posizione disapri
                if( (ncar %2 ) == 1){
                    
                    //se è in posizione dispari aspettiamo l'array di dispari dal precedente (escluso P0)
                    if(n != 0){
                        int rret = read(pipedFF[n-1][0], cur, sizeof(char)*N);
                        if(rret != N*sizeof(char)){
                            printf("ERRORE - lettura dell'array proveniente dal figlio precedente fallita\n"); 
                            exit(-1);
                        }
                    }

                    //scrivo il carattere corrente nell'array ricevuto
                    cur[n] = car;

                    //passo al prossimo figlio l'array
                    int wret = write(pipedFF[n][1], cur, sizeof(char)*N);
                    if(wret != N*sizeof(char) ){
                        printf("ERRORE - scrittura dell'array per il figlio successivo fallita\n"); 
                        exit(-1);
                    }
                }
            }

            exit(car);
        }
    }


    // ---------- padre ----------

    //chiudo i lati delle pipe che non utilizzo
    for(unsigned j = 0; j < N; j++){
        close(pipedFF[j][1]);       //tutte le scritture visto che il padre non deve scrivere
        if (j != N-1) close(pipedFF[j][1]);     //tutte le letture tranne l'ultima
    }

	/* il padre deve leggere tutti gli array di caratteri inviati dall'ultimo figlio */
	while (read(pipedFF[N-1][0],cur,N*sizeof(char)))
	{
        //se il numero di caratteri letto è corretto scrivo l'array sul file creato all'inzio
        int pwret = write(fcreato, cur, sizeof(char)*N);
        if(pwret != sizeof(char)*N){
            printf("ERRORE - scrittura dell'array nel file fcreato fallita\n"); 
            exit(6);
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