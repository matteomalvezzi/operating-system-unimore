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

    pipe_t *pipedFP;  //array di pipe per la comunicazione figlio-padre
    pipe_t *pipedPF;   //array di pipe per la comunicazione padre-figlio

    //controllo numero parametri
	if (argc < 3)
	{
        printf("ERRORE - il numero di parametri deve essere almeno 2, parametri inseriti : %d\n", argc-1);
        exit(1);	
	}
    int N = argc-1;

    //alloco la memoria per gli array di pipe
    pipedPF = malloc(sizeof(pipe_t)*N);
    pipedFP = malloc(sizeof(pipe_t)*N);
    if(pipedPF == NULL){
        printf("ERRORE - allocazione array di pipe padre-figlio non riuscita\n"); 
        exit(2);
    }
    if(pipedFP == NULL){
        printf("ERRORE - allocazione array di pipe figlio-padre non riuscita\n"); 
        exit(3);
    }

    //genero N pipe padre-figlio e N pipe figlio-padre
    for(unsigned i = 0; i < N; i++){
        
        //creo la pipe padre-figlio
        if(pipe(pipedPF[i]) < 0) {
            printf("ERRORE - creazione della pipe padre-figlio\n"); 
            exit(4);
        } 
        //creo la pipe figlio-padre
        if(pipe(pipedFP[i]) < 0) {
            printf("ERRORE - creazione della pipe figlio-padre\n"); 
            exit(5);
        } 
    }

    //genero i N processi
    for(unsigned n =0; n < N; n++){


        //fork e controllo fork per figlio
        if ((pid = fork()) < 0)
        {
            printf("ERRORE - fork\n");
            exit(6);
        }

        //figlio
        if (pid == 0){

            //chiudo le pipe inutilizzate
            for(unsigned j = 0; j < N; j++){    
                close(pipedFP[j][0]);   //lettura dal figlio al padre
                close(pipedPF[j][1]);   //scrittura dal padre al figlio

                if(j != n){     //chiudo la scrittura e la lettura diversa dalla propria
                    close(pipedPF[j][0]);
                    close(pipedFP[j][1]);
                }
            }
            
            //apro il file relativo al figlio
            if ((fd = open(argv[n+1], O_RDONLY)) < 0) {
                printf("ERRORE - non riesco ad aprire il file F%d\n", n); 
                exit(-1);
            }

            //leggo carattere per carattere e quando incontro un end-line invio il carattere precedente e aspetto di leggere le indicazioni del padre
            char linea[40];
            char car;
            char prec_chread;

            char flag = 0;
            int counter = 0;

            while(read(fd, &(car), 1) != 0){
                //costruisco la linea
                linea[counter] = car;
                counter++;

                //controllo che non sia a fine linea
                if(car == '\n'){

                    //linea letta
                    linea[counter +1] = '\0';
                    printf("Figlio %d ha letto la linea : %s\n",n, linea);

                    //nel caso in cui sono a fine linea invio al padre l'ultimo carattere
                    write(pipedFP[n][1], &(prec_chread), 1);

                    //aspetto le istruzioni dal padre che si occuperà di confrontare i caratteri degli altri figli
                    int rret = read(pipedPF[n][0], &(flag), 1);

                    //controllo che la read dal padre sia andata a buon fine
                    if(rret != 1){
                        printf("ERRORE - il figlio di INDICE : %d non riesce a leggere le comunicazioni dal padre\n", n);
                    }

                    if (flag ==1){
                        //dobbiamo stampare
                        printf("Il processo con le seguenti caratteristiche\n INDICE = %d\n PID = %d\n CARATTERE LETTO = %c\n LINEA LETTA = %s\n ha il carattere maggiore di tutti\n", n, getpid(), prec_chread, linea);
                    }
                    //resetto il counter
                    counter=0;
                }
                prec_chread = car;
            }

            exit(0);
        }
    }


    //padre
    //chiudo i lati delle pipe che non utilizzo
    for(unsigned j = 0; j < N; j++){
        close(pipedPF[j][0]);       //lettura dal padre ai figli
        close(pipedFP[j][1]);       //scrittura dal figlio al padre
    }

    //ciclo fin che tutti i file non hanno finito
    int end_flag = 0;
    while(end_flag == 0){
        
        //leggo i caratteri che mi vengono mandati
        char cread;
        char maxc;
        int maxcp;
        char firstc = 0;

        end_flag = 1;       //utilizzo un flag che mi segna che almeno una read abbia letto caratteri

        for(unsigned j = 0; j < N; j++){
            
            //leggo dalla relativa pipe
            int pipe_ret = read(pipedFP[j][0], &cread, 1);

            //se la lettura è buona confronto il carattere con gli altri
            if(pipe_ret != 0){

                end_flag = 0;
                if(firstc ==0){
                    maxc = cread;
                    maxcp = 0;
                    firstc = 1;
                }
                else{
                    if(cread > maxc){
                        maxc = cread;
                        maxcp = j;
                    }
                }
            }
        }

        //invio la comunicazione ai figli per lo stampaggio
        char print=1;
        char notprint=0;
        for(unsigned j = 0; j < N; j++){
            if(maxcp == j){
                write(pipedPF[j][1], &(print), 1);
            }else{
                write(pipedPF[j][1], &(notprint), 1);
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