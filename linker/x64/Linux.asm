;
; Entry Program for IRVM Code
; Owner: Codeleaded
;

bits 64

global _start
global f.syscall

global f.stdout
global f.stderr
global f.stdin

extern f.sys.exit
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

_start:
	mov rbp,rsp
	call f.main
	mov	rsi,rbx
	call f.sys.exit
