#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

int main (int argc, char** argv)
{

    int fd;

    int pid, pidFiglio, status;

    int piped[2][2];        //array di pipe : pipe1 processi pari,  pipe2 processi dispari 

    char ch;
    int ch1, ch2;

    //controllo numero parametri
	if (argc < 3)
	{
        printf("ERRORE - il numero di parametri deve essere almeno 2: %d\n", argc);
        exit(1);	
	}

    //controllo parametri
    for(unsigned i = 1; i < argc; i++){
        if ((fd = open(argv[i], O_RDONLY)) < 0) {
            printf("ERRORE - il parametro %d : %s non è un nome di un file\n",i, argv[i]); 
            exit(2);
        }
    }

    //creo le pipe ( ATTENZIONE: Vanno create separatamente)
    if (pipe(piped[0]) < 0) {
        printf("ERRORE - creazione pipe numeri pari\n");
        exit(2);
    }
    if (pipe(piped[1]) < 0) {
        printf("ERRORE - creazione pipe numeri dispari\n");
        exit(3);
    }

    int N = argc-1;
    //genero i processo
    for(unsigned i =0; i < N; i++){

        //fork e controllo fork
        if ((pid = fork()) < 0)
        {
            printf("ERRORE - fork\n");
            exit(3);
        }

        //figlio
        if (pid == 0){

            //chiudo le pipe di lettura
            close(piped[0][0]);
            close(piped[1][0]);

            //apro il file
            if ((fd = open(argv[i+1], O_RDONLY)) < 0)
            {   
                printf("ERRORE - in apertura del file %s\n", argv[i+1]);
                exit(-1);
            }
            
            //controllo pari o dispari
            if((i+1)%2==0){
                //pari
                
                //estraggo i caratteri numerici
                while (read(fd, &ch, 1) > 0) 
                {   
                    if(isdigit(ch))
                        write(piped[0][1], &ch, 1);
                }
            }else{
                //dispari

                //estraggo i caratteri alfanumerici
                while (read(fd, &ch, 1) > 0) 
                {  
                    if(isalpha(ch))
                        write(piped[1][1], &ch, 1);  
                }
            }
            exit(ch);
        }
    }

    //padre

    //chiudo i lati di scrittura
    close(piped[0][1]);
    close(piped[1][1]);

    //-------- Versione 1 (caratteri mischiati in base a come arrivato sulla pipe) ---------

    //estraggo i caratteri inviati dai figli
    int rd1 = read(piped[0][0], &ch1, 1);       //leggo dalla pipe pari
    int rd2 = read(piped[1][0], &ch2, 1);       //leggo dalla pipe dispari

    int counter = 0;
    //visto che non so se i caratteri numerici sono più o meno dei caratteri alfanumerici fin che c'è ne' anche solo uno dei due li stampo.
    //questo perchè i caratteri alphanumerici vengano mischiati con i numerici
    //uso la write e non la printf così posso mettere come numero di caratteri stampati rd1 e rd2 così se non ce ne sono più non ne stampa visto che sarebbero a 0
    
    while ((rd1 != 0) || (rd2 != 0))
    {
        counter = counter + rd1 + rd2;
        write(1, &ch1, rd1);
        write(1, &ch2, rd2);
        rd1 = read(piped[0][0], &ch1, 1);
        rd2 = read(piped[1][0], &ch2, 1);
    }

    printf("\nNumero di caratteri trovati e stampati: %d\n", counter);

    //-------- Versione 2 (stampo prima gli alfanumerici e poi i numerici (avrò prima tutti i numeri e poi tutti i caratteri)) ---------
    // int counter = 0;
    // while (read(piped[0][0], &ch, 1))
    // {
    //     counter++;
    //     write(1, &ch, 1);
    // }
    // while (read(piped[1][0], &ch, 1))
    // {
    //     counter++;
    //     write(1, &ch, 1);
    // }
    // printf("\nNumero di caratteri trovati e stampati: %d\n", counter);

    //aspetto i figli

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
            printf("Il Figlio con PID = %d ha ritornato il carattere %c (in decimale = %d)\n", pidFiglio, ret, ret);
        }
    }
}