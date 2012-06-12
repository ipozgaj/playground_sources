__author__ = 'Igor Pozgaj'
__date__ = '5 Feb 2006'
__version__ = '0.1'

from sys import maxint


infinity = maxint


class GraphError(Exception):
	"""Generic graph library exception"""
	pass


class Graph:
	"""Graph theory base class"""
	def __init__(self, digraph = False):
		"""Initializes empty graph"""
		self.vertices = []
		self.edges = []
		self.digraph = digraph
			

	def __str__(self):
		"""Nice output of graph"""
		s = 'Vertices: '
		for vertex in self.get_vertices():
			s = s + vertex + ' '
		s = s + '\nEdges:\n'
		edges = self.get_edges()
		edges.sort()
		for edge in edges:
			if self.is_digraph():
				s = s + '\t' + edge[0] + ' - ' + edge[1] + '  -->  ' + str(edge[2]) + '\n'
			else:
				if edge[0] < edge[1]:
					s = s + '\t' + edge[0] + ' - ' + edge[1] + '  -->  ' + str(edge[2]) + '\n'
		return s[:-1]


	def copy(self):
		"""Returns copy of graph"""
		c = Graph(self.digraph)
		c.vertices = self.get_vertices()
		c.edges = self.get_edges()
		return c


	def is_digraph(self):
		"""Returns True if graph is digraph"""
		return self.digraph


	def get_size(self):
		"""Returns size of graph"""
		return len(self.vertices)


	def add_vertex(self):
		"""Add vertex to graph"""
		if not self.vertices:
			self.vertices.append('A')
			return 'A'
		new_vertex = chr(max(map(ord, self.vertices)) + 1)
		self.vertices.append(new_vertex)
		return new_vertex
	

	def remove_vertex(self, vertex):
		"""Removes vertex and all edges containing that vertex"""
		try:
			self.vertices.remove(vertex)
			self.edges = [(start, end, weight) for start, end, weight in self.edges if vertex not in (start, end)]
		except ValueError:
			raise GraphError, 'No such vertex in graph'


	def get_vertices(self):
		"""Returns list of vertices"""
		return self.vertices[:]


	def is_vertex(self, vertex):
		"""Returns True if vertex is in graph"""
		return vertex in self.vertices

	
	def get_degree(self, vertex):
		"""Returns degree of vertex"""
		def sum(l):
			ret = 0
			for i in l:
				ret = ret +i
			return ret
		if not self.is_vertex(vertex):
			raise GraphError, 'No such vertex'
		return sum([weight for start, end, weight in self.get_edges() if end == vertex])

	
	def get_out_degree(self, vertex):
		"""Returns outer degree of vertex"""
		def sum(l):
			ret = 0
			for i in l:
				ret = ret +i
			return ret
		if not self.is_vertex(vertex):
			raise GraphError, 'No such vertex'
		return sum([weight for start, end, weight in self.get_edges() if start == vertex])

	
	def adjacent_to(self, vertex):
		"""Returns list of vertices adjacent to given vertex"""
		adj_list = []
		for next_vertex in self.get_vertices():
			if self.is_edge(vertex, next_vertex):
				adj_list.append(next_vertex)
		return adj_list


	def add_edge(self, start, end, weight = 1):
		"""Add edge to graph. Edge weight is summed if edge already exists"""
		if (not self.is_vertex(start)) or (not self.is_vertex(end)):
			raise GraphError, 'No such vertex in graph'
		if self.is_edge(start, end):
			weight = weight + self.get_weight(start, end)
			self.remove_edge(start, end)
		self.edges.append((start, end, weight))
		if not self.is_digraph():
			self.edges = [(a,b,w) for a, b, w in self.edges if (a != end) or (b != start) ]
			self.edges.append((end, start, weight))
		

	def remove_edge(self, start, end):
		"""Removes edge from graph"""
		if (not self.is_vertex(start)) or (not self.is_vertex(end)):
			raise GraphError, 'No such vertex in graph'
		self.edges = [(s, e, w) for s, e, w in self.get_edges() if start != s or end != e]
		if not self.is_digraph():
			self.edges = [(s, e, w) for s, e, w in self.get_edges() if start != e or end != s]
			
			
	def get_edges(self):
		"""Returns lists of edges"""
		return self.edges[:]


	def get_edges_num(self):
		"""Returns number of edges"""
		return len(self.edges)


	def is_edge(self, i, j):
		"""Returns true if (i, j) is edge in graph"""
		for start, end, weight in self.get_edges():
			if start == i and end == j and weight != infinity:
				return True 
		return False

	
	def get_weight(self, start, end):
		"""Returns weight of edge"""
		if not self.is_edge(start, end):
			raise GraphError, 'No such edge in graph'
		for s, e, weight in self.get_edges():
			if s == start and e == end:
				return weight


	def complement(self):
		"""Returns new graph which is complement of given graph (weight of edges is set to 1)"""
		comp = Graph()
		comp.vertices = self.get_vertices()
		for start in self.get_vertices():
			for end in self.get_vertices():
				if not self.is_edge(start, end) and not comp.is_edge(start, end) and start != end:
					comp.add_edge(start, end, 1)
		return comp


	def get_adj_list(self):
		"""Returns dictionary containing adjacency list of graph"""
		adj_list = {}
		for vertex in self.get_vertices():
			adj_list[vertex] = self.adjacent_to(vertex)
		return adj_list


	def get_adj_matrix(self):
		"""Returns adjacency matrix of graph"""
		adj_matrix = [[0] * self.get_size() for i in range(self.get_size())]
		for vertex in self.get_vertices():
			for next_vertex in self.get_vertices():
				if self.is_edge(vertex, next_vertex):
					adj_matrix[ord(vertex) - ord('A')][ord(next_vertex) - ord('A')] = self.get_weight(vertex, next_vertex)
		return adj_matrix


	def get_inc_matrix(self):
		"""Returns incidency matrix of graph"""
		inc_matrix = [[0] * self.get_edges_num() for i in range(self.get_size())]
		for vertex in self.get_vertices():
			for edge in self.get_edges():
				if vertex in edge:
					inc_matrix[ord(vertex) - ord('A')][self.edges.index(edge)] = 1
		return inc_matrix


