#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>

int main (int argc, char** argv)
{

    int fd;
    int pid;
    int pidFiglio, status;

    char *argin[2];
    argin[0]="/home/matteo/Desktop/Virtual_machine_shared/ese8/es5/mycat";           
	argin[1]= (char *)0;

    //controllo numero parametri
	if (argc != 2)
	{
        printf("ERRORE - il numero di parametri deve essere 1 (nome file da stampare): %d\n", argc);
        exit(1);	
	}

    //controllo parametro
    if ((fd = open(argv[1], O_RDONLY)) < 0) {
        printf("ERRORE - il primo parametro %s deve essere il nome di un file\n", argv[1]); 
        exit(2);
    }

    //fork e controllo fork
    if ((pid = fork()) < 0)
	{
		printf("ERRORE - fork\n");
		exit(3);
	}

    //figlio
    if (pid == 0){
        
        //chiudo lo standard input e redirecto sul mio file
        close(0);
        if (open(argv[1], O_RDONLY)){
            printf("ERRORE - apertura file %s\n", argv[1]);
            exit(-1);
        }

        //invoco il programma mycat
        execvp(argin[0], argin);

        //se procede qui vuol dire che ci sono stati errori (execvp ritorna -1 in caso di errore e scrive nella variabile errno)
		printf("ERRORE - in execvp\n");
		exit(-1);
    }

    //padre
    if ((pidFiglio=wait(&status)) < 0)
	{
        printf("ERRORE - wait\n");
        exit(3);
	}
	if ((status & 0xFF) != 0)
        printf("ERRORE - Figlio con PID = %d terminato in modo anomalo\n", pidFiglio);
	else
	{	
        int ret=(int)((status >> 8) & 0xFF);
        printf("Il figlio con PID = %d ha ritornato %d\n", pidFiglio, ret);
	}	

    exit(0);

}