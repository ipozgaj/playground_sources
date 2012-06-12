#!/usr/bin/python
# Simple Web service for monitoring *nix server
# Original XMLRPCServer code provided by Bozidar Kenig, 2005.
# 
# ChangeLog:
#	Dec 31 2005 (ipozgaj) - added configurability via config file
#
# TODO list:
# 	make command output sequence in same order as in configuration file
# 	make command title letters to be in same case as in configuration file

from SimpleXMLRPCServer import SimpleXMLRPCServer, SimpleXMLRPCRequestHandler
import os, sys, time, ConfigParser

config = ConfigParser.ConfigParser()

# write preamble to output
def open_log (buffer, bgcolor, fgcolor):
	fgcolor = fgcolor.replace("\"", "")
	bgcolor = bgcolor.replace("\"", "")
	buffer = "<body text=\"" + fgcolor + "\" bgcolor=\"" + bgcolor + "\">"
	buffer = buffer + "<font face=\"Verdana\">"
	buffer = buffer + "<hr>Time: " + time.strftime ("%c") + "<hr>"
	return buffer

# close HTML tags specified in preamble
def close_log (buffer):
	buffer = buffer + "</font>"
	buffer = buffer + "</body>"
	return buffer

# add output from system command to user output
def add_command_output (buffer, title, command):
	try:
		data = os.popen(command).read()
		data = data.replace ("<", "&lt;")
		data = data.replace (">", "&gt;")
		data = "<pre>" + data + "</pre>"
	except:
		print "Error"

	buffer = buffer + "<br><b>" + title + "</b>: " + data
	return buffer
	
# HTTP server class
class RequestHandler (SimpleXMLRPCRequestHandler):
	def do_GET (self):
		if config.get ("General", "access_control") == "yes":
			if self.client_address[0] not in config.options ("Allowed hosts"):
				print "Rejected request from: " + self.client_address[0]
				return

		buffer = ""
		buffer = open_log (buffer, config.get ("Colors", "background"), config.get ("Colors", "text"))
		
		for command in config.options ("Commands"):
			buffer = add_command_output (buffer, command, config.get ("Commands", command))

		buffer = close_log (buffer)
		
		# return data to client
		self.send_response (200)
		self.send_header ("Content-type", "text/html")
		self.send_header ("Content-length", str (len (buffer)))
		self.end_headers()
		self.wfile.write (buffer)
		self.wfile.flush()

		self.connection.shutdown (1)


# main function
if __name__ == '__main__':
	print "Parsing configuration file..."

	config.read ("hostinfo.cfg")
	
	service_port = config.get("General", "port")
	
	print "Starting service on local port " + service_port + "..."
	
	local_server = SimpleXMLRPCServer (("0.0.0.0", int (service_port)), RequestHandler)
	local_server.register_function (open_log)
	local_server.register_function (close_log)
	local_server.register_function (add_command_output)

	try:
		local_server.serve_forever()
	except:
		print "Shutting down service..."
