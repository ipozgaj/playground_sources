#!/bin/sh
# Convert Unix files to DOS files
# (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

if [ $# -ne 2 ]; then
	echo "Usage: unix2dos inputfile outputfile"
	exit 1
fi

sed 's/$/^M/' $1 > $2
