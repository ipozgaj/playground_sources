% umnozak elemenata liste, primjer optimizacije poziva
% umnozak(+L, ?R)

umnozak([], 1).
umnozak([H|R], Res):-
	umnozak(R, Tmp),
	Res is H * Tmp.

umnozak2([H|R], Acc, Res):-
	Tmp is Acc * H,
	umnozak2(R, Tmp, Res).
umnozak2([], X, X).
umnozak2(L, R):-
	umnozak2(L, 1, R).

umnozak3p([H|R], Res):-
	Res = H * Tmp,
	umnozak3p(R, Tmp).
umnozak3p([], 1).
umnozak3(L, Res):-
	umnozak3p(L, Tmp),
	Res is Tmp.
