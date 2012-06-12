; ispis datoteke
; (C) 2004  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

(defun file-dump (file-name)
	(setq input-stream (open file-name))
	(loop while (setq line (read-line input-stream nil)) do (write-line line))
	(close input-stream)
)

(format t "Enter filename: ")
(setq file-name (read))
(file-dump file-name)
