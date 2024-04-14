#!/bin/sh

PATH=`pwd`:$PATH
export PATH

#identifico con il nome del processo i file temporanei in modo tale che siano unici nella directory /temp
echo 0 > /tmp/var$$

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
            if test $1 -ne 0 -a $1 -gt 0
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

                        #se è la prima gerarchia la esploro a parte
                        if test $count -eq 2
                        then
                            FCR.sh $G $X /tmp/var$$ /tmp/nomiAssoluti1 #esploriamo la gerarchia
                        else
                            FCR.sh $G $X /tmp/var$$ /tmp/nomiAssoluti_ #esploriamo la gerarchia
                        fi
                          
                        #per ogni gerarchia stampo il numero di file che soddisfano i requisiti richiesti
                        COUNTER=`cat /tmp/var$$`
                        echo "La gerarchia : $G ha esattamente $COUNTER file che hanno $X linee \n"

                        echo 0 > /tmp/var$$  #resetto il contatore
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

#finita l'esplorazione della gerarchia controllo i file uguali per ogni gerarchia

echo "===============CONTROLLO FILE UGUALI====================="

#itero tutti i file della prima gerarchia
for f1 in `cat < /tmp/nomiAssoluti1`
do
    #ne salvo il contenuto
    contenutof1=`cat $f1`

    #itero tutti i file di tutte le gerarchie esclusa la prima
    for fger in `cat < /tmp/nomiAssoluti_`
    do
        #ne salvo il contenuto
        contenutofger=`cat $fger`

        #ne confronto il contenuto
        if test "$contenutof1"="$contenutofger"
        then
            echo "I file $f1 e $fger sono uguali"
        fi
        

    done
done

echo "====================================\n"

rm /tmp/var$$
rm /tmp/nomiAssoluti1
rm /tmp/nomiAssoluti_

echo "==================TERMINE PROGRAMMA=================="