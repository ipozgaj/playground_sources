#!/usr/bin/python
# izbacivanje beskorisnih (mrtvih i nedohvatljivih) 
# znakova iz kontekstno neovisne gramatike
#
# (C) 2004  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

from sys import argv, exit

produkcije = {}

def	izbaci_mrtve_znakove ():
	nova_lista_zivih = []
	stara_lista_zivih = []

	for nezavrsni_znak in produkcije.keys ():
		for desna_strana_produkcije in produkcije[nezavrsni_znak]:
			if desna_strana_produkcije == '$' or desna_strana_produkcije.islower():
				nova_lista_zivih.append (nezavrsni_znak)
				break

	while stara_lista_zivih <> nova_lista_zivih:
		stara_lista_zivih = nova_lista_zivih[:]
		for nezavrsni_znak in produkcije.keys():
			for desna_strana_produkcije in produkcije[nezavrsni_znak]:
				znak_ziv = 1
				for ch in desna_strana_produkcije:
					if ch.isupper() and ch not in nova_lista_zivih:
						znak_ziv = 0
						break
				if znak_ziv:
					if nezavrsni_znak not in nova_lista_zivih:
						nova_lista_zivih.append (nezavrsni_znak)

	for nezavrsni_znak in produkcije.keys():
		if nezavrsni_znak not in nova_lista_zivih:
			del produkcije[nezavrsni_znak]
		else:
			for desna_strana in produkcije[nezavrsni_znak]:
				for znak in desna_strana:
					if znak.isupper() and znak not in nova_lista_zivih:
						produkcije[nezavrsni_znak].remove(desna_strana)

def izbaci_nedohvatljive_znakove ():
	stara_lista_dohvatljivih = []
	nova_lista_dohvatljivih = ['S']

	while stara_lista_dohvatljivih <> nova_lista_dohvatljivih:
		stara_lista_dohvatljivih = nova_lista_dohvatljivih[:]
		for nezavrsni_znak in produkcije.keys():
			for dohvatljiv_znak in stara_lista_dohvatljivih:
				for desna_strana_produkcije in produkcije[dohvatljiv_znak]:
					if nezavrsni_znak in desna_strana_produkcije:
						if nezavrsni_znak not in nova_lista_dohvatljivih:
							nova_lista_dohvatljivih.append (nezavrsni_znak)

	for nezavrsni_znak in produkcije.keys():
		if nezavrsni_znak not in nova_lista_dohvatljivih:
			try:
				del produkcije[nezavrsni_znak]
			except:
				pass


try:
	ulaz = open (argv[1], 'r')
except IOError:
	print 'Ne mogu otvoriti ulaznu datoteku: ' + argv[1]
	exit (1)
except IndexError:
	print 'Sintaksa: ' + argv[0] + ' ulazna_datoteka izlazna_datoteka'
	exit(1)

while 1:
	try:
		nova_produkcija = ulaz.readline ()
		
		if not nova_produkcija:
			raise EOFError
		
		if nova_produkcija[:1] not in produkcije.keys ():
			produkcije[nova_produkcija[:1]] = [];
		
		produkcije[nova_produkcija[:1]].append (nova_produkcija[2:-1])
			
	except EOFError:
		break

izbaci_mrtve_znakove ()
izbaci_nedohvatljive_znakove ()

try:
	izlaz = open (argv[2], 'w')
except IOError:
	print 'Ne mogu otvoriti izlaznu datoteku' + argv[2]
	exit (1)
except IndexError:
	print 'Sintaksa: ' + argv[0] + ' ulazna_datoteka izlazna_datoteka'
	exit(1)

for nezavrsni_znak in produkcije.keys():
	for desna_strana in produkcije[nezavrsni_znak]:
		izlaz.write (nezavrsni_znak + '~' + desna_strana + '\n')
