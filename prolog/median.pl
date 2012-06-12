% vraæa median liste (srednji element)
% rjesenje strukturnom rekurzijom i suceljnim predikatom

median([_], [E1 | _], Res):-
	Res is E1.
	
median([_,_], [E1, E2 | _], Res):-
	Res is (E1 + E2) / 2.
	
median([_,_|R1], [_|R2], Res):-
	median(R1, R2, Res).
	
median(L, R):-
	median(L, L, R).
