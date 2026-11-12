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
	mov	rcx,rdi
	mov rdx,r8
	mov r8,r9
	mov	r9,r10
	;mov stack,r11 ...
	syscall
	mov rbx,rax
	ret

_start:
	mov rbp,rsp
	call f.main
	;mov rax,69
	mov	rax,60
	mov	rcx,rbx
	syscall
