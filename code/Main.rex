i64 fn_add(i64 a,i64 b){
    return a + b;
}
i64 fn_sub(i64 a,i64 b,i64 c,i64 d,i64 e){
    return a + b - c * d / e;
}

i64 main(){
    i64 a = fn_add(1,2);
    i64 b = fn_sub(3,4,5,6,7);
    return a - b;
}