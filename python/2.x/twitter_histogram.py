#!/usr/bin/python

import sys
import urllib
import xml.etree.ElementTree as et
 
if len(sys.argv) < 2:
	print "Usage: %s keywords..." % sys.argv[0]
	sys.exit(1)

search_url = 'http://search.twitter.com/search.atom?'
search_url += 'q=' + urllib.quote(' '.join(sys.argv[1:]))
search_url += '&since=2010-01-01'
search_url += '&rpp=100'

feed = urllib.urlopen(search_url)
tree = et.parse(feed)
feed.close()

for e in tree.getiterator():
	if e.tag.endswith('published'):
		print e.text
