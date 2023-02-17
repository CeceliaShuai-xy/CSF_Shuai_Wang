/* Assembly language implementation of hexdump functions */

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
	ret

hex_write_string:
	ret

hex_format_offset:
	ret

hex_format_byte_as_hex:
	movq $2, %r13
	movb $0, (%rsi, %r13, 1)
	decq %r13
	movb $15, %r12b
	andb %dil, %r12b
	cmpb $10, %r12b
	jae .LPutLetterInSecond
	addb $48, %r12b
	movb %r12b, (%rsi, %r13, 1)

.LStartFirst:
	decq %r13
	movb %dil, %r14b
	shrb $4, %r14b
	cmpb $10, %r14b
	jae .LPutLetterInFirst
	addb $48, %r14b
	movb %r14b, (%rsi, %r13, 1)
	ret

.LPutLetterInSecond:
	addb $87, %r12b
	movb %r12b, (%rsi, %r13, 1)
	jmp .LStartFirst

.LPutLetterInFirst:
	addb $87, %r14b
	movb %r14b, (%rsi, %r13, 1)
	ret




hex_to_printable:
	cmpb $32, %dil
	jb .LReturnDots
	cmpb $127, %dil
	jae .LReturnDots
	movb %dil, %al
	ret
.LReturnDots:
	movb $46, %al
	ret



/* vim:ft=gas:
 */
