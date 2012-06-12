#!/usr/bin/python
# retrievec RSS data
# (C) 2004  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

import sys, getopt, urllib

def display_usage (exit_code):
	print """Usage: rssview [option(s)] url(s)
    -h  --help     Display this usage information
    -o  --output   Save downloaded data to file (default is 'rssdata.xml'
    -v  --verbose  Turn on verbose messages
    -V  --version  Display program version"""
	sys.exit (exit_code)
	

program_version = "0.1"
output = "rssdata.xml"
verbose = 0

short_options = "ho:vV"
long_options = ["help", "output=", "verbose", "version"]

try:
	options, servers_list = getopt.getopt (sys.argv[1:], short_options, long_options)
except getopt.GetoptError:
	display_usage (1)

for option, argument in options:
	if option in ("-h", "--help"):
		display_usage (0)
	if option in ("-o", "--output"):
		output = argument
	if option in ("-v", "--verbose"):
		verbose = 1
	if option in ("-V", "--version"):
		print "rssview version " + program_version

if servers_list == []:
	display_usage (1)

for server in servers_list:
	try:
		if verbose:
			print "Retrieving data from: " + server
		urllib.urlretrieve (server, output)
	except IOError:
		print "Temporary failure in name resolution"
		sys.exit (1)

