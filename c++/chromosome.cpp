// vim: ts=4

#include <cmath>

#include "chromosome.h"

using namespace std;

Chromosome::Chromosome(int val)
{
	value = val;
//	binary_val = static_cast<double> ((val - min_value) / value_range * (pow(2.0, n) - 1));
	binary_val = static_cast<unsigned int>((static_cast<double> (val - min_value) / value_range * (pow(2.0, n) - 1)));
}

void Chromosome::calculate_value()
{
//	value = min_value + (static_cast<double> (binary_val)) / (pow(2.0, n) - 1) * value_range;
	value = min_value + (static_cast<double>(binary_val)) / (pow(2.0, n) - 1) * value_range;
}

int Chromosome::n = static_cast<int>(ceil(log(value_range * pow(10.0, precision) + 1.0) / log(2.0)));
