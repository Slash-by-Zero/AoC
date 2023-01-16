.text
.global _start
_start:


exit_rdi:
	mov $0x0, %rdi
	mov $0x3c, %rax
	syscall
