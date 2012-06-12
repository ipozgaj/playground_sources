#!/usr/bin/python

import sys, os, getopt
import md5, sha

program_version = '0.1.1'
verbose_messages = False
digest_algorithm = 'md5'


def usage():
	"""Display program usage and exit""" 
	print 'psyncdir version %s' % program_version
	print 'Usage: psyncdir [options] URL1 URL2\n'
	print 'Options are:'
	print '  -d, --digest     Select one of hash algorithms: md5(default), sha1'
	print '  -r  --recursive  Recursive directory syncronization'
	print '  -h, --help       Display this usage information'
	print '  -v, --verbose    Display verbose messages'
	print '  -V, --version    Display program version'
	sys.exit()


def calculate_hash(filename, algorithm = 'md5'):
	"""Return hash for given file using on of the hash algorithms"""
	try:
		f = file(filename)
	except IOError:
		print 'No such file: %s' % filename
		sys.exit(1)
	if algorithm == 'md5':
		hash = md5.new()
	elif algorithm == 'sha1':
		hash = sha.new()
	elif algorithm == 'hmac':
		hash = hmac.new()
	else:
		print 'No such algorith: %s' % algorithm
		sys.exit(1)
	for line in f:
		hash.update(line)
	f.close()
	return hash.digest()


def get_files(path, recursive = True):
	"""Returns list of files within directory"""
	files = []
	if path[-1] == os.sep:
		path = path[:-1]
	items = os.listdir(path)
	for item in items:
		if item in ('.', '..'):
			continue
		new_path = path + os.sep + item
		if os.path.isdir(new_path) and recursive:
			files.extend(get_files(new_path, True))
		else:
			files.append(path + os.sep + item)
	return files


def main():
	short_options = 'd:vhv'
	long_options = ['digest=', 'verbose', 'help', 'version']
	try:
		opts, args = getopt.getopt(sys.argv[1:], short_options, long_options)
	except getopt.GetoptError:
		usage()
	for option, arg in opts:
		if option in ('-d', '--digest'):
			if arg not in ('sha1', 'md5'):
				print 'No such hash algorithm: %s' % arg
				sys.exit(1)
			digest_algorithm = arg
		if option in ('-v', '--verbose'):
			verbose_messages = True
		if option in ('-h', '--help'):
			usage()
		if option in ('-V', '--version'):
			print "psyncdir version %s" % program_version
			sys.exit()
		

if __name__ == '__main__':
	for f in get_files('work'):
		print f.ljust(60), calculate_hash(f)
	#main()
