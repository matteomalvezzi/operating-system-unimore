#!/bin/sh

#vado nella directory fornita affinche * mi restituisca i file e le directory di tale cartella
cd $1
X=$2

shift
shift

flag=0

#scorro tutti gli elementi contenuti in questa directory
for F in *
do
    #controllo che sia un file leggibile
    if test -f $F -a -r $F
    then
        
        #controllo che abbia il numero di lineee corretto
        nl=`wc -l < $F`
        if test $nl -ge $X
        then
            flag=1
            #chiamo il file C
            echo "chiamo il file C con questi parametri {$*}"
            #main $F $3
        fi
    fi
    


done

#stampo la directory
if test $flag -eq 1
then
    echo "INFO : la directory \"`pwd`\" contiene almeno un file con $X righe"
fi

#cerco le directory per esplorare la gerarchia
for D in *
do
    #se è una directory esploro la gerarchia
    if test -d $D -a -x $D
    then
        FCR.sh `pwd`/$D $X $*
    fi
done