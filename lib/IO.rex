import "../lib/System.rex";
import "../lib/CStr.rex";

namespace io {
    void input(i8* cstr,i64 len){
        sys::read(0,cstr,len);
    }
    void print(i8* cstr){
        i64 len = cstr::len(cstr);
        sys::write(1,cstr,len);
    }
    void error(i8* cstr){
        i64 len = cstr::len(cstr);
        sys::write(2,cstr,len);
    }

    void print_char(i8 ch){
        sys::write(1,&ch,1);
    }
}