def nullgraph():
	"""Returns null graph"""
	return Graph()


def chain(size, weight = 1):
	"""Returns chain of given size with edges set to given weight"""
	if size == 0:
		return nullgraph()
	if size < 0:
		raise GraphError, 'Invalid graph size'
	graph = Graph()
	prev_vertex = graph.add_vertex()
	for i in range(size - 1):
		new_vertex = graph.add_vertex()
		graph.add_edge(prev_vertex, new_vertex, weight)
		prev_vertex = new_vertex
	return graph


def cycle(size, weight = 1):
	"""Returns cycle of given size with edges set to given weight"""
	if size in (0, 1):
		return nullgraph()
	if size < 0:
		raise GraphError, 'Invalid graph size'
	graph = Graph()
	first = graph.add_vertex()
	prev_vertex = first
	for i in range(size - 1):
		new_vertex = graph.add_vertex()
		graph.add_edge(prev_vertex, new_vertex, weight)
		prev_vertex = new_vertex
	graph.add_edge(first, prev_vertex, weight)
	return graph


def wheel(size, weight = 1):
	"""Returns wheel of given size with edges set to given weight"""
	if size in (0, 1, 2):
		return nullgraph()
	if size < 0:
		raise GraphError, 'Invalid graph size'
	graph = cycle(size - 1, weight)
	base = graph.add_vertex()
	for vertex in graph.get_vertices():
		if base != vertex:
			graph.add_edge(base, vertex, weight)
	return graph


def complete(size, weight = 1):
	"""Returns complete graph of given size with edges set to given weight"""
	if size == 0:
		return nullgraph()
	if size < 0:
		raise GraphError, 'Invalid graph size'
	graph = Graph()
	for i in range(size):
		graph.add_vertex()
	vertices = graph.get_vertices()
	for i in range(graph.get_size()):
		for j in range(i):
			graph.add_edge(vertices[i], vertices[j], weight)
	return graph
		

def complete_bipartite(r, s, weight = 1):
	"""Returns r,s complete bipartite graph with edges set to given weight"""
	if r == 0 or s == 0:
		return nullgraph()
	if r < 0 or s < 0:
		raise GraphError, 'Invalid graph size'
	graph = Graph()
	r_vertices = []
	for i in range(r):
		r_vertices.append(graph.add_vertex())
	s_vertices = []
	for i in range(s):
		s_vertices.append(graph.add_vertex())
	for i in r_vertices:
		for j in s_vertices:
			graph.add_edge(i, j, weight)
	return graph


