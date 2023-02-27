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
	subq $8, %rsp
	movq %rdi, %rsi
	movl $0, %edi
	movq $16, %rdx
	call read
	addq $8, %rsp
	ret

hex_write_string:
	pushq %r12 					/* use r12 as a count index to the length of the string*/
	movq $0, %r12
.LStartWhileHexWrite:
	cmpb $0, (%rdi, %r12, 1)
	je .LEndWhileHexWrite
	incq %r12
	jmp .LStartWhileHexWrite

.LEndWhileHexWrite:
	leaq (%rdi), %rsi
	movq $1, %rdi
	movq %r12, %rdx
	call write
	popq %r12
	ret



hex_format_offset:
	pushq %r12
	movq $0, %r10					/* use r10 as index i*/
.LStartForSetToZero:
	cmpq $8, %r10					/* for loop condition */
	jae .LEndForSetToZero			
	movq $48, (%rsi, %r10, 1)		/* put '0' to sbuf */
	incq %r10						/* move index i */
	jmp .LStartForSetToZero
	
.LEndForSetToZero:
	movq $7, %r10					/* use r10 as counter index*/
	movl $15, %r12d					/* use r12d as variable temp */

.LStartWhileOffset:
	movl $15, %r12d					
	cmpl $0, %edi					/* while condition, compare temp_offset (%edi)*/
	je .LEndWhileOffset
	andl %edi, %r12d				
	cmpl $10, %r12d
	jae .LAddLetterOffset
	movl %r12d, %r11d
	addl $48, %r11d
	movb %r11b, (%rsi, %r10, 1)		/* update sbuf */
	decq %r10
	shrl $4, %edi
	jmp .LStartWhileOffset			/* update temp_offset */

.LAddLetterOffset:
	movl %r12d, %r11d
	addl $87, %r11d					
	movb %r11b, (%rsi, %r10, 1) 	/* update sbuf */
	decq %r10
	shrl $4, %edi					/* update temp_offset */
	jmp .LStartWhileOffset

.LEndWhileOffset:
	movq $ 8, %r10					
	movb $0, (%rsi, %r10, 1)		/* add null terminator at the end of sbuf*/
	popq %r12
	ret

hex_format_byte_as_hex:
	pushq %r13 					/* use r13 as a index counter */
	pushq %r12					/* use r12 as a temp value holder */
	pushq %r14					/* stack alignment(optional)*/
	movq $2, %r13
	movb $0, (%rsi, %r13, 1)	/* put null terminator at the end (index 2) of sbuf*/
	decq %r13					
	movb $15, %r12b				/* r12 unsigned char variable second */
	andb %dil, %r12b			
	cmpb $10, %r12b	
	jae .LPutLetterInSecond
	addb $48, %r12b
	movb %r12b, (%rsi, %r13, 1)

.LStartFirst:
	decq %r13					
	movb %dil, %r14b			/* r14 unsigned char variable first */
	shrb $4, %r14b
	cmpb $10, %r14b
	jae .LPutLetterInFirst
	addb $48, %r14b
	movb %r14b, (%rsi, %r13, 1)
	popq %r14
	popq %r12
	popq %r13
	ret

.LPutLetterInSecond:
	addb $87, %r12b
	movb %r12b, (%rsi, %r13, 1)		/* update sbuf */
	jmp .LStartFirst

.LPutLetterInFirst:
	addb $87, %r14b
	movb %r14b, (%rsi, %r13, 1)
	popq %r14
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
