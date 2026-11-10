
global i64 main(i64 a,i64 b){
    i64 c = a == b;
    i64 d = a != b;
    i64 e = a < b;
    i64 f = a > b;
    i64 g = a <= b;
    i64 h = a >= b;
    i64 i = a && b;
    i64 j = a || b;
    i64 k = !j;
    return c + d + e + f + g + h + i + j + k;
}
