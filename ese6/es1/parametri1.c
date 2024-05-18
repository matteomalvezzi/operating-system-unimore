/* file parametri1.c */

#include <stdio.h>
#include <stdlib.h>

int main  (int argc, char **argv)
{ 	

    //controllo che il numero di parametri sia corretto con un controllo LASCO (controllo lasco: ci deve essere almeno un parametro e quindi argc deve essere >= 2, altrimenti errore)
    if (argc < 2){
        printf("Errore: Necessario almeno 1 parametro per %s e invece argc=%d\n", argv[0], argc); 
        exit(1);
    }

    printf("Avviamo il programma %s avente %d parametri:\n", argv[0], argc);

    //stampo i parametri e come da richiesta ciclo da 0 a N-1 invece che da 1 a N
    int N = argc - 1;
    for(int i = 0; i < N; ++i){
        printf("Parametro numero %d : %s\n", i, argv[i+1]);
    }
    exit(0);
    

}