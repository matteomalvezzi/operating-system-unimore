#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

#define PERM 0644

int main (int argc, char** argv)
{

    int fd;
    int pid, pidFiglio, status;

    char* Fout;
    int fl;

    //controllo numero parametri
	if (argc < 4)
	{
        printf("ERRORE - il numero di parametri deve essere almeno 3: %d\n", argc);
        exit(1);	
	}

    //controllo parametri
    for(unsigned i = 1; i < argc; i++){
        if ((fd = open(argv[i], O_RDONLY)) < 0) {
            printf("ERRORE - il parametro %d : %s non è un nome di un file\n",i, argv[i]); 
            exit(2);
        }
    }

    //creo N processi
    int N = argc-1;
    for (unsigned i = 0; i < N; i++)
    {

        //fork e controllo fork
        if ((pid = fork()) < 0)
        {
            printf("ERRORE - fork\n");
            exit(3);
        }

         //figlio
        if (pid == 0){
            
            printf("DEBUG - nuovo processo figlio con PID = %d e INDICE = %d\n", getpid(), i);
            
            //creo la stringa del nuovo nome
            int FoutLenght = strlen(argv[i+1]) + 6;     //lunghezza nuovo nome = lunghezza parametro + .sort + terminatore
            Fout=malloc(sizeof(char) * FoutLenght);
            if (Fout == NULL) {
                printf("ERRORE - malloc\n");
                exit(-1);
            }

            //inserisco le varie stringhe
            strcpy(Fout, argv[i+1]);
            strcat(Fout, ".sort");

            //creo il file e lo chiudo
            fl = creat(Fout, PERM);
            if (fl < 0){
                printf("ERRORE - creat\n");
                exit(-1);
            }
            close(fl);

            //creo il processo nipote
            if ((pid = fork()) < 0)
            {
                printf("ERRORE - fork\n");
                exit(3);
            }

                //nipote
                if (pid == 0){
                    
                    printf("DEBUG - nuovo processo nipote con PID = %d figlio del processo di INDICE = %d", getpid(), i);

                    //chiudo lo standard input e lo sostituisco con il file da ordinare
                    close(0);
                    if ( open(argv[i+1], O_RDONLY ) < 0){
                        printf("ERRORE - apertura file da ordinare\n");
                        exit(-1);
                    }

                    //chiudo lo standard output e lo sostituisco con il file dove inserire l'output ordinato
                    close(1);
                    if ( open(Fout, O_WRONLY ) < 0){
                        printf("ERRORE - apertura file dove inserire il testo ordinato\n");
                        exit(-1);
                    }

                    //ordino
                    execlp("sort", "sort", (char *)0);

                    //in caso di errore
                    printf("ERRORE - in execvp\n");
		            exit(-1);
                }

                //figlio
                if ((pidFiglio=wait(&status)) < 0)
                {
                    printf("ERRORE - wait nipote\n");
                    exit(3);
                }
                if ((status & 0xFF) != 0)
                    printf("ERRORE - Nipote con PID = %d terminato in modo anomalo\n", pidFiglio);
                else
                {	
                    int ret=(int)((status >> 8) & 0xFF);
                    printf("Il nipote con PID = %d ha ritornato %d\n", pidFiglio, ret);
                    exit(ret);
                }	
        }

        //padre
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
            printf("Il Figlio con PID = %d ha ritornato %d\n", pidFiglio, ret);
        }	
    }
    exit(0);
}