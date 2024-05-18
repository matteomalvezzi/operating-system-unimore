#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

/* ----- Versione 2
    Utilizziamo una sola pipe
    e ci scriviamo dentro una struttura composta da:
    1 carattere e il numero di ricorrenze di quel carattere
*/

typedef struct 	{
	char c;
	long int n;
} tipoS;

int main (int argc, char** argv)
{
    int fd;
    int N;
    int piped[2];

    tipoS msg;

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

    //creo la pipe
    if(pipe(piped) < 0) {
        printf("ERRORE - creazione della pipe\n"); 
        exit(4);
    } 

    //genero i N processi
    for(unsigned i =0; i < N; i++){

        //fork e controllo fork
        if ((pid = fork()) < 0)
        {
            printf("ERRORE - fork\n");
            exit(5);
        }

        //figlio
        if (pid == 0){

            //chiudo la pipe di lettura
            close(piped[0]);

            //apro il file
            if ((fd = open(argv[1], O_RDONLY)) < 0)
            {   
                printf("ERRORE - in apertura del file %s\n", argv[1]);
                exit(-1);
            }

            //inizializzo la struttura
            msg.c = argv[i+2][0];
            msg.n = 0L;
            
            //conto le ricorrenze
            while (read(fd, &ch, 1) > 0) 
            {   
                if(ch == argv[i+2][0])
                    msg.n++; 
            }
            //scrivo sulla pipe il numero di ricorrenze
            write(piped[1], &msg, sizeof(msg));
            exit(argv[i+2][0]);
        }
    }

    //padre

    //chiudo la scrittura della pipe
    close(piped[1]);

    //estraggo dalla pipe la struttura
    tipoS msg_ric;
    while(read(piped[0], &msg_ric, sizeof(tipoS))){
        
        printf("Il carattere %c è stato travato %ld volte nel file %s\n", msg_ric.c, msg_ric.n, argv[1]);
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