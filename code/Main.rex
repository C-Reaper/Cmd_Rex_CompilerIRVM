//import Lib;

struct Animal [
    i64 a,
    i64 b
];

struct Human [
    Animal a,
    i32 b
];

global i64 main(){
    Human a;
    a.a.a = 1;
    a.a.b = 2;
    a.b = 3;
    return a.a.a + a.a.b + a.b;
    //Human b = a;
    //Human* c = &a;
    //Human d = *c;
}

/*

allocs 20 %a
add ptr %0 %a 0
add ptr %1 %0 0
store i64 %1 1

add ptr %2 %a 0
add ptr %3 %2 8
store i64 %3 2

add ptr %4 %a 16
store i32 %4 3

add ptr %5 %a 0
add ptr %6 %5 0

add ptr %7 %a 0
add ptr %8 %7 8

load i64 %9 %6
load i64 %10 %8
add i64 %11 %9 %10

add ptr %12 %a 16
load i32 %13 %12
add i64 %14 %11 %13
ass i64 &0 %14
destroy %a
ret
destroy %a
ret

*/