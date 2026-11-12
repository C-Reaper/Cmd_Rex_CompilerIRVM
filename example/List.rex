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
import List;

global i32 main(){
    i8[40000] buffer;
    mem::Allocator alloc = mem::Allocator::by(buffer,40000);

    ios::OStream cout = ios::OStream::new(&alloc,1);
    cout.cstr("Hello World\n")->flush();
    cout.delete();

    con::List list = con::List::new(&alloc);
    
    u64 value1 = 1;
    list.push(&value1,8);

    u64 value2 = 2;
    list.push(&value2,8);

    u64 value3 = 3;
    list.push(&value3,8);

    list.print();
    list.delete();

    //ios::IStream cin = ios::IStream::new(&alloc,0);
    //u64 input = cin.uint();
    //cin.delete();

    alloc.delete();
    return 0;
}