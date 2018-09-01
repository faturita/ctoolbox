	.file	"easy.c"
	.version	"01.01"
gcc2_compiled.:
.section	.rodata
.LC0:
	.string	"%d"
.text
	.align 4
.globl main
	.type	 main,@function
main:
	pushl %ebp
	movl %esp,%ebp
	subl $4,%esp
	movl $2,-4(%ebp)
	addl $-128,-4(%ebp)
	movl -4(%ebp),%eax
	pushl %eax
	pushl $.LC0
	call printf
	addl $8,%esp
	movl $254,%eax
	jmp .L1
	.p2align 4,,7
.L1:
	leave
	ret
.Lfe1:
	.size	 main,.Lfe1-main
	.ident	"GCC: (GNU) egcs-2.91.66 19990314/Linux (egcs-1.1.2 release)"
