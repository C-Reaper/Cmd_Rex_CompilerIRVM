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
    Human a = Human::new(10);
    a.delete(5);
    a.delete(5);
    return a.size;
    //Human b = a;
    //Human* c = &a;
    //Human d = *c;
}