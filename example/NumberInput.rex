import Parser;
import System;
import CStr;
import Char;
import Pointer;
import IO;
import Time;

global i64 main(){
    io::print("-- Start --\n");

    i8[32] buffer;
    ptr::set(buffer,0,32);
    io::input(buffer,32);
    
    u64 num = parse::uint::by(buffer);

    io::print("-----------\n");
    return num;
}