from html.parser import HTMLParser
from http.client import HTTPConnection

artist, song = "", ""

class AntenaZgHTMLParser(HTMLParser):
	def handle_starttag(self, tag, attrs):
		if tag == "td":
			if attrs[0] == ('class', 'song'):
				song = attrs[1][1]
				print(song)
			if attrs[0] == ('class', 'artist'):
				artist = attrs[1][1]
				print(artist, end = " - ")
				
	def handle_endtag(self, tag):
		pass

for page in range(1, 61):
	conn = HTTPConnection("300.antenazagreb.hr")
	conn.request("GET", "/?page={}".format(page))
	response = conn.getresponse()
	parser = AntenaZgHTMLParser()
	data = response.read()
	parser.feed(str(data))
	conn.close()