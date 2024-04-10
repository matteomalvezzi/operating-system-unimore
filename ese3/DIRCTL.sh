#!/bin/sh
if test $# -eq 1
then 	echo SONO DIR.sh
	echo numero parametri $#
     	echo 'Il valore di $0 ===>' $0
	echo 'Il valore di $1 ===>' $1
	echo "DEBUG-Ora eseguo ls -l $1"
	ls -l $1

else	echo numero di parametri $# non corretto
fi
