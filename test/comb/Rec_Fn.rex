i64 fib(i64 n){
    if n <= 1 {
        return n;
    }
    return fib(n - 2) + fib(n - 1);
}

global i64 main(i64 a,i64 b){
    return fib(a + b);
}