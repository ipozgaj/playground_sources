#!/usr/bin/python

import urllib
import htmllib
import formatter

class HTMLTableParser(htmllib.HTMLParser):
	def __init__(self):
		self.data = []
		self.td_data = []
		self.in_table = True
		self.in_center = True
		htmllib.HTMLParser.__init__(self, formatter.NullFormatter())

	def start_table(self, attributes):
		in_table = True

	def end_table(self):
		in_table = False

	def start_tr(self, attributes):
		self.row = []

	def end_tr(self):
		self.data.append(self.row)

	def start_td(self, attributes):
		self.td_data = []

	def end_td(self):
		self.row.append(" ".join(self.td_data))

	def start_center(self, attributes):
		self.in_center = True

	def end_center(self):
		self.in_center = False

	def handle_data(self, data):
		if self.in_table:
			self.td_data.append(data.rstrip().lstrip())
		if self.in_center and 'sati' in data:
			print data + '\n'


def extract_data(url):
	html = urllib.urlopen(url).read()
	parser = HTMLTableParser()
	parser.feed(html)
	parser.close()
	return parser.data


def formatted_output(data):
	print 'Grad                  Vjetar  Temp Vlaga',
	print ' Tlak/+- u 1h   Vrijeme'
	print '-' * 75
	for line in data[1:]:
		print line[0][2:].ljust(21),
		print line[1].ljust(3) + line[2].ljust(5),
		print line[3].ljust(5) + line[4].ljust(5),
		print line[5].ljust(8) + line[6].ljust(6),
		print line[7]


if __name__ == '__main__':
	data = extract_data('http://vrijeme.hr/croa_n.html')[:-1]
	formatted_output(data)
