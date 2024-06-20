#!/bin/sh

cd $1

#variabile per i file da passare al file C
files=

#counter per capire se pari o dispari
count=`expr $3 + 1`
div=`expr $count % 2`


#verifico che la cartella corrente sia pari o dispari
if test $div -eq 0
then

    echo "INFO : la directory $1 è una directory pari"

    #estraggo i file con i requisiti
    for F in *
    do
        #controllo che sia un file
        if test -f $F -a -r $F
        then
            #calcolo il numero di righe e verifico che sia uguale a X
            lines=`wc -l < $F`
            if test -r $F -a $lines -eq $2
            then
                #in caso positivo aggiungo il file alla lista dei file da passare al main C
                files="$files $F"
            fi
        fi
    done

    #chiamo il file C
    echo INFO : invochiamo la parte C con la lista di file {$files } aventi come numero di linee = $2
    main $files
else
    echo "INFO : la directory $1 è una directory dispari"
fi

#cerco le directory per esplorare la gerarchia
for D in *
do
    #se è una directory esploro la gerarchia
    if test -d $D -a -x $D
    then
        FCR.sh `pwd`/$D $2 $count
    fi
done

