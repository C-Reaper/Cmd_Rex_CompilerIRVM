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
        h.a.a = 1;
        h.a.b = 2;
        h.size = size;
        return h;
    }
    pub Human cpy(Human hadd){
        Human set;
        set.a.a = hadd.a.a;
        set.a.b = hadd.a.b;
        set.size = hadd.size;
        return set;
    }
    pub i64 dec(Human* self){
        self->size -= 1;
        return self->size;
    }
}

global i64 main(i64 a,i64 b){
    Human h_a = Human::new(a);
    Human h_b = Human::cpy(h_a);
    i64 c = h_a.dec();
    return a + b + c + (h_a.a.a + h_a.a.b + h_a.size) + (h_b.a.a + h_b.a.b + h_b.size);
}
