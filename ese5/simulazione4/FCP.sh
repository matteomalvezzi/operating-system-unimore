#!/bin/sh

PATH=`pwd`:$PATH
export PATH

#identifico con il nome del processo i file temporanei in modo tale che siano unici nella directory /temp
echo 0 > /tmp/var$$
touch /tmp/nomiAssoluti

C=""

#controllo che i parametri siano maggiori o uguali a 2
if test $# -ge 2
then
    
    count=1
    #scorro tutti i parametri
    for i in $*
    do
        #se il contatore è = a 1 allora è il primo parametro
        if test $count -eq 1
        then
            case $i in
            #singolo carattere
            ?)  
                echo "PARAMETRO CORRETTO: il primo parametro è un carattere"
                C=$i
            ;;
            #carattere non valido
            *)
                echo "ERRORE PARAMETRI: \"$i\" non è un carattere"
                exit 2
            ;;
            esac

        #altrimenti sono gli altri parametri
        else
            #controllo che sia un path assoluto che punta a una directory trasversabile
            case $i in
            #path assoluto
            /*) 
                #controllo che sia una directory
                if test -d $i -a -x $i
                    #directory
                    then
                        echo "PARAMETRO VALIDO : $i"
                        FCR.sh $i $C /tmp/var$$ /tmp/nomiAssoluti #esploriamo la gerarchia
                else
                    #not a directory
                    echo "ERRORE PARAMETRO: \"$i\" non è una directory trasversabile"
                    exit      
                fi
            ;;
            #tutte le altre stringhe
            *)
                echo "ERRORE PARAMETRO: \"$i\" non è un path assoluto"
                exit
            ;;
            esac
        fi
        count=`expr $count + 1`
    done

#in caso contrario stampo un errore
else
    echo "ERRORE PARAMETRI: parametri insufficienti"
fi

#stampo il counter
COUNTER=`cat /tmp/var$$`        #get counter from file
echo "Numero di directory che rispetto i REQUISITI : $COUNTER"     #print it

#mostro una directory alla volta
echo "\n"
echo "Ecco le directory trovate: "

#itero le righe del file visto che per ogni riga c'è una directory
for riga in `cat /tmp/nomiAssoluti`
do
    echo $riga
    echo "Elena, vuoi visualizzare il contenuto di questa directory (compresi i suoi file nascosti?)(Y-y-ok/no)"
    #leggo la scelta dell'utente
    read scelta

    case $scelta in

    Y*|y*|s*|S*|ok*)
        echo "\n"
        echo "Ecco il contenuto: "
        ls -al $riga
        echo "\n"
    ;;

    *)
        echo "Si è scelto di non visualizzarne il contenuto"
    ;;
    esac


done

rm /tmp/var$$
rm /tmp/nomiAssoluti