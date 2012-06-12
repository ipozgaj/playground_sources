#!/usr/bin/env python
# -*- coding: ascii -*-

"""
Time measuring functions, decorators and context managers
"""

__author__ = 'Igor Pozgaj <ipozgaj@gmail.com'
__copyright__ = 'Copyright (c) 2012 Igor Pozgaj'
__license__ = 'BSD'
__version__ = '1.0'

import sys
import time

if sys.platform == 'win32':
	default_timer = time.clock
else:
	default_timer = time.time

# use as decorator
def measure_time(func):
	def _measure_time(*args, **kwargs):
		start_time = default_timer()
		res = func(*args, **kwargs)
		print 'Time elapsed for function {0}: {1}'.format(func.__name__, default_timer() - start_time)
		return res
	return _measure_time
		

# use as context manager
class MeasureTime:
	def __enter__(self):
		self.start_time = default_timer()
	
	def __exit__(self, exception_type, exception_val, exception_traceback):
		print 'Time elapsed: {0}'.format(default_timer() - self.start_time)
