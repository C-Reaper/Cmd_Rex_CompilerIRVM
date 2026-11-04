struct Animal [
    i32 a,
    i32 b
];

struct Human [
    i32 a,
    i32 b,
    Animal c
];

impl Human {
    Human new(){
        Human h;
        h.a = 1;
        h.b = 2;
        h.c.a = 3;
        h.c.b = 4;
        return h;
    }
    void sayHello(Human* self){
        print("Hello from Human\n",17);
    }
    Animal getA(Human* self){
        self->sayHello();
        return (*self).c;
    }
}

i8* buffer = "             ";
input(buffer,12);
*(buffer + 0) = 48;
buffer[1] = 49;
print(buffer,12);
bool a = 1 == 2;
return buffer[2];

Human h = Human::new();
h.sayHello();
Animal a = h.getA();
print("Hello World\n",12);
return a.a + a.b;

for i64 i = 0, i<3 , i = i + 1 {
    i64 a = 0;
    i64 b = 1;
    i64 j = 0;
    while j<5 {
        if a > b {
            io::print("Greater\n",8);
        }
        elif a < b {
            io::print("Less\n",5);
        }
        else{
            io::print("Equal\n",6);
        }
        j = j + 1;
        b = b + 1;
    }
}

i64 a = 69;
i64* pa = &a;
i64** ppa = &pa;
i64*** pppa = &ppa;
i64**** ppppa = &pppa;
return ****ppppa;


i64 func1(i64 a,i64 b){
    return a + b;
}
i64 func2(i64(i64,i64)* a,i64(i64,i64)* b){
    return a(1,2) + b(3,4);
}

i64(i64(i64,i64)*,i64(i64,i64)*)* f = func2;
i64 a = f(func1,func1);

i64 a = 1;
i64 b = 2;
a = b;
a += b + 1;
a++;
a--;
i64 c = a % 2;


struct Animal [
    pub i32 a,
    pub i32 b
];

struct Human [
    pub i32 a,
    pub i32 b,
    pub Animal c
];

impl Human {
    Human new(){
        Human h;
        h.a = 1;
        h.b = 2;
        h.c.a = 3;
        h.c.b = 4;
        return h;
    }
    pub void sayHello(Human* self){
        io::print("Hello from Human\n",17);
    }
    pub Animal getA(Human* self){
        self->sayHello();
        return self->c;
    }
}

Human h = Human::new();
h.sayHello();
Animal a = h.getA();
return a.a + a.b;

f64 a = 1.5;
f64 b = 3.0;
f64 c = a + b;
return (i64)(a + b);

win::Window w = win::Window::new(400,300,"Hello World",&alloc);
w.delete();

namespace std {
    struct Human[
        pub i8* name,
        pub i64 age
    ];

    impl Human{
        pub void init(Human* self,i8* name,i64 age){
            self->name = name;
            self->age = age;
        }
        pub void delete(Human* self){
            
        }
        pub Human new(i8* name,i64 age){
            Human vec;
            vec.init(name,age);
            return vec;
        }

        pub void* print_name(Human* self,i8* name,i64 age,i64 b,i64 c){
            io::print(name);
            io::print("\n");
            io::print_int(age);
            io::print("\n");
            io::print_int(b);
            io::print("\n");
            io::print_int(c);
            io::print("\n");
            return name;
        }
        pub void print_all(Human* self){
            i8* name = self->print_name(self->name,self->age + self->age - self->age,self->age - 1,self->age + 1);
        }
    }
}

mem::Allocator alloc = mem::Allocator::new();
con::Vector v = con::Vector::new(&alloc,8);

for i64 i = 0,i<10,i+=1 {
    i64 var = i * (i + 1);
    v.push(&var);
}

alloc.print();
v.print();
v.delete();

alloc.delete();


mem::Allocator alloc = mem::Allocator::new();

// st::String buffer = st::String::new(&alloc);
// buffer.cstr("Hello + ")->int(69)->cstr(" + World!\n");
// buffer.write(1);
// alloc.print();
// buffer.delete();

ios::OStream stdout = ios::OStream::new(&alloc,1);
stdout.cstr("Hello ")->cstr("World: ")->int(69)->cstr("\n")->flush();
stdout.delete();
alloc.delete();




mem::Allocator alloc = mem::Allocator::new();
ios::OStream stdout = ios::OStream::new(&alloc,1);
stdout.cstr("-> Stdout avalible!\n")->flush();
ios::IStream stdin = ios::IStream::new(&alloc,0);
stdout.cstr("-> Stdin: Read: ")->flush();

i8* input = stdin.line();
stdout.cstr("-> Stdout: Found: \'")->cstr(input)->cstr("\'!\n")->flush();
alloc.free(input);
stdout.cstr("-> Stdout closed!\n")->flush();
stdout.delete();
alloc.print();
alloc.delete();



import "../lib/Memory.rex";
import "../lib/Stream.rex";

i64 fib(i64 n){
    if n <= 1 {
        return n;
    }
    return fib(n - 1) + fib(n - 2);
}

i64 main(){
    mem::Allocator alloc = mem::Allocator::new();
    ios::OStream stdout = ios::OStream::new(&alloc,1);
    
    for i64 i = 0,i<20,i+=1 {
        i64 num = fib(i);
        stdout.cstr("Fib(")->int(i)->cstr(") = ")->int(num)->cstr("\n")->flush();
    }
    
    stdout.delete();
    alloc.print();
    alloc.delete();
    return 0;
}


i64 a = 1;
    bool c = a == 0 || a != 1 || a > 0 || a < 0 || a >= 0 || a <= 0;
    return (i64)c;


i64 sum = 0;
    for i64 i = 0,i<20,i+=1 {
        if i == 13 {
            continue;
        }

        sum += i;
    }

    return sum;