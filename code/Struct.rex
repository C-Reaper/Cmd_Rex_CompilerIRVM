struct Human[
    pub i64 size
];

impl Human{
    pub void init(Human* self,i64 size){
        self->size = size;
    }
    pub void delete(Human* self,i64 size){
        self->size -= size;
    }
    pub Human new(i64 size){
        Human h;
        h.init(size);
        return h;
    }
}

global i64 main(){
    Human a = Human::new(20);
    a.delete(10);
    a.delete(5);
    return 1 + a.size;
    //Human b = a;
    //Human* c = &a;
    //Human d = *c;
}


struct Human [
   pub i64 a,
   pub i64 b,
   pub i64 c,
   pub i64 d,
   pub i32 e,
   pub i32 f,
   pub i16 g,
   pub i16 h,
   pub i16 i,
   pub i16 j
];

Human newh(i64 a,i64 b,i64 c,i64 d,i32 e,i32 f,i16 g,i16 h,i16 i,i16 j){
    Human hm;
    hm.a = a;
    hm.b = b;
    hm.c = c;
    hm.d = d;
    hm.e = e;
    hm.f = f;
    hm.g = g;
    hm.h = h;
    hm.i = i;
    hm.j = j;
    return hm;
}
Human addh(Human a,Human b){
    Human h;
    h.a = a.a + b.a;
    h.b = a.b + b.b;
    h.c = a.c + b.c;
    h.d = a.d + b.d;
    h.e = a.e + b.e;
    h.f = a.f + b.f;
    h.g = a.g + b.g;
    h.h = a.h + b.h;
    h.i = a.i + b.i;
    h.j = a.j + b.j;
    return h;
}
i64 foldh(Human a){
    return a.a + a.b + a.c + a.d + a.e + a.f + a.g + a.h + a.i + a.j;
}
i64 add10(i64 a,i64 b,i64 c,i64 d,i32 e,i32 f,i16 g,i16 h,i16 i,i16 j){
    return a + b + c + d + e + f + g + h + i + j;
}
i64 change(Human* a){
    a->a = 10;
    a->j = 20;
    return a->a + a->j;
}

global i64 main(){
    Human a = newh(1,2,3,4,5,6,7,8,9,10);
    Human b = newh(a.a,a.b,a.c,a.d,a.e,a.f,a.g,a.h,a.i,a.j);
    Human h = addh(a,b);
    return change(&a) + foldh(a) + foldh(b) + foldh(h) + add10(1,2,3,4,5,6,7,8,9,10) + add10(1,2,3,4,5,6,7,8,9,10);
    //return h.a + h.b + h.c + h.d + h.e + h.f + h.g + h.h + h.i + h.j;
}
