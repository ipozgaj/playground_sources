; racunanje fibbonacijevih brojeva
; (C) 2004  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

(defun fibbonaci (n)
	(cond ((or (eq n 1) (eq n 2)) 1)
		  (t (+ (fibbonaci (1- n)) (fibbonaci (- n 2))))
	)
)

(format t "N = ")
(setq var (read))
(print (fibbonaci var))
