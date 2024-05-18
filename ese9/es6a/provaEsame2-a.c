#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

/* ----- Versione 1
    Utilizziamo N pipe differenti per ogni carattere
    creiamo un nuovo tipo di dato ovvero pipe_t
    pipe_t è  un array di 2 interi esso specifica
    ci servirà per creare un array di pipe

    infatti ogni pipe è un array di 2 di interi (scrittura e lettura)

    successivamente possiamo creare un array dinamico di pipe_t
    pipe_t *piped;
*/

typedef int pipe_t[2];

int main (int argc, char** argv)
{

    int fd;
    int N;
    pipe_t *piped;

    int pid, pidFiglio, status;

    char ch;

    //controllo numero parametri
	if (argc < 3)
	{
        printf("ERRORE - il numero di parametri deve essere almeno 2: %d\n", argc);
        exit(1);	
	}

    //controllo primo parametro
    if ((fd = open(argv[1], O_RDONLY)) < 0) {
        printf("ERRORE - il parametro %s non è un nome di un file\n", argv[1]); 
        exit(2);
    }

    //controllo altri N parametri
    for(unsigned i = 2; i < argc; i++){
        if (strlen(argv[i]) != 1)
        { 	
            printf("Errore: Il parametro numer %d : %s non e' un singolo carattere\n", i,argv[i]); 
            exit(3); 
        }
    }
    N = argc-2;

    //allocazione dinamica array di pipse
    piped = (pipe_t *) malloc (N*sizeof(pipe_t)); 
    if (piped == NULL) 
    { 
        printf("ERRORE - allocazione memoria\n"); 
        exit(4);
    }

    //creo le pipe
    for (unsigned j=0; j < N; j++) 
    {
        if(pipe(piped[j]) < 0) {
            printf("ERRORE - creazione della pipe\n"); 
            exit(5);
        } 
    }

    //genero i N processi
    for(unsigned i =0; i < N; i++){

        //fork e controllo fork
        if ((pid = fork()) < 0)
        {
            printf("ERRORE - fork\n");
            exit(6);
        }

        //figlio
        if (pid == 0){

            //chiudo le pipe di lettura (e quelle di scrittura che non mi servono per il processo corrente(se sono al processo 3 le scritture del 2 le posso chiudere))
            for(unsigned j =0; j <N; j++){
                close(piped[j][0]);
                if (j != i)  
                    close(piped[j][1]);
            }

            //apro il file
            if ((fd = open(argv[1], O_RDONLY)) < 0)
            {   
                printf("ERRORE - in apertura del file %s\n", argv[1]);
                exit(-1);
            }
            
            //conto le ricorrenze
            long int count =0;
            while (read(fd, &ch, 1) > 0) 
            {   
                if(ch == argv[i+2][0])
                    count++; 
            }
            //scrivo sulla pipe il numero di ricorrenze
            write(piped[i][1], &count, 1);
            exit(argv[i+2][0]);
        }
    }

    //padre

    //chiudo i lati di scrittura    
    for (unsigned j = 0; j< N; j++) { 
        close(piped[j][1]); 
    } 

    //estraggo dalla pipe il numero e ne deduco il relativo carattere
    long int n_ric =0;
    for (unsigned k = 0; k <N ; k++){
        
        //leggo il numero
        read(piped[k][0], &n_ric, 1);
        printf("Il carattere %c è stato travato %ld volte nel file %s\n", argv[k+2][0], n_ric, argv[1]);
    }

    //aspetto tutti i figli
    for(unsigned i = 0; i < N; i++){
        if ((pidFiglio=wait(&status)) < 0)
        {
            printf("ERRORE - wait figlio del padre INDICE = %d\n", i);
            exit(3);
        }
        if ((status & 0xFF) != 0)
            printf("ERRORE - Figlio con PID = %d terminato in modo anomalo\n", pidFiglio);
        else
        {	
            int ret=(int)((status >> 8) & 0xFF);
            printf("Il Figlio con PID = %d ha ritornato il carattere assegnatogli in precedenza %c\n", pidFiglio, ret);
        }
    }
}