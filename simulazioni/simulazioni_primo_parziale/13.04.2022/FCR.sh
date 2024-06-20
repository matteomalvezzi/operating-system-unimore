#!/bin/sh

#vado nella directory fornita affinche * mi restituisca i file e le directory di tale cartella
cd $1

#scorro tutti gli elementi contenuti in questa directory
for F in *
do

    #controllo che sia un file leggibile
    if test -f $F -a -r $F
    then
        #estraggo il numero di linee
        L=`wc -l < $F`

        #controllo che il numero di linee sia uguale a X
        if test $L -eq $2
        then
            #salvo il percorso del file sul file temporaneo (lo metto in append)
            echo "`pwd`/$F" >> $3
        fi
    fi

    #se è una directory esploro la gerarchia
    if test -d $F -a -x $F
    then
        FCR.sh `pwd`/$F $2 $3
    fi
done