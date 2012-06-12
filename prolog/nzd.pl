nzd(X, X, X).
nzd(X, Y, R):-
	X < Y,
	T is Y - X,
	nzd(X, T, R).
nzd(X, Y, R):-
	X > Y,
	nzd(Y, X, R).
