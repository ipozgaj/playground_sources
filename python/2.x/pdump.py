#!/usr/bin/python
# simple hexdump program
#
# (C) 2004  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

import sys, string

def dump (stream):
	eol = address = 0
	str = ''
	for ch in stream.read():
		if eol % 16 == 0:
			address = address + 8
			print ' %(str)s\n%(address)08X  ' % vars(),
			str = ''
			
		out = ord (ch)
		if ch in string.printable and ch not in string.whitespace:
			str = str + ch
		else:
			if ch == ' ':
				str = str + ' '
			else:
				str = str + '.'
		print '%(out)02X' % vars(),
		eol = eol + 1

for arg in sys.argv[1:]:
	try:
		fd = open(arg, 'r')
	except IOError:
		print 'Cannot open:', arg
	else:
		dump (fd)
		fd.close ()
		print ""
