/* Hexdump main function, assembly language version */
	.section .rodata
fmtstr:
	.string ":"
empty:
	.string " "
empty2:
	.string "  "
empty3:
	.string "   "

	.section .text

	.globl main
main:
	pushq %rbp
	pushq %r12					/* use this as n */
	pushq %r13					/* use this as offset */
	pushq %r14					/* use this as i in for loop*/
	pushq %r15					/* */
	movq %rsp, %rbp
	subq $32, %rsp
	movl $0, %r13d
.LStartWhile:
	leaq (%rbp), %rdi
	call hex_read
	movl %eax, %r12d
	cmpl $0, %r12d
	je .LEndWhile
	movl %r13d, %edi
	leaq -18(%rbp), %rsi
	call hex_format_offset
	leaq -18(%rbp), %rdi
	call hex_write_string
	movq $fmtstr, %rdi
	call hex_write_string
	movl $0, %r14d
.LStartForSbuf:
	cmpl %r12d, %r14d
	jae .LEndForSbut
	movb (%rbp, %r14, 1), %dil  /* check movq */
	leaq -27(%rbp), %rsi
	call hex_format_byte_as_hex
	movb (%rbp, %r14, 1), %dil
	call hex_to_printable
	movb %al, (%rbp, %r14, 1)
	movq $empty, %rdi
	call hex_write_string
	leaq -27(%rbp), %rdi
	call hex_write_string
	incl %r14d
	jmp .LStartForSbuf

.LEndForSbut:
	movl %r12d, %r14d
.LStartEmptyForLoop:
	cmpl $16, %r14d
	jae .LEndEmptyForLoop
	movq $empty3, %rdi
	call hex_write_string
	incl %r14d
	jmp .LStartEmptyForLoop

.LEndEmptyForLoop:
	movq $empty2, %rdi
	call hex_write_string
	movb $10, (%rbp, %r12, 1)
	movq %r12, %r10
	incq %r10
	movb $0, (%rbp, %r10, 1)
	leaq (%rbp), %rdi
	call hex_write_string
	addl %r12d, %r13d
	jmp .LStartWhile
	
.LEndWhile:
	addq $32, %rsp
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbp
	movq $0, %rax
	ret


/* vim:ft=gas:
 */
