#!/bin/sh

#vado nella directory fornita affinche * mi restituisca i file e le directory di tale cartella
cd $1

#scorro tutti gli elementi contenuti in questa directory
for F in *
do
    #controllo che la radice stessa contenga un file di nome S
    if test -f $F -a $F = $2
    then
        #aggiorno il contatore
        COUNTER=`cat $3`              #get var from file
        COUNTER=$(expr $COUNTER + 1)  #increment var
        echo $COUNTER > $3            #set new var

        #aggiungo la directory
        echo "`pwd`" >> $4 
    fi

    #se è una directory esploro la gerarchia
    if test -d $F -a -x $F
    then
        FCR.sh $F $2 $3 $4
    fi

done