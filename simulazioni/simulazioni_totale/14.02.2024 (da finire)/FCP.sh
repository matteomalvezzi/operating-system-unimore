#!/bin/sh

#controllo numero di parametri
if test $# -lt 4
then
    echo ERRORE : numero di parametri $# non corretto : Usare almeno 4 parametri
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

#controllo secondo parametro (numero intero strettamente positivo)

# ---- X -----
case $2 in
    *[!0-9]*)
        echo "ERRORE : parametro non corretto - il secondo parametro deve essere un intero > 0"
        exit 4
        ;;
    *)
        if test $2 -eq 0
        then
            echo "ERRORE : parametro non corretto - il secondo parametro deve essere != da 0"
            exit 5
        else
            echo DEBUG : secondo parametro $2 corretto
        fi
        ;;
esac
X=$2

shift
shift

# ---- S -----
for S
do
	case $S in
    ?)
        echo ERRORE : \"$S\" parametro non corretto - non sono ammessi caratteri, lunghezza stringa almeno due caratteri
        exit 6;;
    */*) 
        echo ERRORE : \"$S\" parametro non corretto - non è una stringa, contiene degli / non ammessi
	    exit 7;;
	*)
        echo DEBUG : \"$S\" parametro corretto
		;;
	esac
done


#settiamo il PATH in modo tale che FCR sia raggiungibile
PATH=`pwd`:$PATH
export PATH

FCR.sh $G $X $*