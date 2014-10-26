#include "i8254.h"
#include "kbd.h"

.global _asmHandler


.data
	decimal: .string "==== %u =====\n\n"
	hexa: .string "==== 0x%x =====\n\n"
	break: .string "Breakcode : 0x%x\n"
	make: .string "Makecode : 0x%x\n"
	special: .string "Special key => "
	speciallen: .int 16
	sbreak: .string "Breakcode : 0xE0%x\n"
	smake: .string "Makecode : 0xE0%x\n"
	trying: .int 0
	
	

.text

_asmHandler:
	movl 4(%esp), %edx
	
	in $KBD_OUT_BUF, %eax
	push %eax
	
	mov $TWO_BYTE_SCAN, %cl
	cmp %dl, %cl
	jne one
	
	
two:
	push $special
	call _printf
	add $4, %esp
	
	pop %eax
	push %eax
	
	mov $0, %ecx
	mov $BIT(7), %cl
	test %al, %cl
	jz smk
	mov $0, %ecx
	mov %al, %cl
	push %ecx
	push $sbreak
	call _printf
	add $8, %esp
	jmp fim
	
smk:
	mov $0, %ecx
	mov %al, %cl
	push %ecx
	push $smake
	call _printf
	add $8, %esp
	jmp fim
	
	
	
	
one:
	mov $TWO_BYTE_SCAN, %cl
	cmp %al, %cl
	je fim
	
	mov $BIT(7), %cl
	test %al, %cl
	jz mk
	mov $0, %ecx
	mov %al, %cl
	push %ecx
	push $break
	call _printf
	add $8, %esp
	jmp fim
	
mk:
	mov $0, %ecx
	mov %al, %cl
	push %ecx
	push $make
	call _printf
	add $8, %esp
	jmp fim
	

	


fim:
	pop %ecx
	mov $0, %eax
	mov %cl, %al
	ret
	.end _asmHandler
	
