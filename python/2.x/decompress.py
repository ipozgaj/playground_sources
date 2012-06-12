#!/usr/bin/python
# Python decompressor (see compress.py)
# (C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

from sys import argv, exit
from os import path
from zlib import decompress

if (len (argv) <> 2):
	print "Syntax compress.py [filename]"
	exit (1)

if (path.isfile (argv[1])):
	new_filename = argv[1][:-5]
else:
	print "No such file: " + argv[1]
	exit (1)

try:
	input = open (argv[1], "rb")
except:
	print "Input file error"
	exit (1)

data = input.read()
decompressed_data = decompress (data)
input.close()

try:
	output = open (new_filename, "wb")
except:
	print "Output file error"
	exit (1)

output.write (decompressed_data)
output.close()
