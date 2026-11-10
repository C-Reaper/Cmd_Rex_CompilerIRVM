import "../lib/Memory.rex";

namespace st {
    struct String[
        u64 size,
        u64 memory_size,
        i8* memory,
        pub mem::Allocator* alloc
    ];

    impl String {
        pub void init(String* self,mem::Allocator* alloc){
            self->alloc = alloc;
            self->size = 0;
            self->memory_size = 5;
            self->memory = alloc->alloc(5);
        }
        pub void delete(String* self){
            void* mem = self->memory;
            self->alloc->free(mem);
            self->alloc = null;
            self->size = 0;
            self->memory_size = 0;
            self->memory = null;
        }
        pub String new(mem::Allocator* alloc){
            String vec;
            vec.init(alloc);
            return vec;
        }
        pub void write(String* self,i64 file){
            sys::write(file,self->get_data(),self->get_size());
        }

        pub i8 get(String* self,u64 i){
            if i>=self->size {
                return 0;
            }
            return self->memory[i];
        }
        pub void set_c(String* self,u64 i,i8* cstr,u64 count){
            if cstr != null && i>=0 && i+count<=self->size {
                ptr::cpy(self->memory + i,cstr,count);
            }
        }
        pub void expand(String* self,u64 count){
            u64 length = self->size + count;
            if length > self->memory_size {
                u64 newsize = self->memory_size;
                while length>newsize {
                    newsize *= 2;
                }
                void* block = self->alloc->alloc(newsize);
                ptr::cpy(block,self->memory,self->size);

                self->alloc->free(self->memory);
                self->memory = block;
                self->memory_size = newsize;
            }
        }
        pub void shrink(String* self,u64 count){
            u64 newsize = self->memory_size / 2;
            if self->size <= newsize && newsize>5 {
                void* block = self->alloc->alloc(newsize);
                ptr::cpy(block,self->memory,self->size);

                self->alloc->free(self->memory);
                self->memory = block;
                self->memory_size = newsize;
            }
        }
        pub void tozero(String* self){
            ptr::set(self->memory,0,self->size);
        }
        pub void clear(String* self){
            void* block = self->alloc->alloc(5);
            self->alloc->free(self->memory);
            self->memory = block;
            self->size = 0;
            self->memory_size = 5;
        }
        pub i8* get_data(String* self){
            return self->memory;
        }
        pub u64 get_size(String* self){
            return self->size;
        }

        pub void push_c(String* self,i8* cstr,u64 count){
            self->expand(count);
            self->size+=count;
            self->set_c(self->size-count,cstr,count);
        }
        pub void pop_c(String* self,u64 count){
            if self->size<count {
                count = self->size;
            }
            self->size-=count;
            self->shrink(count);
        }

        pub i8* cpy(String* self){
            u64 size = self->get_size();
            i8* out = self->alloc->alloc(size + 1);
            ptr::cpy(out,self->get_data(),size);
            out[size] = 0;
            return out;
        }

        pub String* string(String* self,String* ptr){
            self->push_c(ptr->get_data(),ptr->get_size());
            return self;
        }
        pub String* char(String* self,i8 ch){
            self->push_c(&ch,1);
            return self;
        }
        pub String* cstr(String* self,i8* ptr){
            u64 len = cstr::len(ptr);
            self->push_c(ptr,len);
            return self;
        }
        pub String* uint(String* self,u64 n){
            i8[48] block;
            ptr::set(block,0,48);
            parse::uint::get(block,n);
            u64 len = cstr::len(block);
            self->push_c(block,len);
            return self;
        }
        pub String* int(String* self,i64 n){
            i8[48] block;
            ptr::set(block,0,48);
            parse::int::get(block,n);
            u64 len = cstr::len(block);
            self->push_c(block,len);
            return self;
        }
        pub String* boolean(String* self,bool b){
            i8[8] block;
            ptr::set(block,0,8);
            parse::boolean::get(block,b);
            u64 len = cstr::len(block);
            self->push_c(block,len);
            return self;
        }
        pub String* float(String* self,f64 n,u64 postdigits){
            i8[48] block;
            ptr::set(block,0,48);
            parse::float::get(block,n,postdigits);
            u64 len = cstr::len(block);
            self->push_c(block,len);
            return self;
        }

        pub String* hex(String* self,i64 n){
            i8[48] block;
            ptr::set(block,0,48);
            parse::hex::get(block,n);
            u64 len = cstr::len(block);
            self->push_c(block,len);
            return self;
        }
        pub String* oct(String* self,i64 n){
            i8[48] block;
            ptr::set(block,0,48);
            parse::oct::get(block,n);
            u64 len = cstr::len(block);
            self->push_c(block,len);
            return self;
        }
        pub String* bin(String* self,i64 n){
            i8[48] block;
            ptr::set(block,0,48);
            parse::bin::get(block,n);
            u64 len = cstr::len(block);
            self->push_c(block,len);
            return self;
        }

        pub String* address(String* self,void* n){
            i8[48] block;
            ptr::set(block,0,48);
            
            void** ptr = &n;
            for i32 j = 0,j<8,j+=1 {
                u8 ch = *((u8*)(ptr+j));
                *(block+j*2) = char::to::hex((u64)(ch & 0xF0) >> 8);
                *(block+j*2+1) = char::to::hex(ch & 0xF);
            }

            u64 len = cstr::len(block);
            self->push_c(block,len);
            return self;
        }
        pub String* mem(String* self,void* n,u64 bytes){
            u64 count = bytes * 2;
            i8* block = self->alloc->alloc(count+1);
            
            ptr::set(block,0,count+1);
            
            for i32 j = 0,j<bytes,j+=1 {
                u8 ch = *((u8*)(n+j));
                *(block+j*2) = char::to::hex((u64)(ch & 0xF0) >> 8);
                *(block+j*2+1) = char::to::hex(ch & 0xF);
            }

            self->push_c(block,count);
            self->alloc->free(block);
            return self;
        }
    }
}