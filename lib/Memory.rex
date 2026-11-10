import "../lib/System.rex";
import "../lib/IO.rex";
import "../lib/Pointer.rex";
import "../lib/Parser.rex";

namespace mem {
    struct Header[
        pub u32 length,
        pub u32 flags
    ];

    struct Allocator[
        void* memory,
        u64 length
    ];

    impl Allocator{
        pub void init(Allocator* self,u64 size){
            self->memory = sys::mmap(size);
            self->length = size;
            ptr::set(self->memory,0,size);

            Header* first = self->memory;
            first->length = size - 8;
            first->flags = 0;
        }
        pub void delete(Allocator* self){
            if self->memory != 0 {
                sys::munmap(self->memory,self->length);
            }
            self->memory = 0;
            self->length = 0;
        }
        
        pub Allocator new(){
            Allocator alloc;
            alloc.init(10000);
            return alloc;
        }
        pub Allocator make(u64 size){
            Allocator alloc;
            alloc.init(size);
            return alloc;
        }

        pub void* alloc(Allocator* self,u64 size){
            void* ptr = null;
            Header* first = self->memory;
            
            while true {
                if first+8+size > self->memory+self->length {
                    return null;
                }elif first->flags==0 && first->length>=size {
                    u64 rem = first->length - size;
                    if rem<=8 {
                        size = first->length;
                    }
                    
                    first->flags = 1;
                    first->length = size;
                    ptr = first + 8;

                    Header* next = ptr + size;
                    if rem>8 && next+rem<=self->memory+self->length {
                        next->length = rem - 8;
                        next->flags = 0;
                    }

                    return ptr;
                }else{
                    first = first + 8 + (u64)first->length;
                }
            }
            return ptr;
        }
        pub void free(Allocator* self,void* ptr){
            if self == null || ptr == null {
                return;
            }
            if ptr<self->memory || ptr>self->memory+self->length {
                return;
            }
            
            Header* prev = null;
            Header* first = self->memory;
            
            while true {
                if first + 8 + (u64)first->length > self->memory+self->length {
                    return;
                }elif ptr >= first && ptr < first + 8 + (u64)first->length {
                    first->flags = 0;
                    
                    Header* next = first + 8 + (u64)first->length;
                    if next<=self->memory + self->length {
                        if next->flags==0 {
                            first->length += 8 + next->length;
                        }
                    }

                    if prev>=self->memory {
                        if prev->flags==0 {
                            prev->length += 8 + first->length;
                        }
                    }
                    return;
                }else{
                    prev = first;
                    first = first + 8 + (u64)first->length;
                }
            }
        }
        
        pub void* realloc(Allocator* self,void* ptr,u64 size){
            if ptr<self->memory || ptr>self->memory+self->length {
                return null;
            }
            
            Header* first = self->memory;
            
            while true {
                if first + 8 + (u64)first->length > self->memory+self->length {
                    return null;
                }elif ptr >= first && ptr < first+8+first->length {
                    Header* next = first + 8 + (u64)first->length;
                    
                    if first->length>size {
                        u64 diff = (u64)first->length - size - 8;
                        if diff>0 {
                            first->length = size;
                            Header* renext = first + 8 + (u64)first->length;
                            renext->flags = 0;
                            renext->length = diff;
                        }
                        return ptr;
                    }elif next->flags==0 {
                        if first->length + 8 + next->length>size {
                            u64 diff = (u64)first->length + (u64)next->length - size;// + 8 & - 8 => 0
                            if diff>0 {
                                first->length = size;
                                Header* renext = first + 8 + (u64)first->length;
                                renext->flags = 0;
                                renext->length = diff;
                            }else{
                                first->length += 8 + next->length;
                            }
                            return ptr;
                        }
                    }

                    void* newptr = self->alloc(size);
                    ptr::cpy(newptr,ptr,first->length);
                    self->free(ptr);
                    return newptr;
                }else{
                    first = first + 8 + (u64)first->length;
                }
            }
            return ptr;
        }
        pub void* calloc(Allocator* self,u64 count,u64 elementsize){
            void* ptr = self->alloc(count * elementsize);
            ptr::set(ptr,0,count * elementsize);
            return ptr;
        }

        pub void print(Allocator* self){
            io::print("---------------------- Allocator ---------------------\n");
            
            i8[128] cstr;
            
            ptr::set(cstr,32,128);
            ptr::cpy(cstr,"| Memory:",9);
            parse::hex::get(cstr+10,self->memory);
            
            cstr[53] = '|';
            ptr::cpy(cstr+125,"\n",2);
            io::print(cstr);

            ptr::set(cstr,32,128);
            ptr::cpy(cstr,"| Length:",9);
            parse::uint::get(cstr+10,self->length);
            
            cstr[53] = '|';
            ptr::cpy(cstr+125,"\n",2);
            io::print(cstr);

            Header* first = self->memory;
            while first >= self->memory && first < self->memory + self->length {
                ptr::set(cstr,32,128);
                ptr::cpy(cstr,"| Block:",8);
                
                parse::hex::get(cstr+9,first);
                parse::uint::get(cstr+28,(u64)first->length);
                parse::hex::get(cstr+45,(u64)first->flags);
                
                cstr[53] = '|';
                ptr::cpy(cstr+125,"\n",2);

                io::print(cstr);
            
                first = first + 8 + (u64)first->length;
            }

            io::print("------------------------------------------------------\n");
        }
    }
}
