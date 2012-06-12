; raèunanje fibbonacijevih brojeva
; ecx = koliko fibbonacijevih brojeva treba ispisati
; eax i ebx = registri za fibbonacijeve brojeve
; (C) 2004 Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

global main
extern printf
extern fflush

main:
	push ebx

	mov ecx, 30		; racunaj prvih 30 brojeva
	
	xor eax, eax		; ocisti eax
	xor ebx, ebx		; ocisti ebx
	inc ebx			; prvi fibbonacijev broj
	
print_num:
	push eax		; spremi eax i ecx
	push ecx
	
	push eax		; za ispis koristimo C biblioteku
	push dword format
	call printf
	add esp, 8		; ukloni parametre sa stoga
	
	pop ecx			; vrati ecx i eax
	pop eax

	mov edx, ebx		; racunaj sljedeci fibbonacijev broj (eax je stari, ebx novi)
	add ebx, eax
	mov eax, edx
	
	dec ecx			; gotovi?
	jnz print_num
	
	pop ebx
	ret

format db '%d', 10, 0		; printf specifikator ispisa
