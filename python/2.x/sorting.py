import random
import time

def selection_sort(array):
	"""Insertion sort
	Best case:        O(n^2)
	Worst case:       O(n^2)
	Average case:     O(n^2)
	Space complexity: O(1)
	"""
	for i in range(len(array) - 1):
		min = i
		for j in range(i + 1, len(array)):
			if array[j] < array[min]:
				min = j
		array[i], array[min] = array[min], array[i]
	return array


def insertion_sort(array):
	"""Insertion sort
	Best case:        O(n)
	Worst case:       O(n^2)
	Average case:     O(n^2)
	Space complexity: O(1)
	"""
	for i in range(1, len(array)):
		key = array[i]
		j = i
		while j > 0 and array[j - 1] > key:
			array[j] = array[j - 1]
			j = j - 1
		array[j] = key
	return array
		

def shell_sort(array):
	"""Shell sort
	Best case:        O(n)
	Average case:     O(n^3/2)
	Space complexity: O(1)
	"""
	step = len(array) / 2
	while step > 0:
		for i in range(step, len(array)):
			key = array[i]
			j = i
			while j > 0 and array[j - step] > key:
				array[j] = array[j - step]
				j = j - step
			array[j] = key
		step = step / 2
	return array


def bubble_sort(array):
	"""Bubble sort
	Best case:        O(n)
	Average case:     O(n^2)
	Space complexity: O(1)
	"""
	for i in range(len(array)):
		for j in range(len(array) - 1, i, -1):
			if array[i] > array[j]:
				array[i], array[j] = array[j], array[i]
	return array
	

def merge_sort(array):
	"""Merge sort
	Best case:        O(n log n)
	Average case:     O(n log n)
	Worst case:       O(n log n)
	"""
	def merge(left, right):
		i, j = 0, 0
		result = []
		while i < len(left) and j < len(right):
			if left[i] <= right[j]:
				result.append(left[i])
				i = i + 1
			else:
				result.append(right[j])
				j = j + 1
		result += left[i:]
		result += right[j:]
		return result
	if len(array) <= 1:
		return array
	else:
		middle = len(array) / 2
		left = merge_sort(array[:middle])
		right = merge_sort(array[middle:])
		return merge(left, right)
	

def heap_sort(array):
	"""Heap sort
	Best case:    O(n log n)
	Worst case:   O(n log n)
	Average case: O(n log n)
	"""
	def heapify(list, root, last):
		left = 2 * root
		right = 2 * root + 1
		if left <= last and list[left] > list[root]:
			largest = left
		else:
			largest = root
		if right <= last and list[right] > list[largest]:
			largest = right
		if largest != root:
			list[root], list[largest] = list[largest], list[root]
			heapify(list, largest, last)
	def build_heap(list):
		n = len(list) - 1
		for i in range(n / 2, -1, -1):
			heapify(list, i, n)
	build_heap(array)
	size = len(array) - 1
	for i in range(size, 0, -1):
		array[0], array[i] = array[i], array[0]
		size = size - 1
		heapify(array, 0, size)
	return array
		

def quick_sort(array):
	"""Quick sort
	Best case:        O(n log n)
	Average case:     O(n log n)
	Worst case:       O(n^2)
	"""
	if array == []: return []
	return quick_sort([e for e in array[1:] if e <= array[0]]) + array[0:1] + quick_sort([e for e in array[1:] if e > array[0]])
	

if __name__ == '__main__':
	array = range(4096)
	random.shuffle(array)

	for sort in [insertion_sort, selection_sort, bubble_sort, shell_sort, merge_sort, heap_sort, quick_sort]:
		start = time.time()
		sorted_array = sort(array[:])
		end = time.time()
		print "%s: %f" % (sort.__name__.ljust(16), end - start)
		if sorted_array != sorted(array):
			print "SORT NOT OK!"

