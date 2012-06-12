; vraca n-ti element liste
; (C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fe.hr>

(defun element (n lista)
	(cond ((zerop n) (car lista))
		  (t (element (1- n) (cdr lista)))
	)
)

(setq l '(1 2 3 4 5 6 7 8 9 10))
(print (element 5 l))
