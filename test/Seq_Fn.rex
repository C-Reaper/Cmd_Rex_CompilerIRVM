i64 add(i64 a,i64 b){
    return a + b;
}

global i64 main(i64 a,i64 b){
    i64 res = add(add(a,b) + add(a,b)) + add(add(a,b) + add(a,b));
    return a + b + add(a,b);
}