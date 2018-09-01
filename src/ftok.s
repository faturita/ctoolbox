	.file	"ftok.c"
	.version	"01.01"
gcc2_compiled.:
.text
	.align 4
.globl err_quit
	.type	 err_quit,@function
err_quit:
	pushl %ebp
	movl %esp,%ebp
	subl $4,%esp
	leal 12(%ebp),%eax
	movl %eax,-4(%ebp)
	movl -4(%ebp),%eax
	pushl %eax
	movl 8(%ebp),%eax
	pushl %eax
	pushl $3
	pushl $0
	call err_doit
	addl $16,%esp
	pushl $1
	call exit
	addl $4,%esp
	.p2align 4,,7
.L5:
	leave
	ret
.Lfe1:
	.size	 err_quit,.Lfe1-err_quit
	.align 4
	.type	 err_doit,@function
err_doit:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	call __errno_location
	movl %eax,%eax
	movl %eax,%edx
	movl (%edx),%eax
	movl %eax,-4(%ebp)
	pushl $0
	call exit
	addl $4,%esp
	.p2align 4,,7
.L6:
	leave
	ret
.Lfe2:
	.size	 err_doit,.Lfe2-err_doit
.section	.rodata
.LC0:
	.string	"Usage: ftok <pathname>"
	.align 32
.LC1:
	.string	"st_dev: %lx, st_ino: %lx, key: %x\n"
.text
	.align 4
.globl main
	.type	 main,@function
main:
	pushl %ebp
	movl %esp,%ebp
	subl $88,%esp
	cmpl $2,8(%ebp)
	je .L8
	pushl $.LC0
	call err_quit
	addl $4,%esp
.L8:
	leal -88(%ebp),%eax
	pushl %eax
	movl 12(%ebp),%eax
	addl $4,%eax
	movl (%eax),%edx
	pushl %edx
	call stat
	addl $8,%esp
	pushl $87
	movl 12(%ebp),%eax
	addl $4,%eax
	movl (%eax),%edx
	pushl %edx
	call ftok
	addl $8,%esp
	movl %eax,%eax
	pushl %eax
	movl -76(%ebp),%eax
	pushl %eax
	movl -88(%ebp),%eax
	pushl %eax
	pushl $.LC1
	call printf
	addl $16,%esp
	pushl $0
	call exit
	addl $4,%esp
	.p2align 4,,7
.L7:
	leave
	ret
.Lfe3:
	.size	 main,.Lfe3-main
	.comm	daemon_proc,4,4
	.ident	"GCC: (GNU) egcs-2.91.66 19990314/Linux (egcs-1.1.2 release)"
