// vim: ts=4

#ifndef GENETIC_H
#define GENETIC_H

#include "chromosome.h"

using namespace std;

static const int iterations_num          = 10000;
static const int population_size         = 10;
static const int chromosomes_to_breed    = 3;
static const double mutation_probability = 0.2;

class GeneticAlgorithm {
		int n, max_index;
		int indexes[3], parms[10];
		unsigned int threshold;
		double *func_values;
		Chromosome ***chromosoms;
		short function;
		
	public:
		GeneticAlgorithm(int);
		~GeneticAlgorithm();
		void evaluate();
		void select();
		void breed();
		void mutate();
		void result();
		void print();
};

#endif // GENETIC_H
