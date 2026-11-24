#include <stdint.h>

#if defined(_WIN32)
#include "AlxWindow_Windows.h"
#elif defined(__linux__)
#include "AlxWindow_Linux.h"
#else
#error "Unsupported AlxWindow platform"
#endif

#include "Rect.h"
#include "Circle.h"

void AlxWindow_InitRex(
    AlxWindow* w,
    char* name,
    int width,
    int height,
    int font_width,
    int font_height
){
    AlxWindow_Init(w,name,width,height,font_width,font_height,NULL,NULL,NULL,NULL);
}

int AlxWindow_IsRunningRex(AlxWindow* w){
    return w->Running;
}

void AlxWindow_UpdateRex(AlxWindow* w){
    AlxWindow_GameUpdate(w);
}

void AlxWindow_FreeRex(AlxWindow* w){
    AlxWindow_Free(w);
    free(w->Name);
    w->Name = NULL;
}

void* AlxWindow_GetBufferRex(AlxWindow* w){
    return w->Buffer;
}

int AlxWindow_GetWidthRex(AlxWindow* w){
    return w->Width;
}

int AlxWindow_GetHeightRex(AlxWindow* w){
    return w->Height;
}

uint64_t AlxWindow_GetElapsedNsRex(AlxWindow* w){
    return (uint64_t)(w->ElapsedTime * 1000000000.0);
}

int AlxWindow_GetMouseXRex(AlxWindow* w){
    return w->MouseX;
}

int AlxWindow_GetMouseYRex(AlxWindow* w){
    return w->MouseY;
}

int AlxWindow_GetMouseBeforeXRex(AlxWindow* w){
    return w->MouseBeforeX;
}

int AlxWindow_GetMouseBeforeYRex(AlxWindow* w){
    return w->MouseBeforeY;
}

unsigned int AlxWindow_GetStrokeCountRex(AlxWindow* w){
    if(!w){
        return 0;
    }
    return MAX_STROKES;
}

typedef struct AlxWindowRexStrokeState {
    uint32_t down;
    uint32_t pressed;
    uint32_t released;
    uint32_t repeat;
} AlxWindowRexStrokeState;

int AlxWindow_GetStrokeStateRex(
    AlxWindow* w,
    unsigned int index,
    AlxWindowRexStrokeState* result
){
    if(!w || !result || index >= MAX_STROKES){
        return 0;
    }

    result->down = w->Strokes[index].DOWN;
    result->pressed = w->Strokes[index].PRESSED;
    result->released = w->Strokes[index].RELEASED;
    result->repeat = w->Strokes[index].REPEAT;
    return 1;
}

int AlxWindow_SetNameRex(AlxWindow* w,char* name){
    if(!w || !name){
        return 0;
    }

    const size_t name_length = strlen(name) + 1U;
    char* new_name = (char*)malloc(name_length);
    if(!new_name){
        return 0;
    }
    memcpy(new_name,name,name_length);

#if defined(_WIN32)
    if(!w->hwnd || !SetWindowTextA(w->hwnd,new_name)){
        free(new_name);
        return 0;
    }
#else
    if(!w->display || !w->alxwindow || !XStoreName(w->display,w->alxwindow,new_name)){
        free(new_name);
        return 0;
    }
    XFlush(w->display);
#endif

    free(w->Name);
    w->Name = new_name;
    return 1;
}

void AlxWindow_ToggleFullscreenRex(AlxWindow* w){
    AlxWindow_SetFullscreen(w);
}

int AlxWindow_SetMouseVisibleRex(AlxWindow* w,int visible){
#if defined(_WIN32)
    if(!w || !w->hwnd){
        return 0;
    }
#else
    if(!w || !w->display || !w->alxwindow){
        return 0;
    }
#endif

    if(visible){
        AlxWindow_Mouse_SetVisible(w);
    }else{
        AlxWindow_Mouse_SetInvisible(w);
    }
    return 1;
}

int AlxWindow_SetMousePositionRex(AlxWindow* w,int x,int y){
#if defined(_WIN32)
    if(!w || !w->hwnd){
        return 0;
    }
#else
    if(!w || !w->display || !w->alxwindow){
        return 0;
    }
#endif

    AlxWindow_Mouse_Set(w,(Vec2){ (float)x,(float)y });
    return 1;
}

typedef struct AlxWindowRexRectDraw {
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
    uint32_t color;
    uint32_t stroke;
} AlxWindowRexRectDraw;

typedef struct AlxWindowRexCircleDraw {
    int32_t x;
    int32_t y;
    int32_t radius;
    uint32_t color;
    uint32_t stroke;
} AlxWindowRexCircleDraw;

int AlxWindow_DrawRectRex(
    Pixel* buffer,int width,int height,const AlxWindowRexRectDraw* rect
){
    if(!buffer || !rect || width <= 0 || height <= 0 || rect->width <= 0 || rect->height <= 0){
        return 0;
    }
    Rect_RenderXX(
        buffer,width,height,(float)rect->x,(float)rect->y,
        (float)rect->width,(float)rect->height,rect->color
    );
    return 1;
}

int AlxWindow_DrawRectWireRex(
    Pixel* buffer,int width,int height,const AlxWindowRexRectDraw* rect
){
    if(!buffer || !rect || width <= 0 || height <= 0
        || rect->width <= 0 || rect->height <= 0 || rect->stroke == 0){
        return 0;
    }
    Rect_RenderXXWire(
        buffer,width,height,(float)rect->x,(float)rect->y,
        (float)rect->width,(float)rect->height,rect->color,(float)rect->stroke
    );
    return 1;
}

int AlxWindow_DrawCircleRex(
    Pixel* buffer,int width,int height,const AlxWindowRexCircleDraw* circle
){
    if(!buffer || !circle || width <= 0 || height <= 0 || circle->radius <= 0){
        return 0;
    }
    Circle_RenderX(
        buffer,width,height,(Vec2){ (float)circle->x,(float)circle->y },
        (float)circle->radius,circle->color
    );
    return 1;
}

int AlxWindow_DrawCircleWireRex(
    Pixel* buffer,int width,int height,const AlxWindowRexCircleDraw* circle
){
    if(!buffer || !circle || width <= 0 || height <= 0
        || circle->radius <= 0 || circle->stroke == 0){
        return 0;
    }
    Circle_RenderXWire(
        buffer,width,height,(Vec2){ (float)circle->x,(float)circle->y },
        (float)circle->radius,circle->color,(float)circle->stroke
    );
    return 1;
}
