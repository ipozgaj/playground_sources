/* Quicksort implementation in Java
 * 
 * (C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr> 
 */

import java.util.Random;

class SortExample {
	public static void main (String[] args) {
		try {
			QuickSort qs = new QuickSort (Integer.parseInt (args[0]));
			qs.fillArray();
			qs.printArray();
			qs.sortArray();
			qs.printArray();
		} catch (Exception e) {
			System.out.println ("You must specify a number of elements");
			System.exit (1);
		}
	}
}

class QuickSort {
	private	int array[];
	private int size;

	public QuickSort (int n) 
	{
		size = n;
		array = new int[size];
	}

	public void fillArray () 
	{
		int i;
		Random rnd = new Random();

		for (i = 0 ; i < size ; ++i)
			array[i] = rnd.nextInt (size + 1);
	}

	public void qsort (int left, int right)
	{
		int i, j, v, tmp;

		if (right > left)
		{
			v = array[right];
			i = left - 1;
			j = right;

			while (true) {
				while (array[++i] < v);
				while (array[--j] > v);
	
				if (i >= j) break;
				
				tmp = array[i];
				array[i] = array[j];
				array[j] = tmp;
			}
		
			tmp = array[i];
			array[i] = array[right];
			array[right] = tmp;

			qsort (left, i - 1);
			qsort (i + 1, right);
		}
	}
	
	public void sortArray ()
	{
		qsort (0, size - 1);
	}

	public void printArray ()
	{ 
		int i;

		for (i = 0 ; i < size ; ++i)
			System.out.print (array[i] + " ");

		System.out.println ("\n");
	}
}
