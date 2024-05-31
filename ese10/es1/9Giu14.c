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

    pipe_t *pipedPF;       //N pipe padre-figlio
    pipe_t pipedFN;        //1 pipe figlio-nipote

    //controllo numero parametri
	if (argc < 3)
	{
        printf("ERRORE - il numero di parametri deve essere almeno 2, parametri inseriti : %d\n", argc-1);
        exit(1);	
	}

    //controllo parametri
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
        printf("ERRORE - allocazione array delle pipe Padre-Figlio non riuscita\n"); 
        exit(3);
    }

    //creo le N pipe padre-figlio
    for(unsigned i = 0; i < N; i++){
        
        //creo la pipe padre-figlio
        if(pipe(pipedPF[i]) < 0) {
            printf("ERRORE - creazione della pipe padre-figlio\n"); 
            exit(4);
        } 
    }

    //genero i N processi figli
    for(unsigned i =0; i < N; i++){


        //fork e controllo fork per figlio
        if ((pid = fork()) < 0)
        {
            printf("ERRORE - fork creazione figlio\n");
            exit(5);
        }

        //figlio
        if (pid == 0){

            //chiudo le pipe che non mi servono
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
                printf("ERRORE - fork creazione nipote\n");
                exit(-1);
            }

            //nipote
            if(pid == 0){
                //chiudo il lato di lettura
                close(pipedFN[0]);

                //chiudo lo standard output così che quando duplico la pipeline la scrittura della pipeline sostituisca lo standard output
                close(1);

                //duplico la pipe (così sostituisce lo standard output)
                if(dup(pipedFN[1]) == -1){
                    printf("ERRORE - duplicazione pipeline\n");
                    exit(-1);
                }

                //chiudo lo standard input e lo sostituisco con il file che devo passare (non passo direttamente il file come parametro perchè se no il wc mi restituisce non solo il numero di linee ma anche il nome del file che dopo non riesco più a converitre in numero)
                close(0);
                open(argv[i+1], O_RDONLY);
                
                //conto le linee (l'output verrà scritto diretto nella pipeline)
                execlp("wc", "wc", "-l", (char *)0);
            
                //in caso di errore (scrivo in stderror perchè stdout è sostituita dalla pipeline)
                perror("ERRORE - problemi del nipote a eseguire la wc");
                exit(-1);
            }

            //figli

            //chiudo la pipe in scrittura
            close(pipedFN[1]);
            
            //leggo dalla pipeline (versione completa con realloc)
            char* n_line;
            char ch;
            int lenght =0;
            while(read(pipedFN[0], &ch, 1)){
                n_line = realloc(n_line, lenght+1);
                if(n_line == NULL){
                    printf("ERRORE - realloc\n");
                    exit(-1);
                }
                n_line[lenght] = ch;
                lenght++;
            }
            printf("DEBUG - Nipote ha ritornato il numero di linee %s\n", n_line);

            //aspetto il nipote
            if ((pidFiglio=wait(&status)) < 0)
            {
                printf("ERRORE - wait nipote del figlio con INDICE = %d\n", i);
                exit(-1);
            }
            if ((status & 0xFF) != 0)
                printf("ERRORE - Nipote con PID = %d terminato in modo anomalo\n", pidFiglio);
            else
            {	
                int ret=(int)((status >> 8) & 0xFF);
                printf("DEBUG - Nipote con PID = %d terminato con valore di ritorno = %d\n", pidFiglio, ret);
            }

            //aggiungo il terminatore di stringa
            n_line = realloc(n_line, lenght+1);
            n_line[lenght] = '\0';

            //converto in numero
            int n_line_int = atoi(n_line);

            //ritorno il valore al padre scrivendolo sulla pipe padre-figlio
            write(pipedPF[i][1], &n_line_int, sizeof(int));

            exit(0);
        }

    }

    //padre

    //chiudo le pipe di scrittura
        for(unsigned j = 0; j < N; j++){
        close(pipedPF[j][1]);       //scrittura
    }

    //leggo da ogni pipe di lettura
    int ret_wc_int = 0;
    for(unsigned j = 0; j < N; j++){
        
        int pipe_ret = read(pipedPF[j][0], &ret_wc_int, sizeof(int));

        //controllo che la lettura sia andata a buon fine
        if(pipe_ret != 0){

            printf("\nIl figlio numero : %d ha contato %d linee\n", j, ret_wc_int);

        }else{
            printf("ERRORE - lettura padre dalla pipe padre-figlio, figlio numero= %d\n", j);
        }         

    }
    //aspetto il figlio
    if ((pidFiglio=wait(&status)) < 0)
    {
        printf("ERRORE - wait figlio\n");
        exit(5);
    }
    if ((status & 0xFF) != 0)
        printf("ERRORE - Figlio con PID = %d terminato in modo anomalo\n", pidFiglio);
    else
    {	
        int ret=(int)((status >> 8) & 0xFF);
        printf("Il Figlio con PID = %d ha returnato %d \n", pidFiglio, ret);
    }

    exit(0);
}