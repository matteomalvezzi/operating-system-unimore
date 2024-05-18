/* FILE: status1.c */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> 

int getValore(){
	int valore;
	printf("Inserisci un valore numero :\n");
	scanf("%d", &valore);
	if(valore > 255 || valore <0 ) printf("il valore inserito sarà troncato\n");
	else printf("il valore inserito non sarà troncato\n");
	return valore;
}

int main ()
{
     int pid; 				/* pid per fork */
     int pidFiglio, status, ritorno;    /* per wait padre */

	if ((pid = fork()) < 0)
	{ 	/* fork fallita */
		printf("Errore in fork\n");
		exit(1);
	}

	if (pid == 0)
	{ 	/* figlio */
		printf("Esecuzione del figlio\n");
	
		int ret = getValore();
		exit(ret);	/* si torna un valore che si suppone possa essere derivante dall'esecuzione di un compito assegnato al figlio */
	}

	/* padre */
	printf("Generato figlio con PID = %d\n", pid);

	/* il padre aspetta il figlio in questo caso interessandosi del valore della exit del figlio */
	if ((pidFiglio=wait(&status)) < 0)
	{
		printf("Errore in wait\n");
		exit(2);
	}

	if (pid == pidFiglio)
        	printf("Terminato figlio con PID = %d\n", pidFiglio);
	else
        {       /* problemi */
                printf("Il pid della wait non corrisponde al pid della fork!\n");
                exit(3);
        }

	if ((status & 0xFF) != 0)
    		printf("Figlio terminato in modo involontario (cioe' anomalo)\n");
    	else
    	{
    		ritorno = status >> 8;
    		ritorno &= 0xFF;
    		printf("Il figlo con PID = %d ha returnato il valore = %d\n", pid, ritorno);
    	}

	exit(0);
}
