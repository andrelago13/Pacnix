#include "kbd.h"

.global _asmHandler


.data
	break: .string "Breakcode : 0x%02X\n"		# String for normal breakcode
	make: .string "Makecode : 0x%02X\n"			# String for normal makecode
	special: .string "Special key => "			# String for special key
	sbreak: .string "Breakcode : 0xE0%02X\n"	# String for 2 byte breakcode
	smake: .string "Makecode : 0xE0%02X\n"		# String for 2 byte makecode

.text

_asmHandler:
	movl 4(%esp), %edx				# Read argument - last code read
	
	in $KBD_OUT_BUF, %eax			# Read scancode from OUT_BUF
	push %eax						# Preserve EAX
	
	mov $TWO_BYTE_SCAN, %cl			# Compare last scan do 0xE0
	cmp %dl, %cl					# If equal, current scan is a
	jne one							# 2 byte scancode
	
	push $special
	call _printf					# Print special key message
	add $4, %esp
	
	pop %eax						# Return EAX to previous value
	push %eax
	
	mov $0, %ecx
	mov $BIT(7), %cl
	test %al, %cl					# Check if current scan is break or make
	jz smk
	mov $0, %ecx
	mov %al, %cl
	push %ecx
	push $sbreak
	call _printf					# Print special breakcode message
	add $8, %esp
	jmp fim
	
smk:
	mov $0, %ecx
	mov %al, %cl
	push %ecx
	push $smake
	call _printf					# Print special makecode message
	add $8, %esp
	jmp fim
	
one:
	mov $TWO_BYTE_SCAN, %cl			# Check if current scancode is 0xE0
	cmp %al, %cl					# If equal, must return to check
	je fim							# 2nd byte of scancode
	
	mov $BIT(7), %cl
	test %al, %cl					# Check if current scan is break or make
	jz mk
	mov $0, %ecx
	mov %al, %cl
	push %ecx
	push $break
	call _printf					# Print 1 byte breakcode message
	add $8, %esp
	jmp fim
	
mk:
	mov $0, %ecx
	mov %al, %cl
	push %ecx
	push $make
	call _printf					# Print 1 byte makecode message
	add $8, %esp
	jmp fim
	
fim:
	pop %ecx						# Return - put in al the scancode read
	mov $0, %eax
	mov %cl, %al
	ret
	.end _asmHandler
	
