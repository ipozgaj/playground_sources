class Proxy:
	def __init__(self, target):
		self.target = target
		
	def __getattr__(self, attr):
		return getattr(self.target, attr)

class Foo:
	def first(self):
		print 'First method'
		
	def second(self):
		print 'Second method'

class FooProxy(Proxy):
	def second(self):
		pass

print 'Base class:'
f = Foo()
f.first()
f.second()

print '\nProxy class:'
fp = FooProxy(f)
fp.first()
fp.second()