def cube(size, w = 1):
	"""Returns cube graph of given size with vertices set to given weight """
	def generate_subgraph(subgraph_size, g):
		if subgraph_size > 1:
			generate_subgraph(subgraph_size - 1, g)
		vertices = g.get_vertices()
		edges = g.get_edges()
		new_vertices = map(lambda x: g.add_vertex(), vertices)
		for i in range(len(vertices)):
			g.add_edge(vertices[i], new_vertices[i], 1)
		for a, b, w in edges:
			g.add_edge(new_vertices[vertices.index(a)], new_vertices[vertices.index(b)], w)
	g = Graph()
	g.add_vertex()
	generate_subgraph(size, g)
	return g


def petersen(weight = 1):
	"""Returns Peterson's graph with edges set to given weight"""
	graph = Graph()
	a = graph.add_vertex()
	b = graph.add_vertex()
	c = graph.add_vertex()
	d = graph.add_vertex()
	e = graph.add_vertex()
	f = graph.add_vertex()
	g = graph.add_vertex()
	h = graph.add_vertex()
	i = graph.add_vertex()
	j = graph.add_vertex()
	graph.add_edge(a, b, weight)
	graph.add_edge(b, c, weight)
	graph.add_edge(c, d, weight)
	graph.add_edge(d, e, weight)
	graph.add_edge(e, a, weight)
	graph.add_edge(a, f, weight)
	graph.add_edge(b, g, weight)
	graph.add_edge(c, h, weight)
	graph.add_edge(d, i, weight)
	graph.add_edge(e, j, weight)
	graph.add_edge(f, h, weight)
	graph.add_edge(h, j, weight)
	graph.add_edge(j, g, weight)
	graph.add_edge(g, i, weight)
	graph.add_edge(i, f, weight)
	return graph


def depth_first_search(graph, visit_functor, start_vertex = 'A'):
	"""Depth first search of graph. Visit functor is called for every vertex"""
	def visit(vertex):
		visit_functor(vertex)
		visited[vertex] = True
		for next_vertex in graph.adjacent_to(vertex):
			if not visited[next_vertex]:
				visit(next_vertex)
	visited = {}
	for vertex in graph.get_vertices():
		visited[vertex] = False
	n = 1
	visit(start_vertex)
	for vertex in graph.get_vertices():
		if not visited[vertex]:
			n = n + 1
			visit(vertex)
	return n


def breath_first_search(graph, visit_functor, start_vertex = 'A'):
	"""Breath first search of graph. Visit functor is called for every vertex"""
	visited = {}
	for vertex in graph.get_vertices():
		visited[vertex] = False
	next = [start_vertex]
	while next:
		current_node = next[0]
		next = next[0:]
		visit_functor(current_node)
		visited[current_node] = True
		next.extend(graph.adjacent_to(current_node))
		next = filter(lambda x: not visited[x], next)
		if not next and False in visited:
			next = [visited.index(False)]


def number_of_components(graph):
	"""Returns number of unconnected componnents in graph"""
	def visit_functor(x):
		pass
	return depth_first_search(graph, visit_functor)


def is_connected(graph):
	"""Returns True if graph is connected"""
	return number_of_components(graph) == 1


def get_hamilton_path(graph):
	"""Returns True (and path) if graph contains Hamilton path"""
	marked = 0
	path = []
	def mark_vertex(start_vertex, vertex, marked):
		marked = marked + 1
		visited[vertex] = marked
		if visited[vertex] == graph.get_size() and graph.is_edge(vertex, start_vertex):
			for i in range(1, graph.get_size() + 1):
				for v in graph.get_vertices():
					if i == visited[v]:
						path.append(v)
			return
		for v in graph.get_vertices():
			if not visited[v] and graph.is_edge(v, vertex):
				mark_vertex(start_vertex, v, marked)
		marked = marked - 1
		visited[vertex] = 0
	visited = {}
	for vertex in graph.get_vertices():
		visited[vertex] = 0
	for vertex in graph.get_vertices():
		if not visited[vertex]:
			mark_vertex(vertex, vertex, marked)
	return path


def is_hamilton(graph):
	"""Returns True if graph is Hamilton's"""
	return get_hamilton_path(graph) != []


