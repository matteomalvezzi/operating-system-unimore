#!/bin/sh

PATH=`pwd`:$PATH
export PATH

#identifico con il nome del processo i file temporanei in modo tale che siano unici nella directory /temp
echo 0 > /tmp/var$$
touch /tmp/file$$


#controllo che il numero di parametri sia corretto
if test $# -ge 2
then

    last=""
    count=1

    #estraggo l'ultimo parametro ovvero la stringa
    for i in $*
    do
        #considero solo l'ultimo parametro
        if test $count -eq $#
        then
            #controllo che sia una normale stringa
            case $i in
            */*)
                echo "ERRORE PARAMETRO: \"$i\" è un path e non una semplice stringa"
                echo "USARE: $0 path path ... stringa"
                exit
            ;;

            *)  
                #controllo che la stringa non sia vuota
                if test "$i"
                then
                    last=$i
                else
                    echo "ERRORE PARAMETRO: \"$i\" è una stringa vuota"
                    exit
                    
                fi
            ;;
            esac
        fi
        count=`expr $count + 1`
    done

    count=1
    #scorro tutti i parametri con un contatore che mi segnala quando è l'ultimo
    for i in $*
    do
        #se non è l'ultimo deve essere una directory
        if test $count -ne $#
        then
            #controllo che sia un nome assoluto
            case $i in
                /*) 
                    #controllo che sia una directory
                    if test -d $i
                        #directory
                        then
                            echo "PARAMETRO VALIDO : $i"
                            FCR.sh $i $last /tmp/file$$ /tmp/var$$ #esploriamo la gerarchia
                    else
                        #not a directory
                        echo "ERRORE PARAMETRO: \"$i\" non è una directory"
                        exit      
                    fi
                ;;

                *) #parametro non valido : path relativo
                    echo "ERRORE PARAMETRO: \"$i\" non è un path assoluto"
                    exit
                ;;
                esac
        fi
        count=`expr $count + 1`
    done
else
    echo "ERRORE PARAMETRI: parametri insufficienti"
fi

#stampo il counter
COUNTER=`cat /tmp/var$$`      #get counter from file
echo "Contatore : $COUNTER"  #print it

echo "\n"
echo "Ecco i file trovati: \n"
cat /tmp/file$$

echo "Visualizzare il file ordinato?(Y-y-ok/no)"
read scelta

case $scelta in

Y*|y*|s*|S*|ok*)
    echo "VISUALIZZO IL FILE ORDINATO: ecco il file ordinato : \n"
    sort -f /tmp/file$$
;;

*)
    echo "Si è scelto di non visualizzare il file in maniera ordinata"
;;
esac

rm /tmp/var$$
rm /tmp/file$$



