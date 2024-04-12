#!/bin/sh

PATH=`pwd`:$PATH
export PATH

#identifico con il nome del processo i file temporanei in modo tale che siano unici nella directory /temp
echo 0 > /tmp/var$$
touch /tmp/nomiAssoluti

W=0
S=""

echo "==================INIZIO PROGRAMMA=================="

if test $# -ge 4
then

    # ------- W -------
    #controllo che il primo sia intero strettamente positivo
    case $1 in
        # Not numeric
        ''|*[!0-9]*)
            echo "ERRORE PARAMETRO: \"$1\" non è un valore numerico intero"
            exit
        ;; 
        
        # Numeric
        *)  
            #controllo che sia positivo e diverso da 0
            if test $1 -ne 0 -a $1 -gt 0
            then
                echo "PARAMETRO VALIDO : $1"
                W=$1
            else
                echo "ERRORE PARAMETRO: \"$1\" deve essere strettamente maggiore di 0"
                exit
            fi
        ;;
    esac


    # ------- S -------
    #controllo che il secondo parametro sia una normale stringa
    case $2 in
    */*)
        #controllo che la stringa non sia un path
        echo "ERRORE PARAMETRO: \"$2\" è un path e non una semplice stringa"
        echo "USARE: $0 numero stringa path ... path"
        exit
    ;;

    *)  
        #controllo che la stringa non sia vuota
        if test "$2"
        then
            S=$2
        else
            echo "ERRORE PARAMETRO: \"$2\" è una stringa vuota"
            exit
        fi
    ;;
    esac


    # ------- Q - G -------
    count=1
    #scorro tutti i parametri
    for G in $*
    do
        #se il contatore è maggiore di 2 allora ci sono solo path
        if test $count -gt 2
        then
            #controllo che sia un path assoluto che punta a una directory trasversabile
            case $G in
            #path assoluto
            /*) 
                #controllo che sia una directory
                if test -d $G -a -x $G
                    #directory
                    then
                        echo "PARAMETRO VALIDO : $G"
                        FCR.sh $G $S /tmp/var$$ /tmp/nomiAssoluti #esploriamo la gerarchia
                else
                    #not a directory
                    echo "ERRORE PARAMETRO: \"$G\" non è una directory trasversabile"
                    exit      
                fi
            ;;
            #tutte le altre stringhe
            *)
                echo "ERRORE PARAMETRO: \"$G\" non è un path assoluto"
                exit
            ;;
            esac

        fi
        count=`expr $count + 1`
    done

fi

echo "==================FINE RICORSIONE=================="

#stampo il counter
COUNTER=`cat /tmp/var$$`        #get counter from file
echo "Numero di directory che contengono un file di nome $S : $COUNTER"     #print it

#se il counter è maggiore di W stampo la X-esima directory
if test $COUNTER -ge $W
then
    echo "Matteo, inserisci il numero della directory da visualizzare (compreso tra 1 e $W) : "
    #leggo la scelta dell'utente
    read X

    case $X in
        # Not numeric
        ''|*[!0-9]*)
            echo "ERRORE: \"$X\" non è un valore numerico intero"
            exit
        ;; 
        
        # Numeric
        *)  
            #controllo che sia compreso tra 1 e W
            if test $X -ge 1 -a $X -le $W
            then
                echo "PARAMETRO VALIDO : $X"
                #visualizzo la X-esima directory
                echo "Ecco la directory numero $X: \n"
                head -$X /tmp/nomiAssoluti | tail -1
            else
                echo "ERRORE: \"$X\" non è compreso tra 1 e $W"
                exit
            fi
        ;;
    esac
fi

rm /tmp/var$$
rm /tmp/nomiAssoluti

echo "==================TERMINE PROGRAMMA=================="