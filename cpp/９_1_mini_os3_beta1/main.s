	.file	"main.c"
	.text
	.globl	os_delay_ms
	.type	os_delay_ms, @function
os_delay_ms:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	movl	-20(%rbp), %eax
	imull	$1000, %eax, %eax
	movl	%eax, -4(%rbp)
	call	clock@PLT
	movq	%rax, -16(%rbp)
	nop
.L2:
	call	clock@PLT
	subq	-16(%rbp), %rax
	movq	%rax, %rdx
	movl	-4(%rbp), %eax
	cltq
	cmpq	%rax, %rdx
	jl	.L2
	movl	$1, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	os_delay_ms, .-os_delay_ms
	.comm	stack1,8,8
	.comm	stack2,8,8
	.comm	ctx1,8,8
	.comm	ctx2,8,8
	.comm	env,200,32
	.globl	next
	.bss
	.align 8
	.type	next, @object
	.size	next, 8
next:
	.zero	8
	.globl	count
	.align 4
	.type	count, @object
	.size	count, 4
count:
	.zero	4
	.section	.rodata
.LC0:
	.string	"flag2 %d,%d,%d\n"
	.text
	.globl	schedule
	.type	schedule, @function
schedule:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	$0, -8(%rbp)
	movl	count(%rip), %eax
	addl	$1, %eax
	movl	%eax, count(%rip)
	movl	count(%rip), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	count(%rip), %eax
	andl	$1, %eax
	testl	%eax, %eax
	jne	.L5
	movq	ctx2(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	save@PLT
	movq	ctx1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	restore@PLT
	jmp	.L7
.L5:
	movq	ctx1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	save@PLT
	movq	ctx2(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	restore@PLT
.L7:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	schedule, .-schedule
	.section	.rodata
.LC1:
	.string	"thread 1 :%d\n"
.LC2:
	.string	"xixi"
.LC3:
	.string	"qiqi"
	.text
	.globl	func1
	.type	func1, @function
func1:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$1, -4(%rbp)
	jmp	.L9
.L10:
	movl	-4(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$1, %edi
	call	sleep@PLT
	leaq	.LC2(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$1, %edi
	call	sleep@PLT
	leaq	.LC3(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
.L9:
	movl	-4(%rbp), %eax
	leal	1(%rax), %edx
	movl	%edx, -4(%rbp)
	testl	%eax, %eax
	jne	.L10
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	func1, .-func1
	.section	.rodata
.LC4:
	.string	"thread 2 :%d\n"
	.text
	.globl	func2
	.type	func2, @function
func2:
.LFB9:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$65535, -4(%rbp)
	jmp	.L12
.L13:
	movl	-4(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC4(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$1, %edi
	call	sleep@PLT
.L12:
	movl	-4(%rbp), %eax
	leal	-1(%rax), %edx
	movl	%edx, -4(%rbp)
	testl	%eax, %eax
	jne	.L13
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	func2, .-func2
	.globl	init_sigaction
	.type	init_sigaction, @function
init_sigaction:
.LFB10:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$160, %rsp
	leaq	schedule(%rip), %rax
	movq	%rax, -160(%rbp)
	movl	$1073741824, -24(%rbp)
	leaq	-160(%rbp), %rax
	addq	$8, %rax
	movq	%rax, %rdi
	call	sigemptyset@PLT
	leaq	-160(%rbp), %rax
	movl	$0, %edx
	movq	%rax, %rsi
	movl	$14, %edi
	call	sigaction@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	init_sigaction, .-init_sigaction
	.globl	init_time
	.type	init_time, @function
init_time:
.LFB11:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	$2, -16(%rbp)
	movq	$0, -8(%rbp)
	movq	-16(%rbp), %rax
	movq	-8(%rbp), %rdx
	movq	%rax, -32(%rbp)
	movq	%rdx, -24(%rbp)
	leaq	-32(%rbp), %rax
	movl	$0, %edx
	movq	%rax, %rsi
	movl	$0, %edi
	call	setitimer@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	init_time, .-init_time
	.globl	main
	.type	main, @function
main:
.LFB12:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	movq	%rsi, -32(%rbp)
	movl	$56, %edi
	call	malloc@PLT
	movq	%rax, ctx1(%rip)
	movl	$56, %edi
	call	malloc@PLT
	movq	%rax, ctx2(%rip)
	movl	$4096, %edi
	call	malloc@PLT
	movq	%rax, stack1(%rip)
	movl	$4096, %edi
	call	malloc@PLT
	movq	%rax, stack2(%rip)
	movq	ctx1(%rip), %rax
	movl	$56, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	memset@PLT
	movq	ctx2(%rip), %rax
	movl	$56, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	memset@PLT
	movq	ctx1(%rip), %rax
	leaq	func1(%rip), %rdx
	movq	%rdx, 16(%rax)
	movq	stack1(%rip), %rax
	leaq	4000(%rax), %rdx
	movq	ctx1(%rip), %rax
	movq	%rdx, 8(%rax)
	movq	ctx1(%rip), %rdx
	movq	8(%rax), %rax
	movq	%rax, (%rdx)
	movq	ctx2(%rip), %rax
	leaq	func2(%rip), %rdx
	movq	%rdx, 16(%rax)
	movq	stack2(%rip), %rax
	leaq	4000(%rax), %rdx
	movq	ctx2(%rip), %rax
	movq	%rdx, 8(%rax)
	movq	ctx2(%rip), %rdx
	movq	8(%rax), %rax
	movq	%rax, (%rdx)
	call	init_sigaction
	movl	$0, %eax
	call	init_time
	movq	ctx1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	save@PLT
	movl	%eax, -4(%rbp)
	movq	ctx2(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	save@PLT
	movl	%eax, -8(%rbp)
	movl	$0, %eax
	call	schedule
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	main, .-main
	.ident	"GCC: (Debian 8.3.0-6) 8.3.0"
	.section	.note.GNU-stack,"",@progbits
