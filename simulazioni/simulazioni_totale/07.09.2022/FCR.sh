#!/bin/sh

#vado nella directory fornita affinche * mi restituisca i file e le directory di tale cartella
cd $1

count=0

flag=0
files=

#scorro tutti gli elementi contenuti in questa directory
for F in *
do
    #controllo che sia un file leggibile
    if test -f $F -a -r $F
    then
        count=`expr $count + 1`

        #controllo che lunghezza sia X
        lenght=`wc -l < $F`
        if test $lenght -eq $2
        then

            #controllo che sia in posizione dispari
            mod_count=`expr $count % 2`
            if test $mod_count -eq 1
            then
                echo "Il file \"$F\" ha esattamente $2 righe e si trova in posizione $count (posizione dispari)"
                flag=1
                files="$files $F"
            fi

        fi
    fi
done

if test $flag -eq 1
then

    echo INFO : la directory $1 contiene almeno un file in posizione dispari avente $2 linee

    echo INFO : invochiamo la parte C con i file {$files }

    #main $files
fi

#esploro la gerarchia
for D in *
do
    #se è una directory esploro la gerarchia
    if test -d $D -a -x $D
    then
        FCR.sh `pwd`/$D $2
    fi
done

