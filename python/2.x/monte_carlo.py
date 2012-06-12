#!/usr/bin/python

from math import hypot
from random import random

nodes = 100000
hits = 0

for i in range(nodes):
	if hypot(random(), random()) <= 1:
		hits = hits + 1
	
print 4. * hits / nodes
		
