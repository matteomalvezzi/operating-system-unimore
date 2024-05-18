/* file parametri2.c */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int main (int argc, char **argv){

    int fd;
    int N;
    char C;

    // controllo che il numero di parametri sia corretto 3 + 1
    if (argc != 4) {
        printf("Errore: Necessari esattamente 3 parametri per %s e invece argc=%d\n", argv[0], argc); 
        exit(1);
    }

    //controllo che il primo parametro sia un nome di un file controllando che tale file esista nel file system
    if ((fd = open(argv[1], O_RDONLY)) < 0) {
        
        printf("Errore: il primo parametro %s deve essere il nome di un file\n", argv[1]); 
        exit(2);
    }

    //controllo che il secondo parametro sia un intero N strettamente positivo
    //o utilizzo atoi ma non gestisco il caso in cui la stringa non è un numero o strtol che mi gestisce questo caso ma poi ho un long da castare a int

    // ---- con strtol ------

    /*
    long L = strtol(argv[2], NULL, 10);
    if(L == 0 && errno == EINVAL){
        printf("Errore: Il secondo parametro %s non e' convertibile in numero\n", argv[2]); 
        exit(1);
    }else{
        if(L <= 0){
            printf("Errore: Il secondo parametro %s non e' un numero strettamente maggiore di 0\n", argv[2]); 
            exit(1); 
        }else{
            N = (int) L; //casto L in intero
        }
    };

    */

    // ---- con atoi ------
    N=atoi(argv[2]); //con atoi quando non riconosce il numero mette 0
	if (N <= 0 ) 
	{ 	
		printf("Errore: Il secondo parametro %s non e' un numero strettamente maggiore di 0\n", argv[2]); 
		exit(3); 
	}

    //controllo che il terzo parametro sia un singolo carattere
    if (strlen(argv[3]) != 1)	/* in alternativa si puo' anche verificare il carattere di posizione [1] della stringa argv[3]: se '\0' va bene, altrimenti errore! */
	{ 	
		printf("Errore: Il terzo parametro %s non e' un singolo carattere\n", argv[3]); 
		exit(4); 
	}
    C = argv[3][0];

    //stampo il nome del programma con i suoi parametri
    printf("Il programma %s ha esattamente %d parametri : \n", argv[0], argc-1);
    printf("Il primo parametro è il nome del file %s\nil secondo parametro è un intero strettamente positivo N = %d\ne l'ultimo parametro è il carattere %c\n", argv[1], N, C);

    //fine programma
    exit(0);
    
}