#!/bin/sh

for el in $1/*    #iterate each element in $1 (folder from FCP.sh)
do
    echo "ELEMENTO: $el"
    #controllo che l'elemento sia un file e sia leggibile
    if test -f $el -a -r $el
        #file readable
        then
            echo "FILE"
            n_righe=`wc -l < $el`
            #controllo il numero di righe
            if test $n_righe -ge $2
            then
                #numero di righe corretto estraggo la quinta riga
                `head -5 $el | tail -1 > $el.quinta` #creo il file
                echo $el.quinta"\n" >> file   #ne salvo il percorso
            else
                #numero di righe insufficienti
                `> $el.NOquinta`        #creo il file
                echo $el.NOquinta"\n" >> file   #salvo il percorso del file
            fi

            #aggiorno il contatore su file
            COUNTER=`cat var`      #get var from file
            COUNTER=$(expr $COUNTER + 1) #increment var
            echo $COUNTER > var    #set new var

    elif test -d $el -a -x $el
        #directory
        then
            echo "DIRECTORY $el"
            FCR.sh $el $2
    else
        echo "NON VALIDO"
    fi
done


#ATTENZIONE
#Se avessi ciclato su for el in * il path assoluto in $el non c'era
#quindi quando aggiungevo il path dovevo fare un pwd per averlo assoluto 