;
; Entry Program for IRVM Code
; Owner: Codeleaded
;

bits 64

global _start
global f.syscall
extern f.exit
extern f.main

section .text

f.syscall:
    push rbp
    mov rbp, rsp

    mov rax, rsi       ; syscall number
    mov rdx, r8        ; a1
    mov r8, r9         ; a2
    mov r9, r10        ; a3
    mov r10, rdi       ; a0

    ; Windows syscall stack:
    ; [rsp + 0x28] = a4 (5th argument)
    ; [rsp + 0x30] = a5 (6th argument)
    ; ...

    sub rsp, 0x60      ; Shadow space + argument space

    mov [rsp + 0x28], r11; a4

    mov r11, [rbp + 56]  ; a5
    mov [rsp + 0x30], r11

    mov r11, [rbp + 48]  ; a6
    mov [rsp + 0x38], r11

    mov r11, [rbp + 40]  ; a7
    mov [rsp + 0x40], r11

    mov r11, [rbp + 32]  ; a8
    mov [rsp + 0x48], r11

    mov r11, [rbp + 24]  ; a9
    mov [rsp + 0x50], r11

    mov r11, [rbp + 16]  ; a10
    mov [rsp + 0x58], r11

    syscall

    mov rsp, rbp
    pop rbp

    mov rbx, rax
    ret

_start:
	mov rbp,rsp
	call f.main
	mov	rsi,rbx
	call f.exit