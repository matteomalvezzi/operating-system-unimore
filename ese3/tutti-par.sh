#!/bin/sh
if test $# -ge 1
then  for param in  $@
	do echo $param
	done
else	echo numero di parametri $# non corretto
fi