def shortest_path(g, start = 'A', end = None):
	"""Dijkstra's algorithm for shortest path"""
	graph = g.copy()
	distances = {}
	previous = {}
	for v1 in graph.get_vertices():
		for v2 in graph.get_vertices():
			if not graph.is_edge(v1, v2):
				graph.add_edge(v1, v2, infinity)
		distances[v1] = infinity
		previous[v1] = None
	distances[start] = 0
	vertices = graph.get_vertices()
	while vertices:
		minimal = vertices[0]
		for vertex in vertices:
			if distances[vertex] < distances[minimal]:
				minimal = vertex
		del vertices[vertices.index(minimal)]
		for v in graph.get_vertices():
			if graph.is_edge(minimal, v):
				if distances[v] > distances[minimal] + graph.get_weight(minimal, v):
					distances[v] = distances[minimal] + graph.get_weight(minimal, v)
					previous[v] = minimal
	if not end:
		end = max(graph.get_vertices())
	path = [end]
	t = previous[end]
	while t:
		path.insert(0, t)
		t = previous[t]
	return distances[end], path
		
			
def get_any_spanning_tree(graph):
	"""Returns any of graph's spanning trees"""
	tree_edges = []
	def mark_vertex(k):
		visited[k] = True
		for i in graph.get_vertices():
			if graph.is_edge(i, k) and not visited[i]:
				tree_edges.append((k, i))
				mark_vertex(i)
	visited = {}
	for vertex in graph.get_vertices():
		visited[vertex] = False
	for vertex in graph.get_vertices():
		if not visited[vertex]:
			mark_vertex(vertex)
	return tree_edges


def get_minimal_spanning_tree(graph):
	"""Returns minimal spanning weight an tree using kruskal's algorithm"""
	mst_weigth, mst_edges = 0, []
	edges = graph.get_edges()
	tree_list = [[e] for e in graph.get_vertices()]
	while edges:
		minimal_weight = infinity
		for start, end, weight in edges:
			if weight < minimal_weight:
				minimal_weight = weight
				minimal_edge = (start, end, weight)
		i = 0
		for tree in tree_list:
			if minimal_edge[0] in tree:
				first_tree = tree
			if minimal_edge[1] in tree:
				second_tree = tree
			i = i + 1
		if first_tree != second_tree:
			new_tree = first_tree + second_tree
			tree_list.remove(first_tree)
			tree_list.remove(second_tree)
			tree_list.append(new_tree)
			mst_edges.append(minimal_edge[:-1])
			mst_weigth = mst_weigth + minimal_weight
		edges.remove(minimal_edge)
	return mst_weigth, mst_edges


def vertex_coloring(graph):
	"""Colors the graph and returns mapping list of vertices and colors"""
	if not graph.get_edges():
		return [(v, 1) for x in graph.get_vertices()]
	def colors_ok(colors):
		vertices = graph.get_vertices()
		for i in range(len(vertices)):
			for j in graph.adjacent_to(vertices[i]):
				color = colors[vertices.index(j)]
				if color == colors[i]:
					return False
		return True
	def upper_bound():
		vertices = graph.get_vertices()
		vx = vertices[:]
		allcol = range(len(vx))
		col = [None for x in vx]
		while vx:
			v = vx[0]
			adjacent = [ col[vertices.index(x)] for x in graph.adjacent_to(v) ]
			available = [ x for x in allcol if adjacent.count(x) == 0 ]
			col[vertices.index(v)] = min(available)
			vx.pop(0)
		return col
	def enumerate_coloring(vx, col, n):
		if vx == []:
			if colors_ok(col):
				return col
			else:
				return None
		v = vx[0]
		for i in range(n):
			c = col[:]
			c.insert(0, i)
			ret = enumerate_coloring(vx[1:], c, n)
			if ret != None:
				return ret
	col = upper_bound()
	n = 1 + max(col)
	for i in range(2, n):
		x = enumerate_coloring(graph.get_vertices(), [], i)
		if x != None:
			col = x
			break
	vertices = graph.get_vertices()
	return [(v, col[vertices.index(v)]) for v in vertices]


def get_chromatic_number(graph):
	"""Returns chromatic number of graph"""
	return 1 + max(map(lambda x: list(x)[1], vertex_coloring(graph)))


