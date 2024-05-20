#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>



int main(int argc, char** argv){


    int fd;

    int pid, pidFiglio, status;

    int piped[2][2];        //doppia pipeline la prima per la comunicazione padre-figlio e la seconda per figlio-nipote

    //controllo numero parametri
	if (argc < 3)
	{
        printf("ERRORE - il numero di parametri deve essere almeno 2, parametri inseriti : %d\n", argc-1);
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

    //genero i N processi
    for(unsigned i =0; i < N; i++){

        //creo la pipe padre-figlio
        if(pipe(piped[0]) < 0) {
            printf("ERRORE - creazione della pipe padre-figlio\n"); 
            exit(3);
        } 


        //fork e controllo fork per figlio
        if ((pid = fork()) < 0)
        {
            printf("ERRORE - fork\n");
            exit(4);
        }

        //figlio
        if (pid == 0){

            //chiudo la pipe in lettura
            close(piped[0][0]);

            //creo la pipe figlio-nipote
            if(pipe(piped[1]) < 0) {
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
                //chiudo il lato di lettura
                close(piped[1][0]);

                //chiudo lo standard output così che quando duplico la pipeline la scrittura della pipeline sostituisca lo standard output
                close(1);

                //duplico la pipe (così sostituisce lo standard output)
                if(dup(piped[1][1]) == -1){
                    printf("ERRORE - duplicazione pipeline\n");
                    exit(-1);
                }

                //chiudo lo standard input e lo sostituisco con il file che devo passare (non passo direttamente il file come parametro perchè se no il wc mi restituisce non solo il numero di linee ma anche il nome del file che dopo non riesco più a converitre in numero)
                close(0);
                open(argv[i+1], O_RDONLY);
                
                //conto le linee (l'output verrà scritto diretto nella pipeline)
                execlp("wc", "wc", "-l", (char *)0);
            
                //in caso di errore
                exit(-1);
            }

            //figli

            //chiudo la pipe in scrittura
            close(piped[1][1]);
            
            //legggo dalla pipeline
            char* n_line;
            char ch;
            int lenght =0;
            while(read(piped[1][0], &ch, 1)){
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
                printf("ERRORE - wait figlio del padre INDICE = %d\n", i);
                exit(-1);
            }
            if ((status & 0xFF) != 0)
                printf("ERRORE - Figlio con PID = %d terminato in modo anomalo\n", pidFiglio);
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
            write(piped[0][1], &n_line_int, sizeof(int));

            exit(0);
        }

        //padre

        //chiudo la pipe di scrittura
        close(piped[0][1]);

        //leggo il valore di ritorno
        int ret_wc_int = 0;
        read(piped[0][0], &ret_wc_int, sizeof(int));

        printf("\nIl figlio ha returnato un numero di linea = %d\n", ret_wc_int);

        //aspetto il figlio
        if ((pidFiglio=wait(&status)) < 0)
        {
            printf("ERRORE - wait figlio del padre INDICE = %d\n", i);
            exit(5);
        }
        if ((status & 0xFF) != 0)
            printf("ERRORE - Figlio con PID = %d terminato in modo anomalo\n", pidFiglio);
        else
        {	
            int ret=(int)((status >> 8) & 0xFF);
            printf("Il Figlio con PID = %d ha verificato che il file %s ha esattamente %d linee ed è terminato con codice di ritorno = %d\n", pidFiglio, argv[i+1], ret_wc_int, ret);
        }


    }
    exit(0);
}