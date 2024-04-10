#!/bin/sh

if test $# -eq 2
then
	#il primo parametro deve essere in path relativo semplice

	case  $1  in
	*/*)  echo "parametro 1: path relativo" ;;
	/*)   echo "parametro 1: path assoluto" ;;
	*)    echo "parametro 1: path relativo semplice"

		#anche il secondo parametro deve essere in relativo semplice
		case $2 in
		*/*) echo "parametro 2: path relativo";;
		/*)  echo "parametro 2: path assoluto";;
		*)   echo "parametro 2: path relativo semplice"

		#il primo parametro deve esistere ed essere leggibile
			if test -f $1 -a -r $1
			then
				#anche il secondo deve esistere e deve essere scrivibile
				if test -f $2 -a -w $2
				then
					echo "il secondo esiste ed è scrivibile"
					#chiamo quindi il file che fa l'append
					./append.sh $1 $2
				else
					#se non eiste controlliamo che la dir corrente sia scrivibile
					current_dir=$(pwd)
					echo visto che non il secondo o non esiste o non è scrivibile verifichiamo che $current_dir sia scrivibile
					if test -w $current_dir
					then echo $current_dir è scrivibile
					else echo $current_dir non scrivibile 
					fi
				fi

			else echo "il primo file non è valido, o non esiste o è illeggibile"
			fi

		;;
		esac
	;;
	esac

else	echo numero di parametri $# non corretto
fi
