;
; Entry Program for IRVM Code
; Owner: Codeleaded
;

bits 32

global _start
global f.syscall
extern f.main

section .text

f.syscall:
    mov eax, [esp + 0]
    mov ebx, [esp + 4]
    mov ecx, [esp + 8]
    mov edx, [esp + 12]
    mov esi, [esp + 16]
    mov edi, [esp + 20]
    mov ebp, [esp + 24]
	int 0x80
	mov ebx,eax
	ret

_start:
	mov ebp,esp
	call f.main
	mov	eax,1
	;mov	ebx,ebx
	int 0x80
