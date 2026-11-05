//import Lib;

struct Animal[
    pub i64 a,
    pub i64 b,
];

struct Human[
    pub Animal a,
    pub i64 size
];

impl Human{
    pub Human new(i64 size){
        Human h;
        h.a.a = 0;
        h.a.b = 1;
        h.size = size;
        return h;
    }
    pub Human add(Human hadd){
        Human set;
        set.a.a = hadd.a.a;
        set.a.b = hadd.a.b;
        set.size = hadd.size;
        return set;
    }
    pub Human sub(Human hadd){
        return hadd;
    }
}

global i64 main(){
    Human a = Human::new(10);
    //Human b = Human::add(a);
    return a.a.a + a.a.b + a.size;
}
