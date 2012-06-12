#!/usr/bin/python
# sudoku.py - sudoku solver
# (C) 2006  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

import sys

def get_square(table, row, column):
	"""Returns 3x3 square containing element at table[row:column]"""
	row_start = 3 * (row / 3)
	row_end = row_start + 3
	column_start = 3 * (column / 3)
	column_end = column_start + 3
	return [x[column_start:column_end] for x in table[row_start:row_end]]

def move_posible(element, table, row, column):
	"""Checks if move is posible"""
	if table[row][column] <> 'X':
		return False
	if element in table[row]:
		return False
	if element in [e[column] for e in table]:
		return False
	for r in get_square(table, row, column):
		if element in r:
			return False
	return True

def solved(table):
	"""Checks if no more moves are posible"""
	for row in table:
		if 'X' in row:
			return False
	return True

def unique_move(element, table, row, column):
	"""Checks if move is unique (there is no other place
	in row or column in which given element can go"""
	if not move_posible(element, table, row, column):
		return False
	moves_posible = 0
	for i in range(9):
		if move_posible(element, table, row, i):
			moves_posible = moves_posible + 1
	if moves_posible == 1:
		return True
	moves_posible = 0
	for i in range(9):
		if move_posible(element, table, i, column):
			moves_posible = moves_posible + 1
	if moves_posible == 1:
		return True
	return False

def solution(table):
	"""Solves sudoku puzzle given with table"""
	while not solved(table):
		for i in range(9):
			for j in range(9):
				for num in range(1,10):
					if unique_move(str(num), table, i, j):
						table[i][j] = str(num)
						break
	return table

def output(table):
	"""Nicer output of solution"""
	table.insert(3, [])
	table.insert(7, [])
	for row in table:
		row.insert(3, " ")
		row.insert(7, " ")
		print " ".join(row)

if __name__ == "__main__":
	try:
		src = file(sys.argv[1])
	except:
		print 'Syntax: sudoku.py <data file>'
		sys.exit(1)

	table = [element.rstrip().split(' ') for element in src]
	s = solution(table)
	output(s)
