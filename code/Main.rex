import Parser;
import System;
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
import String;

global i32 main(){
    i8[40000] buffer;
    mem::Allocator alloc = mem::Allocator::by(buffer,40000);

    ios::OStream cout = ios::OStream::new(&alloc,1);
    cout.cstr("Hello World\n")->flush();
    cout.delete();

    ios::IStream cin = ios::IStream::new(&alloc,0);
    u64 input = cin.uint();
    cin.delete();

    alloc.delete();
    return 0;
}