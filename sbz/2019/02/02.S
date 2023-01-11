.text
.global _start
_start:
	sub $0x100, %rsp
	mov %rsp, %rsi
	xor %r9, %r9
	xor %r8, %r8
	xor %r14, %r14
	mov $0xc, %r12
	mov $0x2, %r13
	mov $0xa, %r10

loop_read:
	mov $0x100, %rdx
	xor %rdi, %rdi
	mov %rax, %rbx
	xor %rax, %rax
	syscall
	test %rax, %rax
	jz execute
	xchg %rbx, %rax
loop_forchar:
	mov (%rsi, %rdi, 1), %r8b
	add $0x1, %rdi
	cmp $0x0a, %r8b
	je on_seperator
	cmp $0x2c, %r8b
	je on_seperator
	mul %r10
	sub $0x30, %r8b
	add %r8, %rax
	cmp %rdi, %rbx
	jg loop_forchar
	jmp loop_read
on_seperator:
	push %rax
	push %rax
	xor %rax, %rax
	cmp %rdi, %rbx
	jg loop_forchar
	jmp loop_read

execute:
	mov $-2, %rcx
	mov %r12, -0x20(%rsi)
	mov %r13, -0x30(%rsi)
loop_execute:
	mov (%rsi, %rcx, 8), %rbx
	cmp $99, %rbx
	je done
	mov -0x10(%rsi, %rcx, 8), %rax 
	not %rax
	shl $1, %rax
	mov (%rsi, %rax, 8), %rax
	mov -0x20(%rsi, %rcx, 8), %r8
	not %r8
	shl $1, %r8
	mov (%rsi, %r8, 8), %r8
	mov -0x30(%rsi, %rcx, 8), %r9
	not %r9
	shl $1, %r9
	sub $0x8, %rcx
	test $0x2, %rbx
	jnz mul
add:
	add %r8, %rax
	mov %rax, (%rsi, %r9, 8)
	jmp loop_execute
mul:
	mul %r8
	mov %rax, (%rsi, %r9, 8)
	jmp loop_execute

done:
	mov -0x10(%rsi), %rax
	test %r14, %r14
	jnz skip_set_res1
	mov %rax, %r14
	mov $-1, %r12
	xor %r13, %r13
skip_set_res1:
	cmp $19690720, %rax
	je print_results
	mov %rsp, %rbx
loop_reset:
	mov 8(%rbx), %rax
	mov %rax, (%rbx)
	add $0x10, %rbx
	cmp %rsi, %rbx
	jne loop_reset
	add $0x1, %r12
	cmp $100, %r12
	jne execute
	xor %r12, %r12
	add $0x1, %r13
	jmp execute
	
print_results:
	mov %r12, %rax
	mul %r10
	mul %r10
	add %r13, %rax
	mov $0x12, %rbx
	sub $0x1, %rsp
	movb $0x0a, (%rsp)
loop_parse_res2:
	xor %rdx, %rdx
	div %r10 
	add $0x30, %rdx
	sub $0x1, %rsp
	mov %dl, (%rsp)
	add $0x1, %rbx
	test %rax, %rax
	jnz loop_parse_res2
	mov $0x203a322074726150, %rdx
	push %rdx
	mov %r14, %rax
	sub $0x1, %rsp
	movb $0x0a, (%rsp)
loop_parse_res1:
	xor %rdx, %rdx
	div %r10 
	add $0x30, %rdx
	sub $0x1, %rsp
	mov %dl, (%rsp)
	add $0x1, %rbx
	test %rax, %rax
	jnz loop_parse_res1
	mov $0x203a322074726150, %rdx
	push %rdx
	mov %rsp, %rsi
	mov $0x1, %rdi
	mov %rbx, %rdx
	mov $0x1, %rax
	syscall
	
// Exit normally
	mov $0x3c, %rax
	xor %rdi, %rdi
	syscall

