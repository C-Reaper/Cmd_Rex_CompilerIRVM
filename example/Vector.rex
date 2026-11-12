import Parser;
import System;
import CStr;
import Char;
import Pointer;
import IO;
import Time;
import Memory;
import OStream;
//import IStream;
import Vector;

global i32 main(){
    i8[40000] buffer;
    mem::Allocator alloc = mem::Allocator::by(buffer,40000);

    ios::OStream cout = ios::OStream::new(&alloc,1);
    cout.cstr("Hello World\n")->flush();
    cout.delete();

    con::Vector vec = con::Vector::new(&alloc,8);
    
    u64 value1 = 1;
    vec.push(&value1);

    u64 value2 = 2;
    vec.push(&value2);

    u64 value3 = 3;
    vec.push(&value3);

    vec.print();
    vec.delete();

    //ios::IStream cin = ios::IStream::new(&alloc,0);
    //u64 input = cin.uint();
    //cin.delete();

    alloc.delete();
    return 0;
}