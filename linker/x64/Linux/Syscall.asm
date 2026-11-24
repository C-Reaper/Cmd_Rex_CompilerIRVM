;
; Entry Program for IRVM Code
; Owner: Codeleaded
;

bits 64

global f.syscall
global main

global f.stdout
global f.stderr
global f.stdin

extern f.main

section .text

f.stdout:
    mov rbx, 1
    ret

f.stderr:
    mov rbx, 2
    ret

f.stdin:
    mov rbx, 0
    ret

f.syscall:
	mov rax,rsi
	;mov	rdi,rdi
	mov rsi,r8
	mov rdx,r9
	;mov	r10,r10
	mov r8,r11
	xor r9,r9
	syscall
	mov rbx,rax
	ret

main:
	push rbp
	mov rbp,rsp
	sub rsp,16
	mov [rbp - 8],rbx
	call f.main
	mov eax,ebx
	mov rbx,[rbp - 8]
	mov rsp,rbp
	pop rbp
	ret

section .note.GNU-stack noalloc noexec nowrite progbits
