import IO;
import Parser;
import Pointer;
import Memory;

namespace con {
    struct Vector[
        u64 elementsize,
        u64 size,
        u64 memory_size,
        void* memory,
        pub mem::Allocator* alloc
    ];

    impl Vector{
        pub void init(Vector* self,mem::Allocator* alloc,u64 elementsize,u64 size){
            self->alloc = alloc;
            self->elementsize = elementsize;
            self->size = 0;
            self->memory_size = size;
            self->memory = alloc->alloc(size * elementsize);
        }
        pub void delete(Vector* self){
            void* mem = self->memory;
            self->alloc->free(mem);
            self->alloc = null;
            self->elementsize = 0;
            self->size = 0;
            self->memory_size = 0;
            self->memory = null;
        }
        pub Vector new(mem::Allocator* alloc,u64 elementsize){
            Vector vec;
            vec.init(alloc,elementsize,5);
            return vec;
        }
        pub Vector make(mem::Allocator* alloc,u64 elementsize,u64 size){
            Vector vec;
            vec.init(alloc,elementsize,size);
            return vec;
        }

        pub void* get(Vector* self,u64 i){
            if i<0 || i>=self->size {
                return null;
            }
            return self->memory + i * self->elementsize;
        }
        pub void set(Vector* self,u64 i,void* element){
            if i>=0 && i<self->size {
                ptr::cpy(self->memory + i * self->elementsize,element,self->elementsize);
            }
        }
        pub void set_c(Vector* self,u64 i,void* elements,u64 count){
            if elements != null && i>=0 && i+count<=self->size {
                ptr::cpy(self->memory + i * self->elementsize,elements,self->elementsize * count);
            }
        }
        pub void expand(Vector* self,u64 count){
            u64 length = self->size + count;
            if length > self->memory_size {
                u64 newsize = self->memory_size;
                while length>newsize {
                    newsize *= 2;
                }

                u64 elementsize = self->elementsize;
                void* block = self->alloc->alloc(newsize * elementsize);
                ptr::cpy(block,self->memory,self->size * elementsize);
                
                self->alloc->free(self->memory);
                self->memory = block;
                self->memory_size = newsize;
            }
        }
        pub void shrink(Vector* self,u64 count){
            u64 newsize = self->memory_size / 2;
            if self->size <= newsize && newsize>5 {
                u64 elementsize = self->elementsize;
                void* block = self->alloc->alloc(newsize * elementsize);
                ptr::cpy(block,self->memory,self->size * elementsize);
                
                self->alloc->free(self->memory);
                self->memory = block;
                self->memory_size = newsize;
            }
        }

        pub void tozero(Vector* self){
            ptr::set(self->memory,0,self->size * self->elementsize);
        }
        pub void clear(Vector* self){
            u64 elementsize = self->elementsize;
            void* block = self->alloc->alloc(5 * elementsize);
            self->alloc->free(self->memory);
            self->memory = block;
            self->size = 0;
            self->memory_size = 5;
        }

        pub void* get_data(Vector* self){
            return self->memory;
        }
        pub u64 get_size(Vector* self){
            return self->size;
        }
        
        pub void push(Vector* self,void* element){
            self->expand(1);
            self->size+=1;
            self->set(self->size-1,element);
        }
        pub void pop(Vector* self){
            self->size-=1;
            self->shrink(1);
        }
        
        pub void push_c(Vector* self,void* elements,u64 count){
            self->expand(count);
            self->size+=count;
            self->set_c(self->size-count,elements,count);
        }
        pub void pop_c(Vector* self,u64 count){
            u64 rcount = count;
            if self->size<count {
                rcount = self->size;
            }
            self->size -= rcount;
            self->shrink(rcount);
        }

        pub void print(Vector* self){
            io::print("------------------- Vector -------------------\n");
            i8[50] cstr;
            ptr::set(cstr,32,49);
            ptr::cpy(cstr+48,"\n",2);
            ptr::cpy(cstr,"| Header: ",10);
            parse::hex::get(cstr+12,(u64)self->memory);
            parse::uint::get(cstr+30,(u64)self->elementsize);
            parse::uint::get(cstr+36,(u64)self->memory_size);
            parse::uint::get(cstr+42,(u64)self->size);
            io::print(cstr);

            for u64 i = 0,i<self->size,i+=1 {
                void* element = self->get(i);
                u64 visiual = 0;
                
                ptr::set(cstr,32,49);
                ptr::cpy(cstr,"| Element: ",11);
                parse::uint::get(cstr+14,(u64)i);
                
                if self->elementsize==1 {
                    visiual = *((u8*)element);
                    cstr[18] = visiual;
                }elif self->elementsize==2 {
                    visiual = (u16)(*((u16*)element));
                    parse::uint::get(cstr+18,visiual);
                }elif self->elementsize==4 {
                    visiual = (u32)(*((u32*)element));
                    parse::uint::get(cstr+18,visiual);
                }else {
                    visiual = (u64)(*((u64*)element));
                    parse::uint::get(cstr+18,visiual);
                }
                
                ptr::cpy(cstr+48,"\n",2);
                io::print(cstr);
            }
            io::print("----------------------------------------------\n");
        }
    }
}



