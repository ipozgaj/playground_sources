; Vraca true ako je zadani element u listi, inace false
; (C) 2004  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

(defun is-member (element lista)
	(cond ((not (car lista)) nil)
		((eq (car lista) element) t)
		(t (is-member element (cdr lista)))
	)
)

(setq brojevi '(1 2 3 4 5 6 7 8 9 10))
(print (is-member 5 brojevi))
(print (is-member 666 brojevi))
