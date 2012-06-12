#include <iostream>

#include "genetic.h"

using namespace std;

int main(int argc, char **argv)
{
	GeneticAlgorithm ga(1);

	cout << "Pocetne vrijednosti: " << endl;
	ga.evaluate();
	ga.print();

	for (int i = 0 ; i < iterations_num * population_size ; ++i) {
		ga.evaluate();
		ga.select();
		ga.breed();
		ga.mutate();
	}

	cout << "Vrijednosti nakon " << dec << iterations_num << " iteracija genetskog algoritma: " << endl;
	ga.print();
	ga.result();
	
	return 0;
}
