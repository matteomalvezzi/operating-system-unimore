#!/bin/sh

# -------- note e codice alternativo per script sh --------

#controllo numero di parametri (con if)
if test $# -lt 3
then
    echo ERRORE : numero di parametri $# insufficiente : Usare $0 integere file ... file
    exit 1
else
    echo DEBUG : numero di parametri $# corretto
fi


#controllo altri parametri (senza shifting, quindi con un contatore, e con controlli invertiti)
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
                    echo "PARAMETRO VALIDO : \"$G\""
            else
                #not a directory
                echo "ERRORE: \"$G\" parametro non corretto - non directory o non traversabile"
                exit 3    
            fi
        ;;
        #tutte le altre stringhe
        *)
            echo "ERRORE PARAMETRO: \"$G\" parametro non corretto - non è un nome assoluto"
            exit 4
        ;;
        esac

    fi
    count=`expr $count + 1`
done

