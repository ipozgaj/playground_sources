; racunanje faktorijela
; (C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

(defun fact (n)
	(cond ((zerop n) 1)
		  (t (* n (fact (1- n))))
	)
)

(format t "N = ")
(setq var (read))
(print (fact var))
