.text
.global _start
_start:
	sub $0x100, %rsp
	xor %rax, %rax
	xor %rdi, %rdi
	mov $0x100, %rsi
	mov %rsp, %rdi
