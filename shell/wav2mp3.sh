#!/bin/bash

rename "s/\ /_/g" *

for i in `ls *.wav`
do 
	lame -b 48 $i $i.mp3
	rm $i
done

rename "s/\.wav//g" *
rename "s/_/\ /g" *

