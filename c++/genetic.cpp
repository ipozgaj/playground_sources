#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include "genetic.h"

using namespace std;

GeneticAlgorithm::GeneticAlgorithm(int func_num)
{
	srand(static_cast<unsigned>(time(NULL)));
	
	function = func_num;
	
	n = static_cast<int> (ceil(log(value_range * pow(10.0, precision) + 1) / log(2.0)));
	threshold = static_cast<unsigned int> (pow(2.0, n) - 1);
	chromosoms = new Chromosome**[population_size];

	for (int i = 0 ; i < population_size ; ++i) {
		chromosoms[i] = new Chromosome*[dimension];
		for (int j = 0 ; j < dimension ; ++j)
			chromosoms[i][j] = new Chromosome((rand() % value_range) + min_value);
	}

	func_values = new double[population_size];

	cout << "Bounds: [" << min_value << ", " << max_value << "]" << endl;
	cout << "Population size: " << population_size << endl;
	cout << "Iterations number: " << iterations_num << endl;
	cout << "------------------------------------------" << endl;

	if (function == 1) {           
		ifstream in("parametri.dat");
		if(!in) {       
			cerr << "No such file: parametri.dat" << endl;
			exit(EXIT_FAILURE);
		}

		for (int i = 0 ; i < dimension ; ++i)
			in >> parms[i];
	}
}

GeneticAlgorithm::~GeneticAlgorithm()
{
}

void GeneticAlgorithm::evaluate()
{
	for (int i = 0 ; i < population_size ; ++i) {
		double sum = 0;
		func_values[i] = 0;

		switch (function) {
			case 1:
				for (int j = 0 ; j < dimension ; ++j)
					func_values[i] += pow(chromosoms[i][j]->value - parms[j], 2.0);
				break;
				
			case 2:
				func_values[i] = fabs(static_cast<double> ((chromosoms[i][0]->value - chromosoms[i][1]->value) * (chromosoms[i][0]->value + chromosoms[i][1]->value))) + pow(pow(chromosoms[i][0]->value, 2.0) + pow(chromosoms[i][1]->value, 2.0), 0.5);
				break;

			case 3:
				for (int j = 0 ; j < dimension ; ++j)
					sum += pow(chromosoms[i][j]->value, 2.0);
				func_values[i] = -0.5 + (pow(sin(pow(sum, 0.5)), 2.0) - 0.5) / pow((1.0 + 0.001 * sum), 2.0);
				break;

			case 4:
				for (int j = 0 ; j < dimension ; ++j) 
					sum += pow(chromosoms[i][j]->value, 2.0);
				func_values[i] = pow(sum, 0.25) * (pow(sin(50 * pow(sum, 0.1)), 2.0) + 1.0);
				break;
		}
	}
}

void GeneticAlgorithm::select()
{
	for (int iteration = 0 ; iteration < chromosomes_to_breed ; ++iteration) {
		indexes[iteration] = rand() % population_size;
		for (int i = iteration - 1 ; i >= 0 ; --i)
			if (indexes[iteration] == indexes[i])
				--iteration;
	}
}

void GeneticAlgorithm::breed()
{
	int x, y;
	unsigned int ch1, ch2, ch3;

	max_index = indexes[0];
	
	for (int i = 1 ; i < chromosomes_to_breed ; ++i)
		if (func_values[indexes[i]] > func_values[max_index])
			max_index = indexes[i];
	
	for (int i = 0 ; i < chromosomes_to_breed ; ++i)
		if (indexes[i] != max_index) {
			x = indexes[i];
			if (indexes[i++] != max_index)
				y = indexes[i];
			else
				y = indexes[i + 1];
			break;
		}

	for (int k = 0 ; k < dimension ; ++k) {
		do {
			ch1 = chromosoms[x][k]->binary_val & chromosoms[y][k]->binary_val;
			ch3 = rand() % threshold;
			ch2 = ch3 & (chromosoms[x][k]->binary_val ^ chromosoms[y][k]->binary_val);
			chromosoms[max_index][k]->binary_val = ch1 | ch2;
		} while (chromosoms[max_index][k]->binary_val > threshold || chromosoms[max_index][k]->binary_val < 0);
		
		chromosoms[max_index][k]->calculate_value();
	}
}

void GeneticAlgorithm::mutate()
{
	unsigned int ch1;

	for (int j = 0 ; j < dimension ; ++j) {
		for (int k = 0 ; k < n ; ++k)
			if ((rand() % static_cast<int>(1 / mutation_probability)) == 0) {
				ch1 = static_cast<unsigned> (pow(2.0, k));
				chromosoms[max_index][j]->binary_val = chromosoms[max_index][j]->binary_val ^ ch1;
			}
		chromosoms[max_index][j]->calculate_value();
	}
}

void GeneticAlgorithm::result()
{
	int min_index = 0;
	double min_val;

	evaluate();
	min_val = func_values[0];
	
	for (int i = 1 ; i < population_size ; ++i)
		if (func_values[i] < min_val) {
			min_val = func_values[i];
			min_index = i;
		}
	
	cout << "Function minimum: " << min_val << endl;
	cout << "Minimum point: ";
	for (int i = 0 ; i < dimension ; ++i)
		cout << chromosoms[min_index][i]->value << " ";
	cout << endl;
}

void GeneticAlgorithm::print()
{
	cout.width(5);
	cout.precision(2);
		
	cout << "Dec\tHex\t\tDec\tHex\t\tF(X)" << endl;
	for(int i = 0 ; i < population_size ; ++i) {
		for(int j = 0 ; j < dimension ; ++j) {
			cout << chromosoms[i][j]->value << "\t";
			cout << hex << chromosoms[i][j]->binary_val << "\t\t";
		}
		
		cout << func_values[i] << endl;
	}

	cout << endl;
}
