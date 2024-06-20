#!/bin/sh

#vado nella directory fornita affinche * mi restituisca i file e le directory di tale cartella
cd $1
flag=0
nfile=0

#scorro tutti gli elementi contenuti in questa directory
for F in *
do
    #controllo che sia un file leggibile
    if test -f $F -a -r $F
    then
        #conto il numero di file visto che ci devono essere esattamente 2 file
        nfile=`expr $nfile + 1`
        #calcolo il numero di caratteri nel file
        chars=`wc -m < $F`
        #controllo da dimensione
        if test $chars -eq $2
        then
            flag=`expr $flag + 1`
        elif test $chars -eq $3
        then
            flag=`expr $flag + 1`
        fi
    
    fi

    #se è una directory esploro la gerarchia
    if test -d $F -a -x $F
    then
        FCR.sh $F $2 $3 $4 $5
    fi

done

#se la directory soddisfa i requisiti allora la salvo
if test $nfile -eq 2 -a $flag -eq 2
then
    #aggiorno il contatore
    COUNTER=`cat $4`              #get var from file
    COUNTER=`expr $COUNTER + 1`  #increment var
    echo $COUNTER > $4            #set new var
    
    #aggiungo il path della directory
    echo "`pwd`" >> $5
fi
