#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main ()
{
    char *argin[2]; //array da passare alla execvp che contiene i parametri: il primo il nome del file da eseguire, il secondo un array che punta al valore 0 (usato come terminatore di parametri, andava bene anche NULL)

	//se non è in PATH lo devo mettere assoluto se no non lo trova
	argin[0]="/home/matteo/Desktop/Virtual_machine_shared/ese8/es1/prova";           
	argin[1]= (char *)0;

    int x;

    //chiedo all'utente se vuole terminare l'esecuzione
	printf("Esecuzione di prova\n");
	printf("Dimmi se vuoi finire (valore 0)!\n");
	scanf("%d", &x);

    //controllo il valore preso in input
	if (x != 0)
	{
        //se diverso da 0 invoco di nuovo il programma prova
		printf("DEBUG-Eseguo di nuovo %s\n", argin[0]);
		int ret = execvp(argin[0], argin);

		// gestione dell'errore più preciso
    	if (ret == -1) {
			if (errno  ==  EACCES)
				printf("ERRORE- Permessi insufficienti per questo file\\n");
			else
				perror("execvp");
			exit(EXIT_FAILURE);
    	}

        //se procede qui vuol dire che ci sono stati errori (execvp ritorna -1 in caso di errore e scrive nella variabile errno)
		printf("Errore in execvp\n");
		exit(1);
	}
	else 	exit(x); //x sarà sicuramente 0
}