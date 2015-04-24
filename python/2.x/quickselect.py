import random

def quickselect(array, k):
    pivot = array[len(array) / 2]
    less_than = [e for e in array if e < pivot]
    more_than = [e for e in array if e > pivot]
    if k <= len(less_than):
        return quickselect(less_than, k)
    elif k > len(array) - len(more_than):
        return quickselect(more_than, k - (len(array) - len(more_than)))
    else:
        return pivot

