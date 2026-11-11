import Parser;
import System;
import CStr;
import Char;
import Pointer;
import IO;
import Time;
import Memory;

global i64 main(){
    //i8[10000] buffer;
    //mem::Allocator alloc = mem::Allocator::by(buffer,10000);
    mem::Allocator alloc = mem::Allocator::make(10000);

    alloc.print();
    
    void* ptr0 = alloc.alloc(10);
    void* ptr1 = alloc.alloc(20);
    void* ptr2 = alloc.alloc(30);

    alloc.print();

    alloc.free(ptr1);

    alloc.print();
    
    alloc.free(ptr2);

    alloc.print();
    alloc.delete();
    return 0;
}