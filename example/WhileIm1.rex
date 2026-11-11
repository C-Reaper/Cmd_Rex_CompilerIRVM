
global i64 main(){
    i64 size = 10;
    i64 i = size-1;
    
    while i >= 0 {
        i8 c = 48;

        if c >= 48 && c <= 57 {
            i -= 1;
        }else{
            return 0;
        }
    }
    
    return i;
}