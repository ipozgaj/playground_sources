#!/bin/sh
# Convert DOS files to Unix files
# (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

if [ $# -ne 2 ]; then
	echo "Usage: dos2unix inputfile outputfile"
	exit 1
fi

tr -d '\015' < $1 > $2
