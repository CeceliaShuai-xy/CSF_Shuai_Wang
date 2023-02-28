/* Assembly language implementation of hexdump functions */
/* Cecelia Shuai(xshuai3), Gigi Wang(ywang580) */
	.section .rodata

	/* TODO: add read-only data such as strings here */

	.section .text

	/* TODO: add functions here */

.global hex_read
.global hex_write_string
.global hex_format_offset
.global hex_format_byte_as_hex
.global hex_to_printable


hex_read:
	subq $8, %rsp				/* stack pointer alignment*/
	movq %rdi, %rsi				/* rdi is the data_buf, will be the second arg in sys call read */
	movl $0, %edi				/* first input is 0 as STDIN_FILENO*/
	movq $16, %rdx				/* the third arg in read() is size 16 */
	call read					
	addq $8, %rsp				/* stack alignment */
	ret

hex_write_string:
	pushq %r12 					/* use callee-saved r12 as a counter index to the length of the string*/
	movq $0, %r12				/* initialize counter */
.LStartWhileHexWrite:
	cmpb $0, (%rdi, %r12, 1)	/* decide whether continue while loop*/
	je .LEndWhileHexWrite		/* end while loop*/
	incq %r12					/* add counter index*/
	jmp .LStartWhileHexWrite	

.LEndWhileHexWrite:
	leaq (%rdi), %rsi			/* put the first arg s[] as the second var in sys call write*/
	movq $1, %rdi				/* first arg in write() is 1 */
	movq %r12, %rdx				/* third arg in write() is the counter */
	call write
	popq %r12					/* pop the callee-saved register */
	ret



hex_format_offset:
	pushq %r12						
	movq $0, %r10					/* use r10 as index i*/
.LStartForSetToZero:
	cmpq $8, %r10					/* for loop condition */
	jae .LEndForSetToZero			
	movb $48, (%rsi, %r10, 1)		/* put '0' to sbuf */
	incq %r10						/* move index i */
	jmp .LStartForSetToZero
	
.LEndForSetToZero:
	movq $7, %r10					/* use r10 as counter index*/
	movl $15, %r12d					/* use r12d as variable temp */

.LStartWhileOffset:
	movl $15, %r12d					
	cmpl $0, %edi					/* while condition, compare temp_offset (%edi) with 0 */
	je .LEndWhileOffset				/* end while loop */
	andl %edi, %r12d				/* update temp by mod 16 */
	cmpl $10, %r12d					/* conditions to update temp */
	jae .LAddLetterOffset
	movl %r12d, %r11d				/* temporal register to hold value of temp*/
	addl $48, %r11d					/* update temp */
	movb %r11b, (%rsi, %r10, 1)		/* update sbuf */
	decq %r10						/* update index i*/
	shrl $4, %edi					/* update temp by dividing 16 */
	jmp .LStartWhileOffset			/* update temp_offset */

.LAddLetterOffset:		
	movl %r12d, %r11d				/* temporal register to hold value of temp*/
	addl $87, %r11d					/* update temp */
	movb %r11b, (%rsi, %r10, 1) 	/* update sbuf */
	decq %r10						/* update index i*/
	shrl $4, %edi					/* update temp_offset */
	jmp .LStartWhileOffset

.LEndWhileOffset:
	movq $ 8, %r10					
	movb $0, (%rsi, %r10, 1)		/* add null terminator at the end of sbuf*/
	popq %r12						/* pop callee-saved register*/
	ret

hex_format_byte_as_hex:
	pushq %r13 					/* use r13 as a index counter */
	pushq %r12					/* use r12 as a temp value holder */
	pushq %r14					/* stack alignment(optional) */
	movq $2, %r13
	movb $0, (%rsi, %r13, 1)	/* put null terminator at the end (index 2) of sbuf*/
	decq %r13					
	movb $15, %r12b				/* r12 unsigned char variable: second */
	andb %dil, %r12b			/* byteval mod 16*/
	cmpb $10, %r12b				/* update second based on the conditions */
	jae .LPutLetterInSecond
	addb $48, %r12b
	movb %r12b, (%rsi, %r13, 1)	/* update sbuf */

.LStartFirst:
	decq %r13					
	movb %dil, %r14b			/* r14 unsigned char variable first */
	shrb $4, %r14b				/* byteval divide 16*/
	cmpb $10, %r14b				/* update first based on the conditions */
	jae .LPutLetterInFirst
	addb $48, %r14b
	movb %r14b, (%rsi, %r13, 1)	/* update sbuf */
	popq %r14					/* pop all callee-saved registers*/
	popq %r12
	popq %r13
	ret

.LPutLetterInSecond:
	addb $87, %r12b
	movb %r12b, (%rsi, %r13, 1)		/* update sbuf */
	jmp .LStartFirst

.LPutLetterInFirst:
	addb $87, %r14b
	movb %r14b, (%rsi, %r13, 1)	/* update sbuf */
	popq %r14					/* pop all callee-saved registers*/
	popq %r12
	popq %r13
	ret




hex_to_printable:
	cmpb $32, %dil			/* check if valid */
	jb .LReturnDots
	cmpb $127, %dil			/* check if valid */
	jae .LReturnDots
	movb %dil, %al			/* if valid, return %dil (byteval) itself */
	ret
.LReturnDots:
	movb $46, %al			/* if invalid, return dot */
	ret



/* vim:ft=gas:
 */
