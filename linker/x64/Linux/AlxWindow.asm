bits 64

%macro SYSV_WRAPPER 2
global %1
extern %2

%1:
    push rbp
    mov rbp, rsp
    and rsp, -16
    xchg rdi, rsi
    mov rdx, r8
    mov rcx, r9
    mov r8, r10
    mov r9, r11
    call %2
    mov rbx, rax
    mov rsp, rbp
    pop rbp
    ret
%endmacro

section .text

SYSV_WRAPPER f.AlxWindow_Render, AlxWindow_Render
SYSV_WRAPPER f.AlxWindow_Update, AlxWindow_UpdateRex
SYSV_WRAPPER f.AlxWindow_IsRunning, AlxWindow_IsRunningRex
SYSV_WRAPPER f.AlxWindow_PreInit, AlxWindow_PreInit
SYSV_WRAPPER f.AlxWindow_Init, AlxWindow_InitRex
SYSV_WRAPPER f.AlxWindow_Free, AlxWindow_FreeRex
SYSV_WRAPPER f.AlxWindow_GetBuffer, AlxWindow_GetBufferRex
SYSV_WRAPPER f.AlxWindow_GetWidth, AlxWindow_GetWidthRex
SYSV_WRAPPER f.AlxWindow_GetHeight, AlxWindow_GetHeightRex
SYSV_WRAPPER f.AlxWindow_GetElapsedNs, AlxWindow_GetElapsedNsRex
SYSV_WRAPPER f.AlxWindow_GetMouseX, AlxWindow_GetMouseXRex
SYSV_WRAPPER f.AlxWindow_GetMouseY, AlxWindow_GetMouseYRex
SYSV_WRAPPER f.AlxWindow_GetMouseBeforeX, AlxWindow_GetMouseBeforeXRex
SYSV_WRAPPER f.AlxWindow_GetMouseBeforeY, AlxWindow_GetMouseBeforeYRex
SYSV_WRAPPER f.AlxWindow_GetStrokeCount, AlxWindow_GetStrokeCountRex
SYSV_WRAPPER f.AlxWindow_GetStrokeState, AlxWindow_GetStrokeStateRex
SYSV_WRAPPER f.AlxWindow_SetName, AlxWindow_SetNameRex
SYSV_WRAPPER f.AlxWindow_SetFullscreen, AlxWindow_ToggleFullscreenRex
SYSV_WRAPPER f.AlxWindow_SetMouseVisible, AlxWindow_SetMouseVisibleRex
SYSV_WRAPPER f.AlxWindow_SetMousePosition, AlxWindow_SetMousePositionRex
SYSV_WRAPPER f.AlxWindow_DrawRect, AlxWindow_DrawRectRex
SYSV_WRAPPER f.AlxWindow_DrawRectWire, AlxWindow_DrawRectWireRex
SYSV_WRAPPER f.AlxWindow_DrawCircle, AlxWindow_DrawCircleRex
SYSV_WRAPPER f.AlxWindow_DrawCircleWire, AlxWindow_DrawCircleWireRex

section .note.GNU-stack noalloc noexec nowrite progbits
