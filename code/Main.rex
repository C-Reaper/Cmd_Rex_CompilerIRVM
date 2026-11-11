import Parser;
import System;
import CStr;
import Char;
import Pointer;
import IO;
import Time;

global i64 main(){
    io::print("-- Start --\n");
    
    for i64 i = 0,i<5,++i {
        u64 tp = time::nget();
        io::print_uint(tp / 1000000000);
        io::print("\n");
        time::nsleep(1000000000);
    }
    
    io::print("-----------\n");
    time::nsleep(1000000000);
    return 0;
}