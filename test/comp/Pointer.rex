global i64 main(){
    i64 a = 0;
    i64* b = &a;
    i64** c = &b;
    i64*** d = &c;
    *b = 1;
    **c = 2;
    ***d = 3;
    return (**d)[0];
}