; clrscr.asm - very fast and small "clear screen" program
; assemble and link with:
;	nasm -f elf clrscr.asm ; ld -s clrscr.o
;
; (C) 2004  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

global _start
	
section .text

_start:
	xor	eax, eax
	mov	al, 4			; write call
	xor	ebx, ebx
	inc	ebx			; write to stdout
	mov	ecx, f_str		; pointer
	xor	edx, edx		; format string
	mov	dl, len			; format string length
	int	0x80			; execute call
	xor	eax, eax
	inc	eax			;  exit call
	xor	ebx, ebx		; return 0 to shell
	int	0x80			; execute call

; format string "\033[2J\033[1;1H" and it's length
f_str 	db	0x1B, 0x5B, 0x32, 0x4A, 0x1B, 0x5B, 0x3B, 0x31, 0x3B, 0x48
len	equ	$ - f_str
