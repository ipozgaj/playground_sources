#!/usr/bin/python

import sys

log = []
identifier = "IPT INPUT packet died"

try:
	f = file(sys.argv[1])
except IndexError:
	print "Usage: grep_log logname"
	sys.exit(1)
except IOError:
	print "Could not open: " + sys.argv[1]
	sys.exit(1)

for line in f:
	index = line.find(identifier)
	if index == -1:
		continue
	l = line.split()
	l[0] = " ".join(l[:2])
	del l[1:9]
	print l
	log.append(l)
