#!/bin/sh

cd $1

#utilizzo una variabile per tracciare il livello corrente
lvcount=$3

#le cartelle sotto questa cartella saranno in questo livello
lvcount=`expr $lvcount + 1`

#lista di file trovati
files=

#controllo se siamo al livello X
if test $lvcount -eq $2

#se siamo al livello corretto considero tutti i file non vuoti
then
    echo "La directory \"`pwd`/$F\" è sul livello X=$2"
    for F in *
    do
        #controllo che sia un file
        if test -f $F -a -r $F
        then
            #controllo che non sia vuoto
            nline=`wc -c < $F`
            if test $nline -ne 0
                then
                    files="$files $F"
            fi
        fi
    done

    #passo la lista di file trovati
    echo "passo al file C i seguenti file {$files } "
    #main $directory
fi

#esploro la gerarchia
for D in *
do
    #se è una directory esploro la gerarchia
    if test -d $D -a -x $D
    then
        FCR.sh `pwd`/$D $2 $lvcount
    fi
done
