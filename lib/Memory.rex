import System;
import IO;
import Pointer;
import Parser;

namespace mem {
    struct Header[
        pub u32 length,
        pub u32 flags
    ];

    struct Allocator[
        void* memory,
        u32 length,
        u32 flags
    ];

    impl Allocator{
        pub void init(Allocator* self,void* block,u64 size){
            if block == null {
                self->memory = sys::mmap(null,size,3,0x22,0,0);
                self->flags = 0;
            }else{
                self->memory = block;
                self->flags = 1;
            }

            self->length = size;
            ptr::set(self->memory,0,size);

            Header* first = self->memory;
            first->length = size - 8;
            first->flags = 0;
        }
        pub void delete(Allocator* self){
            if self->memory != null && self->flags == 0 {
                sys::munmap(self->memory,self->length);
            }
            self->memory = null;
            self->length = 0;
            self->flags = 0;
        }
        
        pub Allocator new(){
            Allocator alloc;
            alloc.init(null,10000);
            return alloc;
        }
        pub Allocator make(u64 size){
            Allocator alloc;
            alloc.init(null,size);
            return alloc;
        }
        pub Allocator by(void* block,u64 size){
            Allocator alloc;
            alloc.init(block,size);
            return alloc;
        }

        pub void* alloc(Allocator* self,u64 size){
            void* addr = null;
            Header* first = self->memory;
            
            while true {
                if first+8+size > self->memory+self->length {
                    return null;
                }elif first->flags==0 && first->length>=size {
                    u64 rem = first->length - size;
                    u64 rsize = size;
                    
                    if rem<=8 {
                        rsize = first->length;
                    }
                    
                    first->flags = 1;
                    first->length = rsize;
                    addr = first + 8;

                    Header* next = addr + rsize;
                    if rem>8 && next+rem<=self->memory+self->length {
                        next->length = rem - 8;
                        next->flags = 0;
                    }

                    return addr;
                }else{
                    first = first + 8 + (u64)first->length;
                }
            }
            return addr;
        }
        pub void free(Allocator* self,void* addr){
            if self == null || addr == null {
                return;
            }
            if addr<self->memory || addr>self->memory+self->length {
                return;
            }
            
            Header* prev = null;
            Header* first = self->memory;
            
            while true {
                if first + 8 + (u64)first->length > self->memory+self->length {
                    return;
                }elif addr >= first && addr < first + 8 + (u64)first->length {
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
        
        pub void* realloc(Allocator* self,void* addr,u64 size){
            if addr<self->memory || addr>self->memory+self->length {
                return null;
            }
            
            Header* first = self->memory;
            
            while true {
                if first + 8 + (u64)first->length > self->memory+self->length {
                    return null;
                }elif addr >= first && addr < first+8+first->length {
                    Header* next = first + 8 + (u64)first->length;
                    
                    if first->length>size {
                        u64 diff = (u64)first->length - size - 8;
                        if diff>0 {
                            first->length = size;
                            Header* renext = first + 8 + (u64)first->length;
                            renext->flags = 0;
                            renext->length = diff;
                        }
                        return addr;
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
                            return addr;
                        }
                    }

                    void* newptr = self->alloc(size);
                    ptr::cpy(newptr,addr,first->length);
                    self->free(addr);
                    return newptr;
                }else{
                    first = first + 8 + (u64)first->length;
                }
            }
            return addr;
        }
        pub void* calloc(Allocator* self,u64 count,u64 elementsize){
            void* addr = self->alloc(count * elementsize);
            ptr::set(addr,0,count * elementsize);
            return addr;
        }

        pub void print(Allocator* self){
            io::print("---------------------- Allocator ---------------------\n");
            
            i8[128] cstr;
            
            ptr::set(cstr,32,128);
            ptr::cpy(cstr,"| Memory:",9);
            parse::hex::get(cstr + 10,(u64)self->memory);
            
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
                
                parse::hex::get(cstr+9,(u64)first);
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
