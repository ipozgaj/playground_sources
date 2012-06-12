; 76 bajtova dugacak linux program
; nasm -f bin 76_bytes.asm ; chmod +x program
; (C) 2004 Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

BITS	32
	org	0x08048000

ehdr:	db	0x7f, "ELF"
	db 	1, 1, 1, 0, 0
_start:	mov	bl, 42
	xor	eax, eax
	inc	eax
	int	0x80
	dw	2
	dw	3
	dd	1
	dd	_start
	dd	phdr - $$
	dd	0
	dd	0
	dw	ehdrsize
	dw	phdrsize
phdr:	dd	1
	dd	0

ehdrsize	equ	$ - ehdr

	dd	$$
	dd	$$
	dd	filesize
	dd	filesize
	dd	5
	dd	0x1000

phdrsize	equ	$ - phdr
filesize	equ	$ - $$
