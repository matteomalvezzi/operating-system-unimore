#!/bin/sh

check=0          #flag that say me if element resprect the requirements 

for el in $1*    #iterate each element in $1 (folder from FCP.sh)
do
    if test -d $el  #check if it is a dir
        then
            # -------- finded directory --------
            check=1    #if there is subdir, requirements are not respected
            #echo "DIRECTORY : $el/"
            FCR.sh $el/ $2
            
    elif test -f $el    #check if it is a file
        then
            # -------- finded a file --------

            #echo "FILE : $el con `wc -l < $el` righe --> righe minime : $2"

            if test `wc -l < $el` -lt $2
            then
                #finded short files
                check=1
                
            fi
    fi
    
done

    #if element respect all requirements (no sub-dir and no short file) update file that contain counter and print directory

    if test $check -eq 0
    then
        COUNTER1=`cat var`      #get var from file
        COUNTER1=$(expr $COUNTER1 + 1) #increment var
        echo $COUNTER1 > var    #set new var

        echo $1
    fi


