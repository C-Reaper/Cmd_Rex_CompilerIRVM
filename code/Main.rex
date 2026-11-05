//import Lib;

struct Animal [
    i64 a
];

struct Human [
    Animal a
];

global i64 main(){
    Human a;
    a.a.a = 1;
    Human* b = &a;
    return b->a.a;
    //Human b = a;
    //Human* c = &a;
    //Human d = *c;
}

/*

*/