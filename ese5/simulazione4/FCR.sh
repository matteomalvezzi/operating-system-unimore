#!/bin/sh

#vado nella directory fornita affinche * mi restituisca i file e le directory di tale cartella
cd $1

#scorro tutti gli elementi contenuti in questa directory
for el in *
do
    if test -d $el
    then
        case $el in
        $2?$2)
            echo "CARTELLA VALIDA TROVATA : $el"

            #aggiorno il contatore
            COUNTER=`cat $3`              #get var from file
            COUNTER=$(expr $COUNTER + 1)  #increment var
            echo $COUNTER > $3            #set new var

            #aggiungo la cartella al file
            echo "`pwd`/$el" >> $4 

        esac
        #se è una directory in cui posso entrare inizio la ricorsione
        if test -x $el
        then
            FCR.sh $el $2 $3 $4
        fi
    fi

done
