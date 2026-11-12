import System;
import IO;
import Memory;

namespace ios {
    struct OStream[
        i32 file,
        u32 size,
        u32 memory_size,
        i8* memory,
        pub mem::Allocator* alloc
    ];

    impl OStream {
        pub void init(OStream* self,mem::Allocator* alloc,i32 file){
            self->file = file;
            self->alloc = alloc;
            self->size = 0;
            self->memory_size = 5;
            self->memory = alloc->alloc(5);
        }
        pub void delete(OStream* self){
            if self->file > 2 {
                sys::close(self->file);
            }

            void* mem = self->memory;
            self->alloc->free(mem);
            self->alloc = null;
            self->size = 0;
            self->memory_size = 0;
            self->memory = null;
        }
        pub OStream new(mem::Allocator* alloc,i32 file){
            OStream stream;
            stream.init(alloc,file);
            return stream;
        }
        //pub OStream open(mem::Allocator* alloc,i8* path){
        //    return ios::OStream::new(alloc,sys::open(path,O_WRONLY));
        //}
        //pub void set_pos_start(OStream* self,u32 pos){
        //    sys::lseek(self->file,pos,SEEK_SET);
        //}
        //pub void set_pos_rel(OStream* self,u32 pos){
        //    sys::lseek(self->file,pos,SEEK_CUR);
        //}
        //pub void set_pos_end(OStream* self,u32 pos){
        //    sys::lseek(self->file,pos,SEEK_END);
        //}

        pub i8 get(OStream* self,u32 i){
            if i>=self->size {
                return 0;
            }
            return self->memory[i];
        }
        pub void set_c(OStream* self,u32 i,i8* cstr,u32 count){
            if cstr != null && i>=0 && i+count<=self->size {
                ptr::cpy(self->memory + i,cstr,count);
            }
        }
        pub void expand(OStream* self,u32 count){
            u32 length = self->size + count;
            if length > self->memory_size {
                u32 newsize = self->memory_size;
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
        pub void shrink(OStream* self,u32 count){
            u32 newsize = self->memory_size / 2;
            if self->size <= newsize && newsize>5 {
                void* block = self->alloc->alloc(newsize);
                ptr::cpy(block,self->memory,self->size);

                self->alloc->free(self->memory);
                self->memory = block;
                self->memory_size = newsize;
            }
        }
        pub void clear(OStream* self){
            void* block = self->alloc->alloc(5);
            self->alloc->free(self->memory);
            self->memory = block;
            self->size = 0;
            self->memory_size = 5;
        }

        pub void push_c(OStream* self,i8* cstr,u32 count){
            self->expand(count);
            self->size += count;
            self->set_c(self->size - count,cstr,count);
        }

        pub OStream* char(OStream* self,i8 ch){
            i8 st_ch = ch;
            self->push_c(&st_ch,1);
            return self;
        }
        pub OStream* cstr(OStream* self,i8* ptr){
            u32 len = cstr::len(ptr);
            self->push_c(ptr,len);
            return self;
        }
        pub OStream* uint(OStream* self,u32 n){
            i8[48] block;
            ptr::set(block,0,48);
            parse::uint::get(block,n);
            u32 len = cstr::len(block);
            self->push_c(block,len);
            return self;
        }
        pub OStream* int(OStream* self,i32 n){
            i8[48] block;
            ptr::set(block,0,48);
            parse::int::get(block,n);
            u32 len = cstr::len(block);
            self->push_c(block,len);
            return self;
        }
        pub OStream* boolean(OStream* self,bool b){
            i8[8] block;
            ptr::set(block,0,8);
            parse::boolean::get(block,b);
            u32 len = cstr::len(block);
            self->push_c(block,len);
            return self;
        }
        //pub OStream* float(OStream* self,f64 n,u32 postdigits){
        //    i8[48] block;
        //    ptr::set(block,0,48);
        //    parse::float::get(block,n,postdigits);
        //    u32 len = cstr::len(block);
        //    self->push_c(block,len);
        //    return self;
        //}

        pub OStream* hex(OStream* self,i32 n){
            i8[48] block;
            ptr::set(block,0,48);
            parse::hex::get(block,n);
            u32 len = cstr::len(block);
            self->push_c(block,len);
            return self;
        }
        pub OStream* oct(OStream* self,i32 n){
            i8[48] block;
            ptr::set(block,0,48);
            parse::oct::get(block,n);
            u32 len = cstr::len(block);
            self->push_c(block,len);
            return self;
        }
        pub OStream* bin(OStream* self,i32 n){
            i8[48] block;
            ptr::set(block,0,48);
            parse::bin::get(block,n);
            u32 len = cstr::len(block);
            self->push_c(block,len);
            return self;
        }

        pub OStream* address(OStream* self,void* n){
            i8[48] block;
            ptr::set(block,0,48);
            
            void* addr = n;
            void** ptr = &addr;
            
            for i32 j = 0,j<8,j += 1 {
                u8 ch = *((u8*)(ptr+j));
                *(block+j*2) = char::to::hex((u32)(ch & 0xF0) >> 8);
                *(block+j*2+1) = char::to::hex(ch & 0xF);
            }

            u32 len = cstr::len(block);
            self->push_c(block,len);
            return self;
        }
        pub OStream* mem(OStream* self,void* n,u32 bytes){
            u32 count = bytes * 2;
            i8* block = self->alloc->alloc(count+1);
            
            ptr::set(block,0,count+1);
            
            for i32 j = 0,j<bytes,j+=1 {
                u8 ch = *((u8*)(n+j));
                *(block+j*2) = char::to::hex((u32)(ch & 0xF0) >> 8);
                *(block+j*2+1) = char::to::hex(ch & 0xF);
            }

            self->push_c(block,count);
            self->alloc->free(block);
            return self;
        }
        
        pub OStream* flush(OStream* self){
            u32 size = self->size;
            if size>0 {
                sys::write(self->file,self->memory,size);
                self->clear();
            }
            return self;
        }
    }
}
