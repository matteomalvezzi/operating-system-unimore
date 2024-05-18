#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main (int argc, char** argv)
{  	
    int piped[2]; 	/* array di due interi per la pipe, il primo sarà file per la lettura e il secondo per la scrittura */

    int fd1;
    int fd2;

    // controllo numero parametri
    if (argc < 3) {
        printf("ERRORE - Necessari esattamente 2 parametri\n"); 
        exit(1);
    }

    //controllo parametri e apertura file
    if ((fd1 = open(argv[1], O_RDONLY)) < 0) {
        printf("ERRORE - Il primo parametro non è un nome di un file\n"); 
        exit(2);
    }
    if ((fd2 = open(argv[2], O_RDONLY)) < 0) {
        printf("ERRORE - Il secondo parametro non è un nome di un file\n"); 
        exit(3);
    }
    
    //chiudo il file che sta in 3
    close(fd1);

	//creo la pipe
	if (pipe (piped) < 0 ) 
	{ 
        printf("ERRORE - creazione pipe\n");
		exit (4); 
	}

    //visto che fd1 è stato chiuso mi aspetto che la lettura della pipe sia in 3 e la scrittura in 5 (in 4 ci sta la fd2)
	printf("Creata pipe con piped[0]= %d \n", piped[0]);
	printf("Creata pipe con piped[1]= %d \n", piped[1]);
	exit(0);
}