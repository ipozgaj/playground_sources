# various dynamic programming problems

def get_maxsum_subsequence(array):
	maximized_sums = [0] * len(array)
	maximized_sequence = []
	for index, element in enumerate(array):
		if index == 0:
			maximized_sums[0] = element
			maximized_sequence = [element]
		else:
			maximized_sums[index] = max(maximized_sums[index-1] + element, array[index])
			if maximized_sums[index-1] + element > array[index]:
				maximized_sequence.append(element)
			else:
				maximized_sequence = [element]
	return max(maximized_sums), maximized_sequence

print get_maxsum_subsequence([1, 0, 2, 5, -10, 1, -1, 2, -1, 8])

