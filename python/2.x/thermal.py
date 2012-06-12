#!/usr/bin/python
# -*- coding: latin-1 -*-

from sys import exit

try:
	f = file('/proc/acpi/ibm/thermal')
	data = f.readline().split()[1:]
except:
	print 'Module ibm-acpi is not loaded'
	exit(1)

titles = ['CPU:', 'PCI:', 'HDD:', 'GPU:', 'System battery:', 'UltraBay battery:', 'System battery:', 'UltraBay battery:']

# R60e specific
del titles[3], data[3], titles[4], data[4], titles[5], data[5]

for e in zip(titles, data):
	if e[1] != '-128':
		print e[0], e[1], '°C',
	else:
		print e[0], 'No data'
