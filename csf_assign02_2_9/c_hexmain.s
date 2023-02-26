	.file	"c_hexmain.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	":"
.LC1:
	.string	" "
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	subq	$56, %rsp
	.cfi_def_cfa_offset 96
	movl	$0, %r13d
	jmp	.L7
.L4:
	movslq	%ebx, %rbp
	movzbl	16(%rsp,%rbp), %edi
	leaq	4(%rsp), %rsi
	call	hex_format_byte_as_hex
	movzbl	16(%rsp,%rbp), %edi
	call	hex_to_printable
	movb	%al, 16(%rsp,%rbp)
	movl	$.LC1, %edi
	call	hex_write_string
	leaq	4(%rsp), %rdi
	call	hex_write_string
	addl	$1, %ebx
.L3:
	cmpl	%r12d, %ebx
	jb	.L4
	movl	%r12d, %ebx
	jmp	.L5
.L6:
	movl	$.LC1, %edi
	call	hex_write_string
	movl	$.LC1, %edi
	call	hex_write_string
	movl	$.LC1, %edi
	call	hex_write_string
	addl	$1, %ebx
.L5:
	cmpl	$15, %ebx
	jle	.L6
	movl	$.LC1, %edi
	call	hex_write_string
	movl	$.LC1, %edi
	call	hex_write_string
	movl	%r12d, %eax
	movb	$10, 16(%rsp,%rax)
	leal	1(%r12), %eax
	movb	$0, 16(%rsp,%rax)
	leaq	16(%rsp), %rdi
	call	hex_write_string
	addl	%r12d, %r13d
.L7:
	leaq	16(%rsp), %rdi
	call	hex_read
	movl	%eax, %r12d
	testl	%eax, %eax
	je	.L2
	leaq	7(%rsp), %rsi
	movl	%r13d, %edi
	call	hex_format_offset
	leaq	7(%rsp), %rdi
	call	hex_write_string
	movl	$.LC0, %edi
	call	hex_write_string
	movl	$0, %ebx
	jmp	.L3
.L2:
	movl	$0, %eax
	addq	$56, %rsp
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 12.2.1 20221121 (Red Hat 12.2.1-4)"
	.section	.note.GNU-stack,"",@progbits
