#!/bin/bash

rename "s/\ /_/g" *.[Mm][Pp]3

for i in `ls *.[Mm][Pp]3`
do
	mplayer -quiet -ao pcm $i
	lame -b 64 audiodump.wav $i.mp3
	rm audiodump.wav
	rm $i
done

rename "s/\.wav//g" *.[Mm][Pp]3
rename "s/_/\ /g" *
