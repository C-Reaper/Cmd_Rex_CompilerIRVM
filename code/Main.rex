//import IO;
//import System;
//import CStr;

i64 len(i8* str){
    i64 size = 0;
    i8 ch = str[0];
    
    while ch != 0 {
        size += 1;
        ch = str[size];
    }
    
    return size;
}

global i64 main(){
    //io::print("Hello World!\n");
    //sys::write(1,"Hello World!\n",13);
    return len("Hello World");
}