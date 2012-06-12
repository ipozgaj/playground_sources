; Kreira datoteku navedenu iza imena programa i upisuje 'hello world' u nju
; (C) 2004 Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

msg db 'Hello world'
len equ $-msg

global _start

_start:
	pop ebx			; argc (argumenti komandne linije se nalaze na stogu)
	pop ebx			; argv[0] (ime programa)
	pop ebx			; prvi argument (ime datoteke)
	
	mov eax, 8		; linux 'creat' poziv (ebx vec sadrzi pointer na ime datoteke!)
	mov ecx, 0644Q		; dozvole (-rw-r--r--)
	int 0x80

	test eax, eax		; provjeri da li je file deskriptor ispravan
	js error		; ako je postavljena zastavica 'sign' doslo je do greske

	call write_text

error:
	mov eax, 1		; linux 'exit' poziv
	mov ebx, 0		; izlazni kod
	int 0x80

write_text:
	mov ebx, eax		; 'creat' poziv je stavio file deskriptor u eax. Stavi ga u ebx
	mov eax, 4		; linux 'write' poziv
	mov ecx, msg		; pointer na poruku
	mov edx, len		; duljina poruke
	int 0x80

	mov eax, 6		; linux 'close' poziv (ebx vec sadrzi file deskriptor)
	int 0x80

	ret
