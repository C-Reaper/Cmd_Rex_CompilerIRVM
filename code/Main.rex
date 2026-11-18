

global i32 main(){
    i32 a = 1;
    i32 b = 1;
    a = 2;
    b = 2;
    
    while a < 10 {
        while b < 10 {
            if a < 5 {
                a = a + b;
            }elif a < 8 {
                a = a + b + 1;
            }else{
                b = a + 1;
            }
        }
    }

    return a + 1;
}