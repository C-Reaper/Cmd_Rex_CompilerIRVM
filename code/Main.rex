

global i32 main(){
    i32 a = 1;
    i32 b = 1;
    a = 2;
    b = 2;
    
    while a < 10 {
        a = a + b;
    }

    return a + 1;
}