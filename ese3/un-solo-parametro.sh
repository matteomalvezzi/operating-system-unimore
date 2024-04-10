#!/bin/sh

if test $# -eq 1

	then

	case $1 in

	/*) echo "è un path assoluto" ;;

	*/*) echo "è un realtivo" ;;

	*) echo "è un path relativo semplice" ;;

	esac

else
	echo "numero di parametri invalido"

fi
