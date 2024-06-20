#!/bin/sh

#vado nella directory fornita affinche * mi restituisca i file e le directory di tale cartella
cd $1

count1=0
count2=0

files1=
files2=

#scorro tutti gli elementi contenuti in questa directory
for F in *
do
    #controllo che sia un file leggibile
    if test -f $F -a -r $F
    then
        
        #controllo che abbia estensione .E1
        case $F in
		    *.$2)   
                count1=`expr $count1 + 1`
			    files1="$files1 $F"         #aggiungo il file in appena alla variabile
			    ;;
		    *) ;;
		esac

        #controllo che sia scrivibile
        if test -w $F
        then
            #controllo che abbia estensione .E2
            case $F in
                *.$3)
                    count2=`expr $count2 + 1`
                    files2="$files2 $F"
                    ;;
                *) ;;
            esac
        fi
    fi
    
    #se è una directory esploro la gerarchia
    if test -d $F -a -x $F
    then
        FCR.sh `pwd`/$F $2 $3
    fi

done

if test $count1 -ne 0 -a $count2 -ne 0
then

    echo INFO : la directory $1 contiene almeno un file con estensione .$2 e almeno un file con estensione .$3
    for file2 in $files2
    do
        echo INFO : invochiamo la parte C con la lista di file con estensione .$2 {$files1} e con il file con estensione .$3 {$file2}
        #invocazione file C
    done
fi

