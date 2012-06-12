// Primjer koristenja STL-a (liste, sort i iteratori) za sortiranje stringova
// (C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

#include <list>
#include <string>
#include <iostream>
#include <iterator>

using namespace std;

int main (int argc, char **argv)
{
	string tmp;
	list<string> input;
	
	while (cin >> tmp)
		input.push_back (tmp);
	
	input.sort();
	
	ostream_iterator<string> out_iter_string (cout, " ");
	copy (input.begin(), input.end(), out_iter_string);
	
	cout << endl;
	
	return 0;
}
