//define NUMBER_PARSE_ERROR  9223372036854775801;
//define DOUBLE_PARSE_ERROR  1.2E-320;
//define BOOL_PARSE_ERROR    -1;

namespace math {
    define PI   3.141592654;
    define E    2.718281828;
    
    namespace lib {
        f64 sin(f64 x){
            asm "fld " x;
            asm "fsin";
            asm "fstp " x;
            return x;
        }
        f64 cos(f64 x){
            asm "fld " x;
            asm "fcos";
            asm "fstp " x;
            return x;
        }
        f64 tan(f64 x){
            return math::lib::sin(x) / math::lib::cos(x);
        }

        f64 atan(f64 x){
            asm "fld " x;
            asm "fpatan";
            asm "fstp " x;
            return x;
        }

        f64 f2xm1(f64 x){ // 2^x-1
            asm "fld " x;
            asm "f2xm1";
            asm "fstp " x;
            return x;
        }
        f64 yl2x(f64 x,f64 y){ // y * log2(x)
            asm "fld " y;
            asm "fld " x;
            asm "fyl2x";
            asm "fstp " x;
            return x;
        }
        f64 yl2xp1(f64 x,f64 y){ // y * log2(x+1)
            asm "fld " y;
            asm "fld " x;
            asm "fyl2xp1";
            asm "fstp " x;
            return x;
        }
        f64 sqrt(f64 x){
            asm "fld " x;
            asm "fsqrt";
            asm "fstp " x;
            return x;
        }
        f64 ln2(){ // ln(2)
            f64 x;
            asm "fldln2";
            asm "fstp " x;
            return x;
        }
        f64 log2_e(){ // log2(e)
            f64 x;
            asm "fldl2e";
            asm "fstp " x;
            return x;
        }
        f64 log2_10(){ // log2(10)
            f64 x;
            asm "fldl2t";
            asm "fstp " x;
            return x;
        }
        f64 pi(){ // 3.141592654
            f64 x;
            asm "fldpi";
            asm "fstp " x;
            return x;
        }
        f64 log10_2(){ // log10(2)
            f64 x;
            asm "fldlg2";
            asm "fstp " x;
            return x;
        }
    }

    namespace uint {
        u64 pow(u64 base,u64 ex){
            u64 out = 1;
            while ex>0 {
                out *= base;
                ex -= 1;
            }
            return out;
        }
        u64 logb(u64 base,u64 res){// BASE ab 16, res, out
            u64 out = -1;
            while res > 0 {
                res /= base;
                out += 1;
            }
            return out;
        }
        u64 log10(u64 res){
            return math::uint::logb(10,res);
        }
        
        u64 min(u64 a,u64 b){
            if a<b {
                return a;
            }
            return b;
        }
        u64 max(u64 a,u64 b){
            if a>b {
                return a;
            }
            return b;
        }
    }
    
    namespace int {
        i64 sign(i64 num){
            if num>0 {
                return 1;
            }elif num<0 {
                return -1;
            }
            return 0;
        }
        i64 min(i64 a,i64 b){
            if a<b {
                return a;
            }
            return b;
        }
        i64 max(i64 a,i64 b){
            if a>b {
                return a;
            }
            return b;
        }
    }

    namespace float {
        void setmode(u16 mode){
            asm "fldcw " mode;
        }
        void setstd(){
            u16 std = 0x037F;
            asm "fldcw " std;
        }

        f64 powi(f64 base,i64 ex){
            f64 out = 1.0;
            if ex<0 {
                base = 1.0 / base;
                ex = -ex;
            }
            while ex>0 {
                out *= base;
                ex -= 1;
            }
            return out;
        }

        f64 sign(f64 num){
            if num>0.0 {
                return 1.0;
            }elif num<0.0 {
                return -1.0;
            }
            return 0.0;
        }

        f64 ylnx(f64 x,f64 y){ // y * ln(x)
            return math::lib::yl2x(x,y) * math::lib::ln2();
        }
        f64 pow(f64 x,f64 y){ // x ^ y -> 2 ^ (y * log2(x))
            f64 ex = math::lib::yl2x(x,y);
            
            i64 ipart = (i64)ex;
            f64 fpart = ex - (f64)ipart;

            f64 iout = math::float::powi(2.0,ipart);
            f64 fout = math::lib::f2xm1(fpart) + 1.0;
            return iout * fout;
        }

        f64 ln(f64 x){ 
            return math::float::ylnx(x,1.0);
        }
        f64 ex(f64 x){ 
            return math::float::pow(2.718281828,x);
        }
    }

