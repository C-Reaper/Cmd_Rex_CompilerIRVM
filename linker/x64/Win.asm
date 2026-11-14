;
; Entry Program for IRVM Code
; Owner: Codeleaded
;

bits 64

extern f.main
global _start

global f.stdout
global f.stderr
global f.stdin

; Windows x64 NT function ABI adapter
; IRVM: RDI, R8, R9, R10, R11, stack
; Windows: RCX, RDX, R8, R9, stack

%macro NT_WRAPPER 2
global %1
extern %2

%1:
    push rbp
    mov rbp, rsp
    sub rsp, 0x60

    ; First four arguments
    mov rcx, rsi
    mov rdx, rdi
    ; mov r8, r8 | R8  = a2 (already correct)
    ; mov r9, r9 | R9  = a3 (already correct)

    ; Fifth and sixth arguments
    mov [rsp + 0x20], r10
    mov [rsp + 0x28], r11

    ; Remaining arguments
    mov rax, [rbp + 56]
    mov [rsp + 0x30], rax

    mov rax, [rbp + 48]
    mov [rsp + 0x38], rax

    mov rax, [rbp + 40]
    mov [rsp + 0x40], rax

    mov rax, [rbp + 32]
    mov [rsp + 0x48], rax

    mov rax, [rbp + 24]
    mov [rsp + 0x50], rax

    mov rax, [rbp + 16]
    mov [rsp + 0x58], rax

    call %2

    ; NTSTATUS -> signed 64-bit result
    movsxd rbx, eax

    mov rsp, rbp
    pop rbp
    ret
%endmacro

section .text

NT_WRAPPER f.NtReadFile, NtReadFile
NT_WRAPPER f.NtWriteFile, NtWriteFile
NT_WRAPPER f.NtCreateFile, NtCreateFile
NT_WRAPPER f.NtClose, NtClose
NT_WRAPPER f.NtDeviceIoControlFile, NtDeviceIoControlFile
NT_WRAPPER f.NtAllocateVirtualMemory, NtAllocateVirtualMemory
NT_WRAPPER f.NtProtectVirtualMemory, NtProtectVirtualMemory
NT_WRAPPER f.NtFreeVirtualMemory, NtFreeVirtualMemory
NT_WRAPPER f.NtQueryInformationProcess, NtQueryInformationProcess
NT_WRAPPER f.NtTerminateProcess, NtTerminateProcess
NT_WRAPPER f.NtWaitForSingleObject, NtWaitForSingleObject
NT_WRAPPER f.NtDelayExecution, NtDelayExecution
NT_WRAPPER f.NtQueryPerformanceCounter, NtQueryPerformanceCounter

f.stdout:
    mov rax, gs:[0x60]       ; PEB
    mov rax, [rax + 0x20]    ; ProcessParameters
    mov rbx, [rax + 0x28]    ; StandardOutput
    ret

f.stderr:
    mov rax, gs:[0x60]
    mov rax, [rax + 0x20]
    mov rbx, [rax + 0x30]
    ret

f.stdin:
    mov rax, gs:[0x60]
    mov rax, [rax + 0x20]
    mov rbx, [rax + 0x20]
    ret

_start:
	mov rbp,rsp
	call f.main
	mov	rsi,-1
	mov	rdi,rbx
	call f.NtTerminateProcess
    ud2