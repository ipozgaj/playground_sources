class Singleton(object):
	"""Singleton pattern - make sure we have only one instance of the class"""
	_instance = None	
	def __new__(cls, *args, **kwargs):
		if Singleton._instance is None:
			Singleton._instance = super(Singleton, cls).__new__(cls, *args, **kwargs)
		return Singleton._instance

class Borg:
	"""Share state between instances of the class"""
	_shared_state = {}
	def __init__(self):
		self.__dict__ = Borg._shared_state
