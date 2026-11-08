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
}

global i64 main(i64 a,i64 b){
    Human h_a = Human::new(a);
    Human h_b = Human::new(b);
    Human* ph_b = &h_b;
    Human** pph_b = &ph_b;

    i64 result_a = (h_a.a.a + h_a.a.b + h_a.size);
    i64 result_b = (h_b.a.a + h_b.a.b + h_b.size);
    i64 result_p = (ph_b->a.a + ph_b->a.b + (*ph_b).size);
    i64 result_pp = ((*pph_b)->a.a + (*pph_b)->a.b + (**pph_b).size);

    return result_a + result_b + result_p + result_pp;
}
