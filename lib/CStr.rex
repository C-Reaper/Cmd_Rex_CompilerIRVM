
namespace cstr {
    i64 len(i8* str){
        i64 size = 0;
        i8 ch = str[0];
        
        while ch != 0 {
            size += 1;
            ch = str[size];
        }
        
        return size;
    }
    void set(i8* ptr,i8 value,i64 size){
        i64 i = 0;
        
        while i<size {
            ptr[i] = value;
            i += 1;
        }
    }
    void cpy(i8* dst,i8* src,i64 size){
        i64 i = 0;
        while i<size {
            dst[i] = src[i];
            i += 1;
        }
    }
}