extern i64 fn_ext(i64 a,i64 b);

i64 fn_add(i64 a,i64 b){
    i64 c = a + b;
    i64 d = a - b;
    i64 e = a * b;
    i64 f = a / b;
    i64 g = a % b;

    i64 h = -a;
    i64 i = ++h;
    i64 j = --h;
    i64 k = ~a;
    i64 l = a & b;
    i64 m = a | b;
    i64 n = a ^ b;
    i64 o = a << b;
    i64 p = a >> b;
    return c;
}
i64 fn_sub(i64 a,i64 b){
    i64 c = a;
    i64 q = a && b;
    i64 r = a || b;
    i64 s = !a;

    i64 t = a == b;
    i64 u = a != b;
    i64 v = a < b;
    i64 w = a > b;
    i64 x = a <= b;
    i64 y = a >= b;

    c += a;
    c -= a;
    c *= a;
    c /= a;
    return c;
}

i64 fn_addc(i64 a,i64 b){
    i64 c = 1 + 2;
    i64 d = 1 - 2;
    i64 e = 1 * 2;
    i64 f = 1 / 2;
    i64 g = 1 % 2;

    i64 h = -1;
    i64 i = ++h;
    i64 j = --h;
    i64 k = ~1;
    i64 l = 1 & 2;
    i64 m = 1 | 2;
    i64 n = 1 ^ 2;
    i64 o = 1 << 2;
    i64 p = 1 >> 2;
    return c;
}
i64 fn_subc(i64 a,i64 b){
    i64 c = 1;
    i64 q = 1 && 2;
    i64 r = 1 || 2;
    i64 s = !1;

    i64 t = 1 == 2;
    i64 u = 1 != 2;
    i64 v = 1 < 2;
    i64 w = 1 > 2;
    i64 x = 1 <= 2;
    i64 y = 1 >= 2;

    c += 1;
    c -= 1;
    c *= 1;
    c /= 1;
    return c;
}