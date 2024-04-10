#!/bin/sh

#remember to set pwd in PATH to make FCP and FCR visible from all folder (comment out these lines after first running to avoid PATH redundancy)
append pwd to PATH
PATH=`pwd`:$PATH
export PATH

echo 0 > var   #reset var file, who will contain counter variable

if test $# -ge 3

then
    #get last parameter with loop
    last=""
    for param in $@
        do last=$param
    done

    #get last parameter with loop and counter
    count=0
    for i in $@
    do
        count=`expr $count + 1`
        if test $count -eq $#
        then
            last=$i
        fi
    done

    echo "I file interni devono avere più di : $last righe"

    echo "Directory che rispettano i requisiti"
    
    #for each parm very if it is an absolute directory
    for var in $@
    do
        #check if it is a number and it is last
        case $var in
            # Not numeric
            ''|*[!0-9]*)
                ;; 
            
            # Numeric
            *)  
                if test $last -eq $var
                #last element
                then 

                    if ! test $last -gt 0
                        #not positive
                        then 
                            echo "numero non positivo"
                            exit
                    fi
                fi
            ;;
        esac

        case $var in

        /*)
            #parameter is an absolute path
            if test -d $var
                #parameter is a directory
	            then
                    FCR.sh $var $last

            else
                #prameter is not a directory
                echo "parameter error : NOT A DIRECTORY"
                exit
            
            fi
        ;;

        ''|*[0-9]*) #parameter is a number
            
        ;;

        *) #parameter is not a number
            echo "parameter error : NOT AN ABSOLUTE PATH"
            exit
            ;;
        esac

	done

else
    echo "number of parameter error"
    exit
fi

#print counter

COUNTER1=`cat var`      #get counter from file
echo "Totale : $COUNTER1"  #print it
