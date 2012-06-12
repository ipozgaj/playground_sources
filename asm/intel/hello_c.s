; hello world program koji koristi C library
; kompajlirati sa:
;	nasm -felf hello_c.s
;	gcc hello_c.o
; (C) 2004  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

global main
extern printf

main:
	push dword msg
	call printf
	add esp, 4			; ukloni parametre sa stoga
	ret

msg db 'Hello world', 10
