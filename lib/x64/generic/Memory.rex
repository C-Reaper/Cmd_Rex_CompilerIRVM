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
            if self == null || size < 8 || size != (u32)size {
                if self != null {
                    self->memory = null;
                    self->length = 0;
                    self->flags = 0;
                }
                return;
            }

            if block == null {
                self->memory = sys::alloc(size);
                self->flags = 0;
            }else{
                self->memory = block;
                self->flags = 1;
            }

            if self->memory == null {
                self->length = 0;
                return;
            }

            self->length = size;
            ptr::set(self->memory,0,size);

            Header* first = self->memory;
            first->length = size - 8;
            first->flags = 0;
        }
        pub void delete(Allocator* self){
            if self->memory != null && self->flags == 0 {
                sys::free(self->memory,self->length);
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
            if self == null || self->memory == null || size == 0 {
                return null;
            }

            Header* first = self->memory;
            void* end = self->memory + self->length;

            while first < end {
                if first + 8 > end {
                    return null;
                }

                Header* next = first + 8 + (u64)first->length;
                if next > end {
                    return null;
                }

                if first->flags == 0 && first->length >= size {
                    u64 remaining = (u64)first->length - size;
                    u64 used = size;
                    if remaining <= 8 {
                        used = first->length;
                    }

                    first->flags = 1;
                    first->length = used;
                    if remaining > 8 {
                        Header* remainder = first + 8 + used;
                        remainder->length = remaining - 8;
                        remainder->flags = 0;
                    }
                    return first + 8;
                }

                first = next;
            }
            return null;
        }
        pub void free(Allocator* self,void* addr){
            if self == null || self->memory == null || addr == null {
                return;
            }
            void* end = self->memory + self->length;
            if addr < self->memory || addr >= end {
                return;
            }

            Header* prev = null;
            Header* first = self->memory;

            while first < end {
                if first + 8 > end {
                    return;
                }

                Header* next = first + 8 + (u64)first->length;
                if next > end {
                    return;
                }

                if addr == first + 8 {
                    if first->flags == 0 {
                        return;
                    }
                    first->flags = 0;

                    if next + 8 <= end {
                        if next->flags==0 {
                            first->length += 8 + next->length;
                        }
                    }

                    if prev != null {
                        if prev->flags==0 {
                            prev->length += 8 + first->length;
                        }
                    }
                    return;
                }

                prev = first;
                first = next;
            }
        }
        
        pub void* realloc(Allocator* self,void* addr,u64 size){
            if self == null || self->memory == null {
                return null;
            }

            if addr == null {
                return self->alloc(size);
            }
            if size == 0 {
                self->free(addr);
                return null;
            }

            void* end = self->memory + self->length;
            if addr < self->memory || addr >= end {
                return null;
            }

            Header* first = self->memory;

            while first < end {
                if first + 8 > end {
                    return null;
                }

                Header* next = first + 8 + (u64)first->length;
                if next > end {
                    return null;
                }

                if addr == first + 8 {
                    if first->length >= size {
                        u64 remaining = (u64)first->length - size;
                        if remaining > 8 {
                            first->length = size;
                            Header* remainder = first + 8 + size;
                            remainder->flags = 0;
                            remainder->length = remaining - 8;
                            Header* after_remainder = remainder + 8 + (u64)remainder->length;
                            if after_remainder + 8 <= end {
                                if after_remainder->flags == 0 {
                                    remainder->length += 8 + after_remainder->length;
                                }
                            }
                        }
                        return addr;
                    }

                    if next + 8 <= end {
                        if next->flags == 0 {
                            u64 available = (u64)first->length + 8 + (u64)next->length;
                            if available >= size {
                                u64 remaining = available - size;
                                if remaining > 8 {
                                    first->length = size;
                                    Header* remainder = first + 8 + size;
                                    remainder->flags = 0;
                                    remainder->length = remaining - 8;
                                    Header* after_remainder = remainder + 8 + (u64)remainder->length;
                                    if after_remainder + 8 <= end {
                                        if after_remainder->flags == 0 {
                                            remainder->length += 8 + after_remainder->length;
                                        }
                                    }
                                } else {
                                    first->length = available;
                                }
                                return addr;
                            }
                        }
                    }

                    void* newptr = self->alloc(size);
                    if newptr == null {
                        return null;
                    }
                    ptr::cpy(newptr,addr,first->length);
                    self->free(addr);
                    return newptr;
                }

                first = next;
            }
            return null;
        }
        pub void* calloc(Allocator* self,u64 count,u64 elementsize){
            u64 size = count * elementsize;
            if count != 0 && size / count != elementsize {
                return null;
            }
            void* addr = self->alloc(size);
            if addr != null {
                ptr::set(addr,0,size);
            }
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
