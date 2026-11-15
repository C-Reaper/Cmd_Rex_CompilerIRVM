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

; Windows x64 function ABI adapter
; IRVM: RSI, RDI, R8, R9, R10, R11, stack
; Windows: RCX, RDX, R8, R9, stack

%macro WIN_WRAPPER 13
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
%if %3 >= 5
    mov [rsp + 0x20], r10
%endif
%if %3 >= 6
    mov [rsp + 0x28], r11
%endif

    ; IRVM packs stack arguments in reverse order using their declared sizes.
%if %3 >= 7
%if %9 == 1
    mov eax, DWORD[rbp + %4]
    mov [rsp + 0x30], rax
%else
    mov rax, [rbp + %4]
    mov [rsp + 0x30], rax
%endif
%endif
%if %3 >= 8
%if %10 == 1
    mov eax, DWORD[rbp + %5]
    mov [rsp + 0x38], rax
%else
    mov rax, [rbp + %5]
    mov [rsp + 0x38], rax
%endif
%endif
%if %3 >= 9
%if %11 == 1
    mov eax, DWORD[rbp + %6]
    mov [rsp + 0x40], rax
%else
    mov rax, [rbp + %6]
    mov [rsp + 0x40], rax
%endif
%endif
%if %3 >= 10
%if %12 == 1
    mov eax, DWORD[rbp + %7]
    mov [rsp + 0x48], rax
%else
    mov rax, [rbp + %7]
    mov [rsp + 0x48], rax
%endif
%endif
%if %3 >= 11
%if %13 == 1
    mov eax, DWORD[rbp + %8]
    mov [rsp + 0x50], rax
%else
    mov rax, [rbp + %8]
    mov [rsp + 0x50], rax
%endif
%endif

    call %2

    ; Extend the 32-bit API result to the internal 64-bit return register.
    movsxd rbx, eax

    mov rsp, rbp
    pop rbp
    ret
%endmacro

section .text

WIN_WRAPPER f.NtReadFile, NtReadFile, 9, 32, 24, 16, 0, 0, 1, 0, 0, 0, 0
WIN_WRAPPER f.NtWriteFile, NtWriteFile, 9, 32, 24, 16, 0, 0, 1, 0, 0, 0, 0
WIN_WRAPPER f.NtCreateFile, NtCreateFile, 11, 36, 32, 28, 20, 16, 1, 1, 1, 0, 1
WIN_WRAPPER f.NtClose, NtClose, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
WIN_WRAPPER f.NtDeviceIoControlFile, NtDeviceIoControlFile, 10, 32, 28, 20, 16, 0, 0, 1, 0, 1, 0
WIN_WRAPPER f.NtAllocateVirtualMemory, NtAllocateVirtualMemory, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
WIN_WRAPPER f.NtProtectVirtualMemory, NtProtectVirtualMemory, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
WIN_WRAPPER f.NtFreeVirtualMemory, NtFreeVirtualMemory, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
WIN_WRAPPER f.NtQueryInformationProcess, NtQueryInformationProcess, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
WIN_WRAPPER f.NtQueryInformationFile, NtQueryInformationFile, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
WIN_WRAPPER f.NtSetInformationFile, NtSetInformationFile, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
WIN_WRAPPER f.NtTerminateProcess, NtTerminateProcess, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
WIN_WRAPPER f.NtWaitForSingleObject, NtWaitForSingleObject, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
WIN_WRAPPER f.NtDelayExecution, NtDelayExecution, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
WIN_WRAPPER f.NtQueryPerformanceCounter, NtQueryPerformanceCounter, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
WIN_WRAPPER f.RtlUTF8ToUnicodeN, RtlUTF8ToUnicodeN, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
WIN_WRAPPER f.RtlDosPathNameToNtPathName_U, RtlDosPathNameToNtPathName_U, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
WIN_WRAPPER f.RtlFreeUnicodeString, RtlFreeUnicodeString, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

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