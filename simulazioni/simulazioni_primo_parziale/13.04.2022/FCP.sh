#!/bin/sh

#controllo numero di parametri (con case)
case $# in
0|1|2)	echo ERRORE : numero di parametri $# insufficiente : Usare $0 integere file ... file
	    exit 1 ;;

*) 	echo DEBUG : numero di parametri $# corretto ;;
esac

#controllo primo parametro

# ---- X -----
case $1 in
*[!0-9]*)   echo ERRORE : $1 parametro non corretto - non numerico o non positivo
	        exit 2 ;;
*)  echo primo parametro giusto $1;;
esac

X=$1

#controllo altri parametri (shiftando il primo parametro e iterndo implicitamente i parametri, quindi senza mettere for G in $*, in $* lo mette lui)
shift

# ---- G -----
for G
do
    #controllo che sia un path assoluto che punta a una directory trasversabile
	case $G in

    #path assoluto
	/*) 
        #controllo che o non è directory o non è traversabile
        if test ! -d $G -o ! -x $G
	    then
            #parametro non corretto
	        echo ERRORE : \"$G\" parametro non corretto - non directory o non traversabile
	        exit 3
	    fi ;;

    #tutte le altre stringhe    
	*)  
        echo ERRORE : \"$G\" parametro non corretto - non è un nome assoluto
        exit 4
    ;;
	esac
done

#settiamo il PATH in modo tale che FCR sia raggiungibile
PATH=`pwd`:$PATH
export PATH

#contatore
count=1

# ---- Q -----
for G
do
    #file temporaneo
	touch /tmp/nomiAssoluti$$-$count
    
	echo DEBUG : fase $count per \"$G\"


	FCR.sh $G $X /tmp/nomiAssoluti$$-$count
	
    #la specifica indicava che doveva essere riportato su standard output il conteggio di OGNI SINGOLA gerarchia!
	echo Ecco il numero di file trovati per la fase $count : `wc -l < /tmp/nomiAssoluti$$-$count`
	
    count=`expr $count + 1`
done

#fine ricorsione
for fileGer1 in `cat /tmp/nomiAssoluti$$-1`
do
    count=1

    #itero le gerarchie (ovvero i parametri dopo lo shifting)
    for G
    do
        #escludo la prima gerarchia
        if test $count -gt 1
        then
            #controllo tutti i file dell gerarchia N-esima
            for fileGerN in `cat /tmp/nomiAssoluti$$-$count`
            do
                #confronto con il file della gerarchia 1
                if diff $fileGer1 $fileGerN > /dev/null 2>&1           #redirecto sia stdout che stderr su /dev/null
                then
                    echo Il file $fileGer1 e il file $fileGerN sono uguali
                fi
            done

        fi
        count=`expr $count + 1`
    done
done

#cancello i file temporanei
count=1
for G
do
    rm /tmp/nomiAssoluti$$-$count
    count=`expr $count + 1`
done
