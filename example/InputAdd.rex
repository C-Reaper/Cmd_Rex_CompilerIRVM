import Parser;
import System;
import CStr;
import Char;
import Pointer;
import IO;
import Time;
import Memory;
import OStream;
import Vector;
import List;
import String;
import IStream;

global i32 main(){
    i8[40000] buffer;
    mem::Allocator alloc = mem::Allocator::by(buffer,40000);

    ios::IStream cin = ios::IStream::new(&alloc,0);
    u64 input1 = cin.uint();
    u64 input2 = cin.uint();
    cin.delete();

    alloc.delete();
    return input1 + input2;
}