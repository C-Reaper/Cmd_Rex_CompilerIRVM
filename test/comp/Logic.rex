i64 test_ifelifelse(i64 a){
    if a <= 1 {
        return 1;
    }elif a >= 20 {
        return 2;
    }elif a >= 10 {
        return 3;
    }else {
        return 4;
    }

    return 5;
}
i64 test_while(i64 a){
    i64 b = a;
    while b <= 10 {
        b = b + 1;
    }
    return b;
}
i64 test_for(i64 a){
    i64 b;
    for b = 0,b <= 10,++b {
        b = b + 1;
    }
    return b;
}

global i64 main(){
    i64 a = test_ifelifelse(5);
    i64 b = test_while(5);
    i64 c = test_for(5);
    return a + b + c;
}