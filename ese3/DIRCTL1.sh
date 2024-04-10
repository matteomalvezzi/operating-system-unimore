#!/bin/sh
if test $# -eq 1
then
	if test -d $1
	then	echo "è una directory"
		ls -ld $1
	elif test -f $1
	then 	echo "è un file"
		ls  -l $1

	fi
else	echo numero di parametri $# non corretto
fi
