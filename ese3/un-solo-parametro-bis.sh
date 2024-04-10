#!/bin/sh

if test $# -eq 1

	then

	case $1 in

	/*) echo "è un path assoluto"
		#controlliamo se è traversabile
		#per essere una directory traversabile deve essere innanzitutto
		#una directory, poi ci devono essere i permessi di scrittura sopra
		#se o NON é una dir o NON è scrivibile non è traversabile
		# -o indica l'OR logico
		if test ! -d $1 -o ! -x $1
			then echo "non è una direcotry travisabile"
		else 	echo "è una directory travisabile"
		fi
	;;

	*/*) echo "è un realtivo" ;;

	*) echo "è un path relativo semplice" ;;

	esac

else
	echo "numero di parametri invalido"

fi
