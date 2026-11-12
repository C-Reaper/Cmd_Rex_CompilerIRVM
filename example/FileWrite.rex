import System;
import Memory;
import IO;
import File;

//import Parser;
//import CStr;
//import Char;
//import Pointer;
//import Time;
//import OStream;
//import Vector;
//import List;
//import String;
//import IStream;

global i32 main(){
    //i8[65535] buffer;
    //mem::Allocator alloc = mem::Allocator::by(buffer,65535);
    mem::Allocator alloc = mem::Allocator::make(65535);

    //fsys::File file = fsys::File::new(1);
    //file.write("Hello World!\n",13);
    //file.close();

    fsys::File file = fsys::File::open("/home/codeleaded/Hecke/C/Cmd_Rex_CompilerIRVM/code/Hello.txt",O_WRONLY | 64);
    if file.isopen() {
        file.write("Hello World!\n",13);
        io::print("Done.\n");
    }else{
        io::print("Not open.\n");
    }
    file.close();

    alloc.delete();
    return 0;
}