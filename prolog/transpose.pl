% transponiranje matrice
% transponiraj(+L, ?Rezultat)

prvi([], [], []).
prvi([[Prvi|Ostatak]|Ostali], [Ostatak|OstatakDodani], [Prvi|PrviDodani]):-
	prvi(Ostali, OstatakDodani, PrviDodani).

transponiraj([[]|_], []).
transponiraj(L, [ResPrvi|ResOstatak]):-
	prvi(L, Ostatak, ResPrvi),
	transponiraj(Ostatak, ResOstatak).
