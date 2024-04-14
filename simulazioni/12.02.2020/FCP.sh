#!/bin/sh

PATH=`pwd`:$PATH
export PATH

#identifico con il nome del processo i file temporanei in modo tale che siano unici nella directory /temp
echo 0 > /tmp/var$$

X=0

echo "==================INIZIO PROGRAMMA=================="

if test $# -ge 4
then
    # ------- K1 -------
    #controllo che il primo sia intero strettamente positivo DISPARI
    case $1 in
        # Not numeric
        ''|*[!0-9]*)
            echo "ERRORE PARAMETRO: \"$1\" non è un valore numerico intero"
            exit
        ;; 
        
        # Numeric
        *)  
            mod=`expr $1 % 2`
            #controllo che sia strettamente positivo e dispari
            if test $1 -gt 0 -a $mod -eq 1
            then
                echo "PARAMETRO VALIDO : $1"
                K1=$1
            else
                echo "ERRORE PARAMETRO: \"$1\" deve essere strettamente maggiore di 0 e PARI"
                exit
            fi
        ;;
    esac

    # ------- K2 -------
    #controllo che il secondo sia intero strettamente positivo PARI
    case $2 in
        # Not numeric
        ''|*[!0-9]*)
            echo "ERRORE PARAMETRO: \"$2\" non è un valore numerico intero"
            exit
        ;; 
        
        # Numeric
        *)  
            mod=`expr $2 % 2`
            #controllo che sia strettamente positivo e dispari
            if test $2 -gt 0 -a $mod -eq 0
            then
                echo "PARAMETRO VALIDO : $2"
                K2=$2
            else
                echo "ERRORE PARAMETRO: \"$2\" deve essere strettamente maggiore di 0 e DISPARI"
                exit
            fi
        ;;
    esac

     # ------- W - G -------
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

                        FCR.sh $G $K1 $K2 /tmp/var$$ /tmp/nomiAssoluti
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

#finita l'esplorazione della gerarchia stampo le directory che soddisfano i requisiti
echo "================NUMERO DI DIRECTORY TROVATE====================\n"
#stampo il counter
COUNTER=`cat /tmp/var$$`        #get counter from file
echo "$COUNTER"                  #print it

echo "===============DIRECTORY CON REQUISITI====================="

#itero tutti i file della prima gerarchia
for d in `cat < /tmp/nomiAssoluti`
do
    echo $d
done

rm /tmp/var$$
rm /tmp/nomiAssoluti

echo "==================TERMINE PROGRAMMA=================="
