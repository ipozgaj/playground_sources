// turing.cpp
// Samostalni projekt iz Automata, formalnih jeuika i jezicnih procesora 1
// Prihvacanje jezika koji sadrzni nizove oblika a^ib^ic^i
// (C) 2004  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;

const unsigned int broj_stanja = 7;
const unsigned int broj_znakova_trake = 7;
const unsigned int maksimalna_velicina_trake = 1000;

struct TStanje {
	int stanje;
	char znak;
	char pomak;
};

const TStanje KRAJ = {-1, 0, 0};

TStanje prijelazi[broj_stanja][broj_znakova_trake] = {
	{{1,'A','R'}, KRAJ, KRAJ, KRAJ, KRAJ, KRAJ, KRAJ},
	{{1,'a','R'}, {2,'B','R'}, KRAJ, KRAJ, {1,'B','R'}, KRAJ, KRAJ},
	{KRAJ, {2,'b','R'}, {3,'C','L'}, KRAJ, KRAJ, {2,'C','R'}, KRAJ},
	{{3,'a','L'},{3,'b','L'}, {2,'c','L'}, {4,'A','R'}, {3,'B','L'}, {3,'C','L'},{3,' ','L'}},
	{{1,'A','R'}, KRAJ, KRAJ, KRAJ, {5,'B','R'}, KRAJ, {5,' ','L'}},
	{KRAJ, KRAJ, KRAJ, KRAJ, {5,'B','R'}, {5,'C','R'}, {6,' ','L'}},
	{KRAJ, KRAJ, KRAJ, KRAJ, KRAJ, KRAJ, KRAJ}
};

class Turing {
	private:
		char *traka;
		int stanje;
		int polozaj_glave;
		int prihvatljivo_stanje;
			
	public:
		Turing (char *ulazni_niz);
		~Turing ();
		bool korak ();
		void simuliraj ();
		void ispisi_stanje ();
};

Turing::Turing (char *ulazni_niz)
{
	traka = strdup (strcat (ulazni_niz, " "));
	stanje = 0;
	polozaj_glave = 0;
	prihvatljivo_stanje = 6;
}

Turing::~Turing ()
{
	delete traka;
}

void Turing::ispisi_stanje ()
{
	cout << "Stanje: " << stanje << '\t';
	cout << "Traka: " << traka << endl;
	cout << "         " << '\t' << "       ";

	for (int i=0 ; i<polozaj_glave ; ++i) cout << " ";
	
	cout << "^" << endl;
}

bool Turing::korak ()
{
	int i;
	char ulazni_znak = traka[polozaj_glave];

	switch (ulazni_znak) {
			case 'a': i = 0; break;
			case 'b': i = 1; break;
			case 'c': i = 2; break;
			case 'A': i = 3; break;
			case 'B': i = 4; break;
			case 'C': i = 5; break;
			case ' ': i = 6; break;
			default: abort();
	}
	
	if (prijelazi[stanje][i].stanje == -1) 
		return false;
			
	traka[polozaj_glave] = prijelazi[stanje][i].znak;

	if (prijelazi[stanje][i].pomak == 'R') 
		++polozaj_glave;
	else
		--polozaj_glave;
	
	stanje = prijelazi[stanje][i].stanje;

	return true;
}

void Turing::simuliraj ()
{
	ispisi_stanje ();
		
	while (korak ()) 
		ispisi_stanje ();

	if (stanje == prihvatljivo_stanje) 
		cout << "Niz se prihvaca" << endl;
	else
		cout << "Niz se ne prihvaca" << endl;
}

int main ()
{
	char s[maksimalna_velicina_trake];
		
	cout << "Unesi ulazni niz: ";
	cin >> s;
	
	for (unsigned int i=0 ; i<strlen(s) ; ++i)
		if (s[i] != 'a' && s[i] != 'b' && s[i] != 'c') {
			cerr << "Nije unesen ispravan niz" << endl;
			exit (EXIT_FAILURE);
		}
	
	Turing *ts = new Turing (s);
	
	ts->simuliraj();
	
	delete ts;
	
	return EXIT_SUCCESS;
}
