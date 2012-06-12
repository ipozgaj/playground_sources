roditelj(mirko,zdenka).
roditelj(marija,vladimir).
roditelj(vladimir,igor).
roditelj(vladimir,alen).
roditelj(zdenka,igor).
roditelj(zdenka,alen).

spol(igor,musko).
spol(alen,musko).
spol(vladimir,musko).
spol(mirko,musko).
spol(marija,zensko).
spol(zdenka, zensko).

musko(X):- spol(X,musko).
zensko(X):- spol(X,zensko).

majka(X,Y):-
	roditelj(X,Y),
	zensko(X).

otac(X,Y):-
	roditelj(X,Y),
	musko(X).

djed(X,Y):-
	otac(X,Z),
	roditelj(Z,Y).

baka(X,Y):-
	majka(X,Z),
	roditelj(Z,Y).
