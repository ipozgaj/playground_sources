def getperm(l):
	if not l:
		yield l
	else:
		for element in l:
			for perm in getperm([x for x in l if x != element]):
				yield [element] + perm

for perm in getperm([1, 2, 3, 4, 5]):
	print perm