def is_planar(g):
	"""Returns True if graph is planar graph"""
	def _is_k5(g):
		return (len(g.get_vertices()) == 5) and (len(g.get_edges()) == 20)
	def _is_k33(g):
		vx = g.get_vertices()
		ex = g.get_edges()
		if (len(vx) != 6) or (len(ex) != 18):
			return False
		white = g.adjacent_to(vx[0])
		if len(white) != 3:
			return False
		black = [ x for x in vx if white.count(x) == 0 ]
		for v in white:
			x = g.adjacent_to(v)
			x.sort()
			if x != black:
				return False
		return True
	def _remove_0degs(g):
		map(g.remove_vertex, [x for x in g.get_vertices() if g.get_degree(x) == 0])
	def _remove_1degs(g):
		while True:
			vx = [x for x in g.get_vertices() if g.get_degree(x) == 1]
			if vx == []:
				return
			map(g.remove_vertex, vx)
	def _remove_2degs(g):
		while True:
			vx = [ x for x in g.get_vertices() if g.get_degree(x) == 2 ]
			if vx == []:
				return
			x = vx[0]
			conn = g.adjacent_to(x)
			if not g.is_edge(conn[0], conn[1]):
				g.add_edge(conn[0], conn[1], 1)
			g.remove_vertex(x)
	def _remove_edges(g):
		_remove_1degs(g)
		_remove_2degs(g)
		_remove_0degs(g)
	def _non_planar_by_limit(g):
		n = len(g.get_vertices())
		m = len(g.get_edges()) / 2
		if n < 3:
			return False
		if m > (3 * n - 6):
			return True
		return False
	def _non_planar(g, l):
			if l == []:
				return _is_k5(g) or _is_k33(g)
			if _non_planar_by_limit(g):
				return True
			if _non_planar(g.copy(), l[1:]):
				return True
			a, b, w = l[0]
			g.remove_edge(a, b)
			_remove_edges(g)
			l = [(a,b,w) for a, b, w in l[1:] if g.is_edge(a, b)]
			return _non_planar(g, l)
	def _less_than_6deg(g):
		return [1 for x in g.get_vertices() if g.get_degree(x) < 6] != []
	g = g.copy()
	_remove_edges(g)
	if not _less_than_6deg(g):
		return False
	ex = [(a,b,w) for a, b, w in g.get_edges() if (a < b)]
	return not _non_planar(g, ex)


def get_pairs_number(graph):
	"""Counts the number of solutions to the pairing"""
	if get_chromatic_number(graph) != 2:
		raise GraphError, "Not a bipartite graph"
	colors = vertex_coloring(graph)
	girls = [v for v,c in colors if c == 0]
	boys = [v for v,c in colors if c == 1]
	def _slave(g, b):
		if g == []:
			return 1
		available = [ x for x in graph.adjacent_to(g[0]) if b.count(x) != 0 ]
		if available == []:
			return 0
		cnt = 0
		for x in available:
			cnt = cnt + _slave(g[1:], [ y for y in b if y != x ])
		return cnt
	return _slave(girls, boys)


def get_max_flow(graph):
	"""Finds the maximum flow in the network"""
	if not graph.is_digraph():
		raise GraphError, "Graph is not directed"
	flow = Graph(True)
	map(lambda x: flow.add_vertex(), graph.get_vertices())
	map(lambda x: flow.add_edge(list(x)[0],list(x)[1],0), graph.get_edges())
	def _connected_to(v):
		return flow.adjacent_to(v)
	def _connected_from(v):
		return [a for a, b, w in flow.get_edges() if b == v]
	def _is_sink(v):
		return flow.adjacent_to(v) == []
	def _path_append(path, start, end, weight, direction):
		p = path[:]
		p.append((start, end, weight, direction))
		return p
	def _augument_flow(path):
		if path == []:
			return
		f = list(path[0])[2]
		for a, b, w, d in path:
			if f > w:
				f = w
		for a, b, w, d in path:
			if d:
				flow.add_edge(a, b, f)
			else:
				flow.add_edge(a, b, -1 * f)
	def _visited(v, path):
		for a, b, w, d in path:
			if (a == v) or (b == v):
				return True
		return False
	def _slave(v, path):
		vx = [x for x in _connected_to(v) if not _visited(x, path) ]
		if vx == []:
			if not _is_sink(v):
				return
			_augument_flow(path)
			return
		for x in vx:
			free = graph.get_weight(v, x) - flow.get_weight(v, x)
			if free > 0:
				_slave(x, _path_append(path, v, x, free, True))
		vx = [x for x in _connected_from(v) if not _visited(x, path) ]
		for x in vx:
			free = flow.get_weight(x, v)
			if free > 0:
				_slave(x, _path_append(path, v, x, free, False))
	vx = flow.get_vertices()
	if vx == []:
		return flow
	_slave(vx[0], [])
	return flow


def get_max_flow_val(graph):
	"""Returns the value of a maximum flow of"""
	flow = get_max_flow(graph)
	vx = flow.get_vertices()
	if vx == []:
		return 0
	return flow.get_out_degree(vx[0])
