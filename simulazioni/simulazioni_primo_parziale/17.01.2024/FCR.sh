#!/bin/sh

#vado nella directory fornita affinche * mi restituisca i file e le directory di tale cartella
cd $1

#scorro tutti gli elementi contenuti in questa directory
for F in *
do

    #se è una directory controllo il suo livello e continuo la gerarchia
    if test -d $F -a -x $F
    then
        #costruisco il path completo
        dir_path=`pwd`/$F

        #prendo il numero di / contenuti nel path
        slash=`echo $dir_path | tr -cd '/'`

        #calcolo il numero di / e in base a quello determino il livello
        level=`echo $slash | wc -c`

        #tolgo 1 perchè echo stampa anche \n e wc conta \n come carattere e quindi conta un livello in più
        level=`expr $level - 1`        
        
        #controllo se è pari
        mod=`expr $level % 2`
        if test $mod -eq 0
        then
            echo $dir_path >> $3
        fi
        
        #invoco la ricorsione
        FCR.sh $F $2 $3
    fi

done