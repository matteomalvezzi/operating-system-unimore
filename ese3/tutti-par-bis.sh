#!/bin/bash
if test $# -ge 1
then  for ((i = 0; i <$#; i++))
	do echo " $i  : ${!i} "
	done
else	echo numero di parametri $# non corretto
fi
