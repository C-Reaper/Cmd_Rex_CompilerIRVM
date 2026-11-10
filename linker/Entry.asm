;
; Entry Program for IRVM Code
; Owner: Codeleaded
;

bits 64

global _start
global f.syscall
extern f.main

section .text

f.syscall:
	mov rax,rsi
	;mov	rdi,rdi
	mov rsi,r8
	mov rdx,r9
	;mov	r10,r10
	mov r8,r11
	;mov r9,stack
	syscall
	ret

_start:
	mov rbp,rsp
	mov	rsi,1
	mov	rdi,2
	call f.main
	;mov rax,69
	mov	rdi,rbx
	mov	rax,60
	syscall
