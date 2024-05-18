/* file contaOccorrenze.c */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>


int main(int argc, char **argv)
{
    int F;
    char Cx;
    long int tot=0;

    // controllo che il numero di parametri sia corretto 2 + 1
    if (argc != 3) {
        printf("Errore: Necessari esattamente 2 parametri per %s e invece argc=%d\n", argv[0], argc); 
        exit(1);
    }

    //controllo che il primo parametro sia un nome di un file controllando che tale file esista nel file system
    if ((F = open(argv[1], O_RDONLY)) < 0) {
        printf("Errore: il primo parametro %s deve essere il nome di un file\n", argv[1]); 
        exit(2);
    }

    //controllo che il secondo parametro sia un singolo carattere
    if (strlen(argv[2]) != 1)
	{ 	
		printf("Errore: Il terzo parametro %s non e' un singolo carattere\n", argv[3]); 
		exit(4); 
	}
    Cx = argv[2][0];

     //stampo il nome del programma con i suoi parametri
    printf("Il programma %s ha esattamente %d parametri : \n", argv[0], argc-1);
    printf("Il primo parametro è il nome del file %s\nil secondo parametro è un carattere = %c\n", argv[1], Cx);


    //leggiamo un carattere alla volta e lo confrontiamo con quello che abbiamo
    char c; //in c mettiamo di volta in volta il carattere letto 
    while(read(F, &c, 1)){
        if(c == Cx) tot++;
    }

    printf("Il file %s contiene il carattere %c esattamente %ld volte\n", argv[1], Cx, tot);
    exit(0);
}