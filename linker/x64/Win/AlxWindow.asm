bits 64

%macro WIN_WRAPPER 2
global %1
extern %2

%1:
    push rbp
    mov rbp, rsp
    and rsp, -16
    sub rsp, 0x30
    mov rcx, rsi
    mov rdx, rdi
    mov [rsp + 0x20], r10
    mov [rsp + 0x28], r11
    call %2
    mov rbx, rax
    mov rsp, rbp
    pop rbp
    ret
%endmacro

section .text

WIN_WRAPPER f.AlxWindow_Render, AlxWindow_Render
WIN_WRAPPER f.AlxWindow_Update, AlxWindow_UpdateRex
WIN_WRAPPER f.AlxWindow_IsRunning, AlxWindow_IsRunningRex
WIN_WRAPPER f.AlxWindow_PreInit, AlxWindow_PreInit
WIN_WRAPPER f.AlxWindow_Init, AlxWindow_InitRex
WIN_WRAPPER f.AlxWindow_Free, AlxWindow_FreeRex
WIN_WRAPPER f.AlxWindow_GetBuffer, AlxWindow_GetBufferRex
WIN_WRAPPER f.AlxWindow_GetWidth, AlxWindow_GetWidthRex
WIN_WRAPPER f.AlxWindow_GetHeight, AlxWindow_GetHeightRex
WIN_WRAPPER f.AlxWindow_GetElapsedNs, AlxWindow_GetElapsedNsRex
WIN_WRAPPER f.AlxWindow_GetMouseX, AlxWindow_GetMouseXRex
WIN_WRAPPER f.AlxWindow_GetMouseY, AlxWindow_GetMouseYRex
WIN_WRAPPER f.AlxWindow_GetMouseBeforeX, AlxWindow_GetMouseBeforeXRex
WIN_WRAPPER f.AlxWindow_GetMouseBeforeY, AlxWindow_GetMouseBeforeYRex
WIN_WRAPPER f.AlxWindow_GetStrokeCount, AlxWindow_GetStrokeCountRex
WIN_WRAPPER f.AlxWindow_GetStrokeState, AlxWindow_GetStrokeStateRex
WIN_WRAPPER f.AlxWindow_SetName, AlxWindow_SetNameRex
WIN_WRAPPER f.AlxWindow_SetFullscreen, AlxWindow_ToggleFullscreenRex
WIN_WRAPPER f.AlxWindow_SetMouseVisible, AlxWindow_SetMouseVisibleRex
WIN_WRAPPER f.AlxWindow_SetMousePosition, AlxWindow_SetMousePositionRex
WIN_WRAPPER f.AlxWindow_DrawRect, AlxWindow_DrawRectRex
WIN_WRAPPER f.AlxWindow_DrawRectWire, AlxWindow_DrawRectWireRex
WIN_WRAPPER f.AlxWindow_DrawCircle, AlxWindow_DrawCircleRex
WIN_WRAPPER f.AlxWindow_DrawCircleWire, AlxWindow_DrawCircleWireRex
