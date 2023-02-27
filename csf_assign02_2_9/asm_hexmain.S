/* Hexdump main function, assembly language version */
/* Cecelia Shuai(xshuai3), Gigi Wang(ywang580) */
	.section .rodata
fmtstr:
	.string ":" 				/* const str variables for formating purposes */
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
	pushq %r14					/* use this as i,j in for loop*/
	pushq %r15					/* for stack alignment*/
	movq %rsp, %rbp
	subq $32, %rsp
	movl $0, %r13d				/* initialize offset as 0 */
.LStartWhile:
	leaq (%rbp), %rdi			/* load the addr of buff */
	call hex_read				
	movl %eax, %r12d			/* save n val in %r12 */
	cmpl $0, %r12d
	je .LEndWhile				/* end while when nothing to read from */
	movl %r13d, %edi			
	leaq -18(%rbp), %rsi		/* load the addr of hex_offset*/
	call hex_format_offset
	leaq -18(%rbp), %rdi		
	call hex_write_string
	movq $fmtstr, %rdi			/* write format str*/
	call hex_write_string
	movl $0, %r14d				/* i for for loop */
.LStartForSbuf:
	cmpl %r12d, %r14d
	jae .LEndForSbut
	movb (%rbp, %r14, 1), %dil  /* buff[i] */
	leaq -27(%rbp), %rsi		/* load addr of sbuf */
	call hex_format_byte_as_hex
	movb (%rbp, %r14, 1), %dil	
	call hex_to_printable	
	movb %al, (%rbp, %r14, 1) 	/* update buff[i] to printable char */
	movq $empty, %rdi
	call hex_write_string		/* print format string */
	leaq -27(%rbp), %rdi		/* load sbuf */
	call hex_write_string
	incl %r14d
	jmp .LStartForSbuf

.LEndForSbut:
	movl %r12d, %r14d			/* now use %r14 as j index counter, starting from %12d (n)*/
.LStartEmptyForLoop:
	cmpl $16, %r14d				
	jae .LEndEmptyForLoop
	movq $empty3, %rdi			/* format: padding 0 for input length < 16 */
	call hex_write_string
	incl %r14d
	jmp .LStartEmptyForLoop

.LEndEmptyForLoop:
	movq $empty2, %rdi			/* print 2 empty spaces before write buff*/
	call hex_write_string
	movb $10, (%rbp, %r12, 1)	/* put '\n' at buff[n] */
	movq %r12, %r10				/* use r10 as a copy of n */
	incq %r10					/* r10 = n+1 */
	movb $0, (%rbp, %r10, 1)	/* put null terminator in the end of buff*/
	leaq (%rbp), %rdi
	call hex_write_string
	addl %r12d, %r13d			/* update offset */
	jmp .LStartWhile
	
.LEndWhile:
	addq $32, %rsp				/* stack alignment */
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbp
	movq $0, %rax
	ret


/* vim:ft=gas:
 */
