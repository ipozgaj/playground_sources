% provjerava da li je element unutar liste
element(X,[X|_]).
element(X,[_|R]):-
	element(X,R).

povezi([],L,L).
povezi([H1|R1],L2,[H1|R3]):-
	povezi(R1,L2,R3).

dodaj(X,L,[X|L]).

obrisi(X,[X|R], R).
obrisi(X,[H|R], [H|R2]):-
	obrisi(X,R,R2).

umetni(X,L1,L2):-
	obrisi(X,L2,L1).

podlista(P,L):-
	povezi(_,S,L),
	povezi(P,_,S).

permutacija([],[]).
permutacija([Prvi|Ostali],P):-
	permutacija(Ostali,P1),
	umetni(Prvi,P1,P).
	
