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

    fsys::File stdout = fsys::File::new(1);
    stdout.write("Hello World!\n",13);
    stdout.close();

    alloc.delete();
    return 0;
}