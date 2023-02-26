	.file	"c_hexfuncs.c"
	.text
	.globl	hex_read
	.type	hex_read, @function
hex_read:
.LFB0:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movq	%rdi, %rsi
	movl	$16, %edx
	movl	$0, %edi
	call	read
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE0:
	.size	hex_read, .-hex_read
	.globl	hex_write_string
	.type	hex_write_string, @function
hex_write_string:
.LFB1:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movq	%rdi, %rsi
	movl	$0, %eax
	jmp	.L4
.L5:
	addl	$1, %eax
.L4:
	movslq	%eax, %rdx
	cmpb	$0, (%rsi,%rdx)
	jne	.L5
	movl	$1, %edi
	call	write
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE1:
	.size	hex_write_string, .-hex_write_string
	.globl	hex_format_offset
	.type	hex_format_offset, @function
hex_format_offset:
.LFB2:
	.cfi_startproc
	movl	$0, %eax
	jmp	.L8
.L9:
	movslq	%eax, %rdx
	movb	$48, (%rsi,%rdx)
	addl	$1, %eax
.L8:
	cmpl	$7, %eax
	jle	.L9
	movl	$7, %edx
	jmp	.L10
.L11:
	addl	$87, %eax
.L12:
	movslq	%edx, %rcx
	movb	%al, (%rsi,%rcx)
	shrl	$4, %edi
	leal	-1(%rdx), %edx
.L10:
	testl	%edi, %edi
	je	.L14
	movl	%edi, %eax
	andl	$15, %eax
	cmpl	$9, %eax
	ja	.L11
	addl	$48, %eax
	jmp	.L12
.L14:
	movb	$0, 8(%rsi)
	ret
	.cfi_endproc
.LFE2:
	.size	hex_format_offset, .-hex_format_offset
	.globl	hex_format_byte_as_hex
	.type	hex_format_byte_as_hex, @function
hex_format_byte_as_hex:
.LFB3:
	.cfi_startproc
	movl	%edi, %eax
	andl	$15, %eax
	shrb	$4, %dil
	cmpb	$9, %al
	ja	.L16
	addl	$48, %eax
	movb	%al, 1(%rsi)
.L17:
	cmpb	$9, %dil
	ja	.L18
	addl	$48, %edi
	movb	%dil, (%rsi)
.L19:
	movb	$0, 2(%rsi)
	ret
.L16:
	addl	$87, %eax
	movb	%al, 1(%rsi)
	jmp	.L17
.L18:
	addl	$87, %edi
	movb	%dil, (%rsi)
	jmp	.L19
	.cfi_endproc
.LFE3:
	.size	hex_format_byte_as_hex, .-hex_format_byte_as_hex
	.globl	hex_to_printable
	.type	hex_to_printable, @function
hex_to_printable:
.LFB4:
	.cfi_startproc
	leal	-32(%rdi), %eax
	cmpb	$94, %al
	jbe	.L23
	movl	$46, %eax
	ret
.L23:
	movl	%edi, %eax
	ret
	.cfi_endproc
.LFE4:
	.size	hex_to_printable, .-hex_to_printable
	.ident	"GCC: (GNU) 12.2.1 20221121 (Red Hat 12.2.1-4)"
	.section	.note.GNU-stack,"",@progbits
