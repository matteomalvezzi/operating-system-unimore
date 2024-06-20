#!/bin/sh

#controllo numero di parametri
if test $# -lt 3
then
    echo ERRORE : numero di parametri insufficiente $# - inserire almeno 3 parametri
    exit 1
else
    echo DEBUG : numero di parametri $# corretto
fi

#controllo primo parametro (numero intero strettamente positivo)

# ---- X -----
case $1 in
    *[!0-9]*)
        echo "ERRORE : parametro non corretto - il primo parametro deve essere un intero > 0"
        exit 2
        ;;
    *)
        if test $1 -eq 0
        then
            echo "ERRORE : parametro non corretto - il primo parametro deve essere != da 0"
            exit 3
        else
            echo DEBUG : primo parametro $1 corretto
        fi
        ;;
esac
X=$1

#controllo altri parametri
shift

#controllo gerarchie, path assoluti di directory traversabili

# ---- G -----
for G
do
    case $G in
    /*) 
        #controllo che o non è directory o non è traversabile
        if test ! -d $G -o ! -x $G
        then
            #parametro non corretto
            echo ERRORE : \"$G\" parametro non corretto - non directory o non traversabile
            exit 4
        else
            echo DEBUG : \"$G\" parametro corretto
        fi 
    ;;

    #tutte le altre stringhe    
    *)  
        echo ERRORE : \"$G\" parametro non corretto - non è un nome assoluto
        exit 5
    ;;
    esac

done

#settiamo il PATH in modo tale che FCR sia raggiungibile
PATH=`pwd`:$PATH
export PATH

#chiamata al file ricorsivo
for G
do
    FCR.sh $G $X 0
done