    /*
    namespace aprox {
        f64 sin_tp19(f64 x){
            x = x - 6.283185307179586476925286766559 * (f64)((i64)(x * 0.15915494309189533576888376337251));
            f64 x3 = x * x * x    * 0.16666666666666666666666666666667; // / 6.0;  
            f64 x5 = x3 * x * x   * 0.05000000000000000000000000000000; // / 20.0; 
            f64 x7 = x5 * x * x   * 0.02380952380952380952380952380952; // / 42.0; 
            f64 x9 = x7 * x * x   * 0.01388888888888888888888888888889; // / 72.0; 
            f64 x11 = x9 * x * x  * 0.00909090909090909090909090909091; // / 110;  
            f64 x13 = x11 * x * x * 0.00641025641025641025641025641026; // / 156;  
            f64 x15 = x13 * x * x * 0.00476190476190476190476190476190; // / 210;  
            f64 x17 = x15 * x * x * 0.00367647058823529411764705882353; // / 272;  
            f64 x19 = x17 * x * x * 0.00292397660818713450292397660819; // / 342;  
            return x - x3 + x5 - x7 + x9 - x11 + x13 - x15 + x17 - x19;
        }
        f64 sin_sq(f64 x){
            f64 y = x * 0.15915494309189533576888376337251;// / (2*PI);
            y = y - (f64)(i64)y;
            y = y + 1.0;
            y = y - (f64)(i64)y;
            if y<0.5 {
                return -16.0*y*y+8.0*y; 
            }
            return 16.0*y*y-16.0*y-8.0*y+8.0;
        }

        f64 ex_tp10(f64 x){
            f64 xe = x;
            f64 out = 1.0 + xe;
            xe *= x;
            out += xe * 0.5; // / 2.0;
            xe *= x;
            out += xe * 0.166666666;
            xe *= x;
            out += xe * 0.041666666;
            xe *= x;
            out += xe * 0.008333333;// 5
            xe *= x;
            out += xe * 0.001388888;
            xe *= x;
            out += xe * 0.000198413;
            xe *= x;
            out += xe * 0.000024802;
            xe *= x;
            out += xe * 0.000002756;
            xe *= x;
            out += xe * 0.000000276;// 10
            return out;
        }
        f64 ln_tp10(f64 x){
            x = x - 1.0;
            f64 xe = x;
            f64 out = xe;
            xe *= x;
            out -= xe * 0.5; // / 2.0;
            xe *= x;
            out += xe * 0.333333333;
            xe *= x;
            out -= xe * 0.25;
            xe *= x;
            out += xe * 0.2;// 5
            xe *= x;
            out -= xe * 0.166666667;
            xe *= x;
            out += xe * 0.142857143;
            xe *= x;
            out -= xe * 0.125;
            xe *= x;
            out += xe * 0.111111111;
            xe *= x;
            out -= xe * 0.1;// 10
            return out;
        }
    }
    */

    /*
    struct Vec2[
        pub f64 x,
        pub f64 y
    ];

    impl Vec2{
        pub Vec2 new(f64 x,f64 y){
            Vec2 vec;
            vec.x = x;
            vec.y = y;
            return vec;
        }

        pub Vec2 add(Vec2* self,Vec2 other){
            Vec2 vec;
            vec.x = self->x + other.x;
            vec.y = self->y + other.y;
            return vec;
        }
        pub Vec2 sub(Vec2* self,Vec2 other){
            Vec2 vec;
            vec.x = self->x - other.x;
            vec.y = self->y - other.y;
            return vec;
        }
        pub Vec2 mul(Vec2* self,f64 n){
            Vec2 vec;
            vec.x = self->x * n;
            vec.y = self->y * n;
            return vec;
        }
        pub Vec2 div(Vec2* self,f64 n){
            Vec2 vec;
            vec.x = self->x / n;
            vec.y = self->y / n;
            return vec;
        }

        pub f64 mag(Vec2* self){
            return math::lib::sqrt(self->x * self->x + self->y * self->y);
        }
        pub f64 mag2(Vec2* self){
            return self->x * self->x + self->y * self->y;
        }

        pub Vec2 norm(Vec2* self){
            return self->div(self->mag());
        }
        pub Vec2 perp(Vec2* self,f64 n){
            Vec2 vec;
            vec.x = -self->y;
            vec.y = self->x;
            return vec;
        }
        pub Vec2 perpa(Vec2* self,f64 n){
            Vec2 vec;
            vec.x = -self->y;
            vec.y = self->x;
            return vec;
        }
    }
    */
}