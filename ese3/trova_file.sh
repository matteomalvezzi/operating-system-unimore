#!/bin/sh

if test $# -eq 1

	then

	case $1 in

	/*) echo "è un path assoluto" ;;
	*/*) echo "è un path relativo" ;;
	#se è un path realtivo semplice allora verifico che esista nella directory
	#corrente e poi ne stampo il path completo
	*) echo "è un path relativo semplice"
		if test -f $1
		then pwd $1
		fi ;;
	esac

else
	echo "numero di parametri invalido"

fi
