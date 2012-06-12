#!/usr/bin/python

url = raw_input('URL: ')
coded = ['%' + hex(ord(ch)).split('x')[1] for ch in url]
print "".join(coded)
