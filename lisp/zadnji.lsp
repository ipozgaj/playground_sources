(defun zadnji(lista)
	(cond ((not (cdr lista)) (car lista))
		(t (zadnji (cdr lista)))
	)
)

(setq brojevi '(2 4 6 8 3 5))
(print (zadnji brojevi))
