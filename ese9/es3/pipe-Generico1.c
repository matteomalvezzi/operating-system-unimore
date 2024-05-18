#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h> 


/*
Versione 1 pipeline.

scrivo all'inizio la lunghezza,
allora nel read leggo il primo numero che mi da
la lunghezza della linea, e con la lunghezza della linea
(che comprende il terminatore) posso
direttamente estrarre dalla pipeline la linea visto che ho la lunghezza
*/

int main (int argc, char **argv)
{   
   int pid, piped[2];
   int j, l;

   char mess[512];
   int pidFiglio, status, ritorno;      /* per wait padre */

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

		printf("DEBUG - figlio con PID = %d sta per iniziare a scrivere una serie di stringhe di lunghezza diversa, sulla pipe dopo averli letti dal file passato come parametro\n", getpid());

		j=0;    //numero di linee
        l=0;    //lunghezza linea

        //leggo il file
		while (read(fd, &(mess[l]), 1))
		{

            if(mess[l] == '\n'){        //fine linea
                
                //mandiamo solo stringhe e non linee quindi mettiamo il terminatore di stringa
                mess[l]='\0';
                l++;     //considerando il terminatore aumento di 1 la lunghezza

                //scrivo sulla pipe la lunghezza
                write(piped[1], &l, sizeof(l));

                //scivo sulla pipe la stringa
                write(piped[1], mess, l);

                //resetto il contatore di caratteri
                l=0;
                //incremento il numero di linee    
                j++;
            }else l++;
        }
		printf("DEBUG - figlio con PID = %d ha scritto %d linee sulla pipe con le relative lunghezze\n", getpid(), j);
		exit(j);
	}

	//padre
	close(piped[1]);  //il padre CHIUDE il lato di scrittura: sara' quindi il LETTORE della pipe	

    printf("DEBUG - padre con PID = %d sta per iniziare a leggere i messaggi dalla pipe\n", getpid());
	j=0;

    //leggo la lunghezza della linea
	while (read(piped[0], &l, sizeof(l)))
	{ 	
        //leggo la linea di lunghezza l
        read(piped[0], mess, l);

        //visto che sono stringhe le metto direttamente nella printf
        printf("%d: %s\n", j, mess);
        j++;
	}
	printf("DEBUG - padre con PID = %d ha letto %d stringhe dalla pipe\n", getpid(), j);

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
