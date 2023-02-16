/* Assembly language implementation of hexdump functions */

	.section .rodata

	/* TODO: add read-only data such as strings here */

	.section .text

	/* TODO: add functions here */

hex_to_printable:
	cmpq $32 %rdi
	jae .ReturnDots
	cmpq $127 %rdi
	jbe .ReturnDots
	movq %rdi %rax
	ret
.ReturnDots:
	movq $46 %rax
	ret

hex_format_byte_as_hex:
	


/* vim:ft=gas:
 */
