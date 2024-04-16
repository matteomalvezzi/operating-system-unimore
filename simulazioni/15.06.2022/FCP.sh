#!/bin/sh

PATH=`pwd`:$PATH
export PATH

#identifico con il nome del processo i file temporanei in modo tale che siano unici nella directory /temp
touch /tmp/nomiAssoluti
echo 0 > /tmp/var$$

X=0

echo "==================INIZIO PROGRAMMA=================="

if test $# -ge 3
then

    # ------- Q - G -------
    #scorro tutti i parametri
    for G in $*
    do
        #controllo che sia un path assoluto che punta a una directory trasversabile
        case $G in
        #path assoluto
        /*) 
            #controllo che sia una directory
            if test -d $G -a -x $G
                #directory
                then
                    echo "PARAMETRO VALIDO : $G"

                    FCR.sh $G /tmp/var$$ /tmp/nomiAssoluti
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
    done

fi

#finita l'esplorazione della gerarchia stampo il contatore e i file che ho trovato

echo "===============OUTPUT DEI FILE====================="

#stampo il counter
N=`cat /tmp/var$$`        #get counter from file
echo "Numero di file che soddisfano i requisiti : $N"     #print it

echo "Ecco i file che soddisfano i requisiti : \n"

#itero tutti i file trovati
count=1
for f in `cat < /tmp/nomiAssoluti`
do
    echo "$count : $f"
    count=`expr $count + 1`
done

echo "====================================\n"

rm /tmp/var$$
rm /tmp/nomiAssoluti

echo "==================TERMINE PROGRAMMA=================="