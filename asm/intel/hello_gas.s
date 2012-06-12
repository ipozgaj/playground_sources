.section .data
msg:
	.ascii	"Hello world\n"
len:
	.long	. - msg

.section .text
.globl _start

_start:
	movl	$4, %eax
	xorl	%ebx, %ebx
	incl	%ebx
	leal	msg, %ecx
	movl	len, %edx
	int		$0x80
	movl	$1, %eax
	xorl	%ebx, %ebx
	int		$0x80
