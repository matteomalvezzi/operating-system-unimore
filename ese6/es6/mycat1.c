#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void printfile(int fd){
    //stampo il contenuto di un file
    int nread;              //numero caratteri letti
    char buffer[BUFSIZ];    //caratteri letti

    while ((nread = read(fd, buffer, BUFSIZ)) > 0 )
        write(1, buffer, nread);
    return;
}

int main(int argc, char **argv)
{
    int fd = 0;

    if (argc < 2) printfile(fd);        //se non ho parametri stampo il file passando fd = 0 (così da utilizzare lo standard input)

    for (int i = 1; i < argc; i++){   //se ho file li scorro, li apro, e li stampo

        fd = open(argv[i], O_RDONLY);
        if (fd < 0){
            printf("Errore in apertura file %s\n", argv[i]);
            exit(1); 
		}
        printfile(fd);
	
    }
	exit(0);
}