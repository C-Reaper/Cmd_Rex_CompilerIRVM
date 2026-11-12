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
    
    fsys::File file = fsys::File::open("/home/codeleaded/Hecke/C/Cmd_Rex_CompilerIRVM/code/Main.rex",O_RDONLY);
    if file.isopen() {
        io::print("Open, Reading.\n");
        
        u64 bytes_read = 1;
        while bytes_read > 0 {
            i8[32] buffer;
            bytes_read = file.read(buffer,32);
            
            if bytes_read > 0 {
                stdout.write(buffer,bytes_read);
            }
        }
    }else{
        io::print("Not open.\n");
    }
    file.close();

    stdout.close();

    alloc.delete();
    return 0;
}