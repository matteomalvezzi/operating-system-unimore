#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#define MSGSIZE 5 

int main (int argc, char **argv)
{   
   int pid, j, piped[2];

   char mess[MSGSIZE];					//array per l'estrazione delle stringhe
   int pidFiglio, status, ritorno;  	 //wai padre

    //controllo numero parametri
	if (argc != 2)
	{	
        printf("ERRORE - Numero dei parametri errato %d: ci vuole un singolo parametro\n", argc);
    	exit(1);
	}

	//creo la pipe
	if (pipe(piped) < 0 )  
	{   
        printf("ERRORE - creazione pipe\n");
    	exit(2); 
	}

	//faccio la fork e controllo
	if ((pid = fork()) < 0)
	{   	
        printf("ERRORE - creazione figlio\n");
    	exit(3); 
	}

    //figlio
	if (pid == 0)  
	{   
		int fd;     //questa variabile sarà presente solo nello stack del figlio

		close(piped[0]); 	//il figlio CHIUDE il lato di lettura: sara' quindi lo SCRITTORE della pipe

        //apro il file passato come parametro
		if ((fd = open(argv[1], O_RDONLY)) < 0)
		{   
            printf("ERRORE - in apertura del file %s\n", argv[1]);
            exit(-1);       //nel figlio exit sempre -1 così verrà visto come errore (poi 255)
		}

		printf("DEBUG - figlio con PID = %d sta per iniziare a scrivere una serie di messaggi, ognuno di lunghezza %d, sulla pipe dopo averli letti dal file passato come parametro\n", getpid(), MSGSIZE);

		j=0;       //conto quante stringhe leggo

        //leggo il file
		while (read(fd, mess, MSGSIZE))     //leggo 4 caratteri + 1 terminatore di linea
		{   
            //mandiamo al padre solo stringhe quindi sostituisco il terminatore di linea con quello di stringhe
            mess[MSGSIZE-1]='\0'; 
            
            //scrivo sulla pipe
            write(piped[1], mess, MSGSIZE);
            j++;
        }
		printf("DEBUG - figlio con PID = %d ha scritto %d messaggi sulla pipe\n", getpid(), j);
		exit(j);
	}

	//padre
	close(piped[1]);  //il padre CHIUDE il lato di scrittura: sara' quindi il LETTORE della pipe	
	
    //ATTENZIONE:
    /*
        La mancata chiusura dei lati della pipeline porta a un deadlock

        questo perchè in Unix la lettura da una pipeline termina solo quando ha letto tutti i dati e il lato di scrittura è stato chiuso (entrambe le condizioni devono essere soddisfatte)
        lo stesso vale per la scrittura che non termina fino a che la lettura non viene chiusa e fino a quando non si è letto tutto

        il padre allora aspetta che la scrittura venga chiusa, ma questo non accade mai, allo stesso modo il figlio non chiude la lettura e
        impedisce al padre di terminare la lettura.
    */

    printf("DEBUG - padre con PID = %d sta per iniziare a leggere i messaggi dalla pipe\n", getpid());
	j=0;
	while (read(piped[0], mess, MSGSIZE))  /* questo ciclo avra' termine appena il figlio terminera' dato che la read senza piu' scrittore tornera' 0! */
	{ 	
        /* dato che il figlio gli ha inviato delle stringhe, il padre le puo' scrivere direttamente con una printf */
        printf("%d: %s\n", j, mess);
        j++;
	}
	printf("DEBUG - padre con PID = %d ha letto %d messaggi dalla pipe\n", getpid(), j);

	//padre aspetta il figlio per il valore di ritorno
	if ((pidFiglio=wait(&status)) < 0)
	{
        printf("ERRORE - wait\n");
        exit(4);
	}
	if ((status & 0xFF) != 0)
        printf("ERRORE - figlio con pid %d terminato in modo anomalo\n", pidFiglio);
	else
	{
       	ritorno=(int)((status >> 8) & 0xFF);
       	if (ritorno != 255){
            printf("DEBUG - il figlio con pid = %d ha ritornato %d \n", pidFiglio, ritorno);
        }else{
            printf("ERRORE - il figlio con pid = %d non è riuscito ad aprire il file infatti ha ritornato %d\n", pidFiglio, ritorno);
        }
	}

	exit(0);
}
