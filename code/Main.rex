import System;
import Parser;
import CStr;
import Char;
import Pointer;
import IO;
import Time;
import Memory;
import OStream;
import IStream;
import Vector;
import List;
import Window;

global i32 main(){
    //i8[40000] buffer;
    //mem::Allocator alloc = mem::Allocator::by(buffer,40000);
    
    mem::Allocator alloc = mem::Allocator::make(40000);
    win::Window win = win::Window::new(&alloc,"Rexlang Window",800,600);
    win.set_name("Rexlang Window Wrappers");

    win::RectDraw rect;
    rect.x = 60;
    rect.y = 60;
    rect.width = 220;
    rect.height = 140;
    rect.color = 0xff4040ff;
    rect.stroke = 2;

    win::RectDraw rect_wire;
    rect_wire.x = 45;
    rect_wire.y = 45;
    rect_wire.width = 250;
    rect_wire.height = 170;
    rect_wire.color = 0xffffffff;
    rect_wire.stroke = 2;

    win::CircleDraw circle;
    circle.x = 430;
    circle.y = 150;
    circle.radius = 55;
    circle.color = 0xff40ff40;
    circle.stroke = 2;

    win::CircleDraw circle_wire;
    circle_wire.x = 430;
    circle_wire.y = 150;
    circle_wire.radius = 80;
    circle_wire.color = 0xffff4040;
    circle_wire.stroke = 2;

    while win.is_running() {
        win.update();

        void* buffer = win.buffer();
        win.draw_rect(buffer,&rect);
        win.draw_rect_wire(buffer,&rect_wire);
        win.draw_circle(buffer,&circle);
        win.draw_circle_wire(buffer,&circle_wire);

        win.render();
        sys::nanosleep(1000000);
    }

    win.delete();
    alloc.delete();
    return 0;
}