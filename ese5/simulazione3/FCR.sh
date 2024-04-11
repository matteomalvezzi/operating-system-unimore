#!/bin/sh

#vado nella directory fornita affinche * mi restituisca i file e le directory di tale cartella
cd $1

#scorro tutti gli elementi contenuti in questa directory
for el in *
do

    #cerco elementi che siano dei file, scrivibili, leggibili e con il nome richiesto
    if test -f $el -a -r $el -a -w $el -a $el = $2.txt
    then
        #ho trovato il file aggiorno il contatore
        COUNTER=`cat $4`       #get var from file
        COUNTER=$(expr $COUNTER + 1) #increment var
        echo $COUNTER > $4     #set new var

        #salvo il path e la lunghezza del file
        echo "`pwd`/$el : `wc -c < $el` caratteri" >> $3

    #se sono directory esploro la gerarchia
    elif test -d $el
    then
        FCR.sh $el $2 $3 $4
    fi
done
