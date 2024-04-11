#!/bin/sh

#remember to set pwd in PATH to make FCP and FCR visible from all folder (if we use source to sharing env comment out these lines after first running to avoid PATH redundancy)
#append pwd to PATH
PATH=`pwd`:$PATH
export PATH

echo 0 > var
touch file

if test $# -ge 2

    then

        #verifico che il primo numero intero sia strettamente positivo
        case $1 in
                # Not numeric
                ''|*[!0-9]*)
                    echo "Invalid parameter"
                    exit
                ;; 
                
                # Numeric
                *)  
                    if ! test $1 -ne 0 -a $1 -gt 0
                    #minore o uguale a 0
                    then
                        echo "Invalida parameter"
                        exit
                    else
                        echo "Parametro intero positivo corretto : $1"
                    fi
                ;;
        esac
        
        count=1
        #ciclo tutti i parametri
        for i in $*
        do
            #escludo il primo
            if test $count -ne 1

            then
                #controllo che siano directory
                case $i in
                /*)
                    #path assoluto
                    if test -d $i
                        #directory
                        then
                            echo "parametro valido : $i"
                            FCR.sh $i $1
                    else
                        #not a directory
                        echo "parameter error : NOT A DIRECTORY"
                        exit      
                    fi
                ;;

                *) #parametro non valido : path relativo
                    echo "parameter error : NOT AN ABSOLUTE PATH"
                    exit
                    ;;
                esac

            fi
            count=`expr $count + 1`
	done
fi

#stampo il counter
COUNTER=`cat var`      #get counter from file
echo "Contatore : $COUNTER"  #print it

for i in `cat < file`
do
    echo "FILE CREATO : $i che contiene la seguente linea"
    cat $i
    echo "\n"
done

rm file