#!/usr/bin/python
# Simple python data compression programm 
# (C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

from sys import argv, exit
from os import path
from zlib import compress

if (len (argv) <> 2):
	print "Syntax compress.py [filename]"
	exit (1)

if (path.isfile (argv[1])):
	new_filename = argv[1] + '.pycp'
else:
	print "No such file: " + argv[1]
	exit (1)

try:
	input = open (argv[1], "rb")
except:
	print "Input file error"
	exit (1)

data = input.read()
compressed_data = compress (data)
input.close()

try:
	output = open (new_filename, "wb")
except:
	print "Output file error"
	exit (1)

output.write (compressed_data)
output.close()
