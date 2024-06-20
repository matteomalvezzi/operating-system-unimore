#!/bin/sh

#controllo numero di parametri
if test $# -ne 3
then
    echo ERRORE : numero di parametri $# non corretto : Usare $0 directory stringa stringa
    exit 1
else
    echo DEBUG : numero di parametri $# corretto
fi

#controllo primo parametro ( path assoluto che punta a una directory trasversabile)

# ---- G -----
case $1 in
    #path assoluto
    /*) 
        #controllo che o non è directory o non è traversabile
        if test ! -d $1 -o ! -x $1
        then
            #parametro non corretto
            echo ERRORE : \"$1\" parametro non corretto - non directory o non traversabile
            exit 2
        fi ;;

    #tutte le altre stringhe    
    *)  
        echo ERRORE : \"$1\" parametro non corretto - non è un nome assoluto
        exit 3
;;
esac

G=$1
shift

#controllo altri 2 parametri
for s
do
	case $s in
	*/*) 
        echo ERRORE : \"$s\" parametro non corretto - non è una stringa, contiene degli / non ammessi
	    exit 4;;
	*)
        echo DEBUG : \"$s\" parametro corretto
		;;
	esac

done

#controllo che le due stringhe non siano uguali
if test $1 = $2
then
    echo ERRORE : le due stringhe \"$1\" e \"$2\" sono uguali, questo non è ammesso
    exit 5
fi

E1=$1
E2=$2

#settiamo il PATH in modo tale che FCR sia raggiungibile
PATH=`pwd`:$PATH
export PATH

#chiamata al file ricorsivo
FCR.sh $G $E1 $E2

