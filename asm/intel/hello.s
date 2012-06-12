; hello world program pisam u asembleru (linux, nasm)
; kompajlirati sa:
;	nasm -f elf hello.s
;	ld hello.o
; (C) 2004  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

msg db 'Hello world', 0xa
len equ $ - msg

global _start

_start:
	mov eax, 4		; linux 'write' poziv
	mov ebx, 1		; file descriptor (0 = stdin, 1 = stdout, 2 = stderr)
	mov ecx, msg		; pokazivac na spremnik
	mov edx, len		; duljina buffera
	int 0x80		; izvrsi kernel poziv

	mov eax, 1		; linux 'exit' poziv
	mov ebx, 0		; izlazni kod je 0
	int 0x80
