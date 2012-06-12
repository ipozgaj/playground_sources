change_one(X, X, Y, Y):-!.
change_one(X, _, _, X).
change2([H|R], S, D, Tmp, Res):-
	change_one(H, S, D, Changed),
	change2(R, S, D, [Changed|Tmp], Res).
change2([], _, _, R, R).
change(L, S, D, R):-
	change2(L, S, D, [], R1),
	reverse(R1, R).
	
