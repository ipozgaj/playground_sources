; trazi i preko $? varijable vraca najveci podatak u bloku podataka
; (C)  2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

.section .data
numbers:
	.long	32, 51, 21, 8, 42, 76, 34, 12, 0
	
.section .text
.globl _start

_start:
	xorl	%esi, %esi
	movl	numbers(,%esi,4), %eax
	movl	%eax, %ebx
next:	cmpl	$0, %eax
	je	quit
	incl	%esi
	movl	numbers(,%esi,4), %eax
	cmpl	%ebx, %eax
	jle	next
	movl	%eax, %ebx
	jmp	next
quit:	movl	$1, %eax
	int	$0x80
