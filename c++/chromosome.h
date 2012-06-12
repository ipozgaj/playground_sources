// vim: ts=4

#ifndef CHROMOSOME_H
#define CHROMOSOME_H

static const int dimension = 2;
static const int min_value = -100;
static const int max_value = 100;
static const int precision = 4;
static const int value_range = max_value - min_value;

class Chromosome
{
	public:
		double value;
		unsigned int binary_val;
		static int n;
		
		Chromosome(int);
		void calculate_value();
};

#endif // CHROMOSOME_H
