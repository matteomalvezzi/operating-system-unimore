#!/bin/sh

#vado nella directory fornita affinche * mi restituisca i file e le directory di tale cartella
cd $1

#scorro tutti gli elementi contenuti in questa directory
for F in *
do
    #controllo che sia un file
    if test -f $F -a -r $F
    then

        #controllo che F abbia almeno un carattere
        nlinee=`wc -m < $F`
        if test $nlinee -gt 0
        then
            #controlliamo che contenga solo lettere minuscole
            case `cat $F` in
                [!a-z]) 
                    #contiene altri caratteri che non soddisfano le specifiche
                    echo "$F non soddisfa le specifiche"
                    ;;
                *)
                    #contiene solo caratteri alfabetici minuscoli
                    
                    #aggiorno il contatore
                    COUNTER=`cat $2`              #get var from file
                    COUNTER=`expr $COUNTER + 1`  #increment var
                    echo $COUNTER > $2            #set new var

                    #aggiungo il path del files
                    echo "`pwd`/$F" >> $3
                    ;;

            esac

        fi
    fi

    #se è una directory esploro la gerarchia
    if test -d $F -a -x $F
    then
        FCR.sh $F $2 $3
    fi

done