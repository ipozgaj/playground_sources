#!/usr/bin/python

import sys
from xml.dom.minidom import parse

class NotTextNodeError:
	pass

def getTextFromNode(node):
	"""Strips text from an XML node"""
	text = ""
	for n in node.childNodes:
		if n.nodeType == node.TEXT_NODE:
			text += n.nodeValue
		else:
			raise NotTextNodeError
	return text

def nodeToDictionary(node):
	dictionary = {}
	siblings = {}
	for n in node.childNodes:
		multiple = False
		if n.nodeType != n.ELEMENT_NODE:
			continue
		if len(node.getElementsByTagName(n.nodeName)) > 1:
			multiple = True
			if not siblings.has_key(n.nodeName):
				siblings[n.nodeName] = []

		try:
			text = getTextFromNode(n)
		except NotTextNodeError:
			if multiple:
				siblings[n.nodeName].append(nodeToDictionary(n))
				dictionary.update({n.nodeName:siblings[n.nodeName]})
				continue
			else:
				dictionary.update({n.nodeName:nodeToDictionary(n)})
				continue

		if multiple:
			siblings[n.nodeName].append(text)
			dictionary.update({n.nodeName:siblings[n.nodeName]})
		else:
			dictionary.update({n.nodeName:text})
	return dictionary

def XMLToDictionary(filename):
	dom = parse(filename)
	return nodeToDictionary(dom)

if __name__ == "__main__":
	try:
		print "Parsing XML data..."
		addresses = XMLToDictionary(sys.argv[1])
	except IndexError:
		addresses = XMLToDictionary('contacts.xml')
	query = raw_input ('Enter search pattern: ').lower()
	for contact in addresses['contacts']['contact']:
		if query in contact['lastname'].lower() or query in contact['firstname'].lower():
			print 
			print 'Name: ' + contact['firstname'] + ' ' + contact['lastname']
			print 'Email: ' + contact['email']

