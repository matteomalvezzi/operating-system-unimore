#!/bin/sh

PATH=`pwd`:$PATH
export PATH

#identifico con il nome del processo i file temporanei in modo tale che siano unici nella directory /temp
touch /tmp/nomiAssoluti$$

X=0

echo "==================INIZIO PROGRAMMA=================="

if test $# -ge 3
then

    # ------- X -------
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
            if test $1 -gt 0
            then
                echo "PARAMETRO VALIDO : $1"
                X=$1
            else
                echo "ERRORE PARAMETRO: \"$1\" deve essere strettamente maggiore di 0"
                exit
            fi
        ;;
    esac

    # ------- Q - G -------
    count=1
    #scorro tutti i parametri
    for G in $*
    do
        #se il contatore è maggiore di 1 allora ci sono solo path
        if test $count -gt 1
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

                        FCR.sh $G $X /tmp/nomiAssoluti$$ #esploriamo la gerarchia
                else
                    #not a directory
                    echo "ERRORE PARAMETRO: \"$G\" non è una directory a cui posso accedere"
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

#finita l'esplorazione della gerarchia stampo le directory che ho trovato

echo "===============DIRECTORY CON LIVELLO PARI====================="

echo "Ecco le directory il quale livello è pari : \n"

#itero tutti i file trovati
count=1
for d in `cat < /tmp/nomiAssoluti$$`
do
    echo "$count : $d"

    #### a questo punto si dovrebbe invocare la parte C passando tutti i file nella directory
    
    count=`expr $count + 1`
done

echo "====================================\n"

rm /tmp/nomiAssoluti$$

echo "==================TERMINE PROGRAMMA=================="