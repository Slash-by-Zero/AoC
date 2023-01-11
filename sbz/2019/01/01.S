.text
.global _start
_start:
	sub $0x100, %rsp
	mov %rsp, %rsi
	xor %r8, %r8
	xor %r14, %r14
	xor %r15, %r15
	xor %rax, %rax
	xor %rbx, %rbx
	mov $0x0a, %r13
	mov $0x3, %r12
lbody_read:
	mov $0x100, %rdx
	xor %rdi, %rdi
	mov %rax, %rbx
	xor %rax, %rax
	syscall
	cmp $0x0, %rax
	jle done
	xchg %rax, %rbx
lbody_2:
	mov (%rsp, %rdi, 0x1), %r8b
	add $0x1, %rdi
	cmp $0x0a, %r8b
	je else
	sub $0x30, %r8b
	mul %r13
	add %r8, %rax
	cmp %rdi, %rbx
	jg lbody_2
	jmp lbody_read
else:
	xor %rdx, %rdx
	div %r12
	cmp $0x0, %rax
	je done
	sub $0x02, %rax
	add %rax, %r14
lbody_part2:
	add %rax, %r15
	xor %rdx, %rdx
	div %r12
	sub $0x02, %rax
	jg lbody_part2
	xor %rax, %rax
	cmp %rdi, %rbx
	jg lbody_2
	jmp lbody_read
done:
	add $0x100, %rsp
	mov %r15, %rax
	xor %rbx, %rbx
	sub $0x01, %rsp
	movb $0x0a, (%rsp)
parse_res2:
	add $0x1, %rbx
	xor %rdx, %rdx
	div %r13
	add $0x30, %dl
	sub $0x1, %rsp
	mov %dl, (%rsp)
	test %rax, %rax
	jnz parse_res2
	mov $0x203a322074726150, %rdx
	push %rdx
	mov %r14, %rax
	sub $0x01, %rsp
	movb $0x0a, (%rsp)
parse_res1:
	add $0x1, %rbx
	xor %rdx, %rdx
	div %r13
	add $0x30, %dl
	sub $0x1, %rsp
	mov %dl, (%rsp)
	test %rax, %rax
	jnz parse_res1
	mov $0x203a312074726150, %rdx
	push %rdx
	add $0x12, %rbx
	mov %rbx, %rdx
	mov %rsp, %rsi
	mov $0x1, %rdi
	mov $0x1, %rax
	syscall
	mov $0x3c, %rax
	xor %rdi, %rdi
	syscall
