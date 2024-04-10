#!/bin/sh

#case che confronta il numero di parametri passati $# è il numero di parametri
case $# in

1) cat >> $1 ;;

2) cat $1 >> $2 ;;

*) echo numero di parametri $# non valido ;;

esac
