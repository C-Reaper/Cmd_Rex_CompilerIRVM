extern void AlxWindow_Render(void* alxw);
extern void AlxWindow_Update(void* alxw);
extern i32 AlxWindow_IsRunning(void* alxw);
extern void AlxWindow_PreInit(void* alxw);
extern void AlxWindow_Init(void* alxw,i8* name,i32 w,i32 h,i32 fw,i32 fh);
extern void AlxWindow_Free(void* alxw);
extern void* AlxWindow_GetBuffer(void* alxw);
extern i32 AlxWindow_GetWidth(void* alxw);
extern i32 AlxWindow_GetHeight(void* alxw);
extern u64 AlxWindow_GetElapsedNs(void* alxw);
extern i32 AlxWindow_GetMouseX(void* alxw);
extern i32 AlxWindow_GetMouseY(void* alxw);
extern i32 AlxWindow_GetMouseBeforeX(void* alxw);
extern i32 AlxWindow_GetMouseBeforeY(void* alxw);
extern u32 AlxWindow_GetStrokeCount(void* alxw);
extern i32 AlxWindow_GetStrokeState(void* alxw,u32 index,void* state);
extern i32 AlxWindow_SetName(void* alxw,i8* name);
extern void AlxWindow_SetFullscreen(void* alxw);
extern i32 AlxWindow_SetMouseVisible(void* alxw,i32 visible);
extern i32 AlxWindow_SetMousePosition(void* alxw,i32 x,i32 y);
extern i32 AlxWindow_DrawRect(void* buffer,i32 width,i32 height,void* rect);
extern i32 AlxWindow_DrawRectWire(void* buffer,i32 width,i32 height,void* rect);
extern i32 AlxWindow_DrawCircle(void* buffer,i32 width,i32 height,void* circle);
extern i32 AlxWindow_DrawCircleWire(void* buffer,i32 width,i32 height,void* circle);

namespace win {
    struct RectDraw[
        pub i32 x,
        pub i32 y,
        pub i32 width,
        pub i32 height,
        pub u32 color,
        pub u32 stroke
    ];

    struct CircleDraw[
        pub i32 x,
        pub i32 y,
        pub i32 radius,
        pub u32 color,
        pub u32 stroke
    ];

    struct StrokeState[
        pub u32 down,
        pub u32 pressed,
        pub u32 released,
        pub u32 repeat
    ];

    struct Window[
        pub void* win,
        pub mem::Allocator* alloc
    ];

    impl Window{
        pub void init(Window* self,mem::Allocator* alloc,i8* name,u32 width,u32 height){
            void* win = alloc->alloc(608);
            AlxWindow_PreInit(win);
            AlxWindow_Init(win,name,width,height,1,1);
            
            self->win = win;
            self->alloc = alloc;
        }
        pub void delete(Window* self){
            AlxWindow_Free(self->win);
            self->alloc->free(self->win);
            self->win = null;
        }
        pub Window new(mem::Allocator* alloc,i8* name,u32 width,u32 height){
            Window w;
            w.init(alloc,name,width,height);
            return w;
        }

        pub void update(Window* self){
            AlxWindow_Update(self->win);
        }
        pub void render(Window* self){
            AlxWindow_Render(self->win);
        }
        pub i32 is_running(Window* self){
            return AlxWindow_IsRunning(self->win);
        }
        pub void* buffer(Window* self){
            return AlxWindow_GetBuffer(self->win);
        }
        pub i32 width(Window* self){
            return AlxWindow_GetWidth(self->win);
        }
        pub i32 height(Window* self){
            return AlxWindow_GetHeight(self->win);
        }
        pub u64 elapsed_ns(Window* self){
            return AlxWindow_GetElapsedNs(self->win);
        }
        pub i32 mouse_x(Window* self){
            return AlxWindow_GetMouseX(self->win);
        }
        pub i32 mouse_y(Window* self){
            return AlxWindow_GetMouseY(self->win);
        }
        pub i32 mouse_before_x(Window* self){
            return AlxWindow_GetMouseBeforeX(self->win);
        }
        pub i32 mouse_before_y(Window* self){
            return AlxWindow_GetMouseBeforeY(self->win);
        }
        pub u32 stroke_count(Window* self){
            return AlxWindow_GetStrokeCount(self->win);
        }
        pub i32 get_stroke(Window* self,u32 index,StrokeState* state){
            return AlxWindow_GetStrokeState(self->win,index,state);
        }
        pub i32 set_name(Window* self,i8* name){
            return AlxWindow_SetName(self->win,name);
        }
        pub void toggle_fullscreen(Window* self){
            AlxWindow_SetFullscreen(self->win);
        }
        pub i32 set_mouse_visible(Window* self,i32 visible){
            return AlxWindow_SetMouseVisible(self->win,visible);
        }
        pub i32 set_mouse_position(Window* self,i32 x,i32 y){
            return AlxWindow_SetMousePosition(self->win,x,y);
        }
        pub i32 draw_rect(Window* self,void* buffer,RectDraw* rect){
            return AlxWindow_DrawRect(buffer,self->width(),self->height(),rect);
        }
        pub i32 draw_rect_wire(Window* self,void* buffer,RectDraw* rect){
            return AlxWindow_DrawRectWire(buffer,self->width(),self->height(),rect);
        }
        pub i32 draw_circle(Window* self,void* buffer,CircleDraw* circle){
            return AlxWindow_DrawCircle(buffer,self->width(),self->height(),circle);
        }
        pub i32 draw_circle_wire(Window* self,void* buffer,CircleDraw* circle){
            return AlxWindow_DrawCircleWire(buffer,self->width(),self->height(),circle);
        }
    }
}