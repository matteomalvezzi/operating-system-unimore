#!/bin/sh

#case che confronta il numero di parametri passati $# è il numero di parametri
case $# in

#opzione)  comando  ;; per terminare

0) ls -al ./* | less ;;

1)	if test -d $1
	then	echo "è una directory"
		ls -ld $1
	elif test -f $1
	then 	echo "è un file"
		ls  -l $1

	fi ;;

*) echo numero di parametri $# non valido ;;
esac
