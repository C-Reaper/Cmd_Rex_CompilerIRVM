import "../lib/System.rex";
import "../lib/IO.rex";
import "../lib/Memory.rex";
import "../lib/Container.rex";

namespace ios {
    struct OStream[
        i64 file,
        u64 size,
        u64 memory_size,
        i8* memory,
        pub mem::Allocator* alloc
    ];

    impl OStream {
        pub void init(OStream* self,mem::Allocator* alloc,i64 file){
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
        pub OStream new(mem::Allocator* alloc,i64 file){
            OStream stream;
            stream.init(alloc,file);
            return stream;
        }
        pub OStream open(mem::Allocator* alloc,i8* path){
            return ios::OStream::new(alloc,sys::open(path,O_WRONLY));
        }

        pub void set_pos_start(OStream* self,u64 pos){
            sys::lseek(self->file,pos,SEEK_SET);
        }
        pub void set_pos_rel(OStream* self,u64 pos){
            sys::lseek(self->file,pos,SEEK_CUR);
        }
        pub void set_pos_end(OStream* self,u64 pos){
            sys::lseek(self->file,pos,SEEK_END);
        }

        pub i8 get(OStream* self,u64 i){
            if i>=self->size {
                return 0;
            }
            return self->memory[i];
        }
        pub void set_c(OStream* self,u64 i,i8* cstr,u64 count){
            if cstr != null && i>=0 && i+count<=self->size {
                ptr::cpy(self->memory + i,cstr,count);
            }
        }
        pub void expand(OStream* self,u64 count){
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
        pub void shrink(OStream* self,u64 count){
            u64 newsize = self->memory_size / 2;
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

        pub void push_c(OStream* self,i8* cstr,u64 count){
            self->expand(count);
            self->size+=count;
            self->set_c(self->size-count,cstr,count);
        }

        pub OStream* char(OStream* self,i8 ch){
            self->push_c(&ch,1);
            return self;
        }
        pub OStream* cstr(OStream* self,i8* ptr){
            u64 len = cstr::len(ptr);
            self->push_c(ptr,len);
            return self;
        }
        pub OStream* uint(OStream* self,u64 n){
            i8[48] block;
            ptr::set(block,0,48);
            parse::uint::get(block,n);
            u64 len = cstr::len(block);
            self->push_c(block,len);
            return self;
        }
        pub OStream* int(OStream* self,i64 n){
            i8[48] block;
            ptr::set(block,0,48);
            parse::int::get(block,n);
            u64 len = cstr::len(block);
            self->push_c(block,len);
            return self;
        }
        pub OStream* boolean(OStream* self,bool b){
            i8[8] block;
            ptr::set(block,0,8);
            parse::boolean::get(block,b);
            u64 len = cstr::len(block);
            self->push_c(block,len);
            return self;
        }
        pub OStream* float(OStream* self,f64 n,u64 postdigits){
            i8[48] block;
            ptr::set(block,0,48);
            parse::float::get(block,n,postdigits);
            u64 len = cstr::len(block);
            self->push_c(block,len);
            return self;
        }

        pub OStream* hex(OStream* self,i64 n){
            i8[48] block;
            ptr::set(block,0,48);
            parse::hex::get(block,n);
            u64 len = cstr::len(block);
            self->push_c(block,len);
            return self;
        }
        pub OStream* oct(OStream* self,i64 n){
            i8[48] block;
            ptr::set(block,0,48);
            parse::oct::get(block,n);
            u64 len = cstr::len(block);
            self->push_c(block,len);
            return self;
        }
        pub OStream* bin(OStream* self,i64 n){
            i8[48] block;
            ptr::set(block,0,48);
            parse::bin::get(block,n);
            u64 len = cstr::len(block);
            self->push_c(block,len);
            return self;
        }

        pub OStream* address(OStream* self,void* n){
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
        pub OStream* mem(OStream* self,void* n,u64 bytes){
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
        
        pub OStream* flush(OStream* self){
            u64 size = self->size;
            if size>0 {
                sys::write(self->file,self->memory,size);
                self->clear();
            }
            return self;
        }
    }

    struct IStream[
        i64 file,
        mem::Allocator* alloc
    ];

    impl IStream {
        pub void init(IStream* self,mem::Allocator* alloc,i64 file){
            self->file = file;
            self->alloc = alloc;
        }
        pub IStream new(mem::Allocator* alloc,i64 file){
            IStream stream;
            stream.init(alloc,file);
            return stream;
        }
        pub IStream open(mem::Allocator* alloc,i8* path){
            return ios::IStream::new(alloc,sys::open(path,O_RDONLY));
        }
        pub void delete(IStream* self){
            if self->file>2 {
                sys::close(self->file);
            }
            self->file = null;
        }

        pub u64 get_size(IStream* self){
            sys::Stat filestate;
            sys::fstat(self->file,&filestate);
            return filestate.st_size;
        }
        pub void set_pos_start(IStream* self,u64 pos){
            sys::lseek(self->file,pos,SEEK_SET);
        }
        pub void set_pos_rel(IStream* self,u64 pos){
            sys::lseek(self->file,pos,SEEK_CUR);
        }
        pub void set_pos_end(IStream* self,u64 pos){
            sys::lseek(self->file,pos,SEEK_END);
        }

        pub i8* line(IStream* self){
            con::Vector line = con::Vector::new(self->alloc,1);
            i8[16] buffer;
        
            bool searching = true;
            while searching {
                ptr::set(buffer,0,16);
                i64 bytes_read = sys::read(self->file,buffer,15);
        
                line.push_c(buffer,cstr::len(buffer));
                
                if bytes_read<15 || buffer[14]==10 {
                    searching = false;
                }
            }
        
            u64 size = line.get_size();
            i8* out = self->alloc->alloc(size + 1);
            ptr::cpy(out,line.get_data(),size);
            out[size] = 0;

            line.delete();
            return out;
        }

        pub i8 char(IStream* self){
            i8* line = self->line();
            i8 ch = line[0];
            self->alloc->free(line);
            return ch;
        }
        pub void buff(IStream* self,i8* cstr,u64 size){
            i8* line = self->line();
            u64 max = cstr::len(line);
            if max<size {
                ptr::cpy(cstr,line,max);
            }else{
                ptr::cpy(cstr,line,size);
            }
            self->alloc->free(line);
        }
        pub i8* cstr(IStream* self,u64 size){
            i8* line = self->line();
            return line;
        }
        pub u64 uint(IStream* self){
            i8* line = self->line();
            u64 size = cstr::len(line)-1;
            line[size] = 0;

            i32 i = cstr::findnot(line,"0123456789");
            u64 num = parse::uint::by(line);
            self->alloc->free(line);
            return num;
        }
        pub i64 int(IStream* self){
            i8* line = self->line();
            u64 size = cstr::len(line)-1;
            line[size] = 0;
        
            i32 i = cstr::findnot(line,"-0123456789");
            i64 num = parse::int::by(line);
            self->alloc->free(line);
            return num;
        }
        pub bool boolean(IStream* self){
            i8* line = self->line();
            u64 size = cstr::len(line)-1;
            line[size] = 0;

            bool b = parse::boolean::by(line);
            self->alloc->free(line);
            return b;
        }
        pub f64 float(IStream* self){
            i8* line = self->line();
            u64 size = cstr::len(line)-1;
            line[size] = 0;
        
            i32 i = cstr::findnot(line,"-.0123456789");
            f64 num = parse::float::by(line);
            self->alloc->free(line);
            return num;
        }
    
        pub u64 hex(IStream* self){
            i8* line = self->line();
            u64 size = cstr::len(line)-1;
            line[size] = 0;
        
            i32 i = cstr::findnot(line,"x0123456789ABCDEFabcdef");
            u64 num = parse::hex::by(line);
            self->alloc->free(line);
            return num;
        }
        pub u64 oct(IStream* self){
            i8* line = self->line();
            u64 size = cstr::len(line)-1;
            line[size] = 0;
        
            i32 i = cstr::findnot(line,"o01234567");
            u64 num = parse::oct::by(line);
            self->alloc->free(line);
            return num;
        }
        pub u64 bin(IStream* self){
            i8* line = self->line();
            u64 size = cstr::len(line)-1;
            line[size] = 0;
        
            i32 i = cstr::findnot(line,"b01");
            u64 num = parse::bin::by(line);
            self->alloc->free(line);
            return num;
        }


        pub bool uint_s(IStream* self,u64* ptr){
            i8* line = self->line();
            u64 size = cstr::len(line)-1;
            line[size] = 0;

            bool ret = parse::uint::by_s(line,ptr);
            self->alloc->free(line);
            return ret;
        }
        pub bool int_s(IStream* self,i64* ptr){
            i8* line = self->line();
            u64 size = cstr::len(line)-1;
            line[size] = 0;
        
            bool ret = parse::int::by_s(line,ptr);
            self->alloc->free(line);
            return ret;
        }
        pub bool boolean_s(IStream* self,bool* ptr){
            i8* line = self->line();
            u64 size = cstr::len(line)-1;
            line[size] = 0;

            bool ret = parse::boolean::by_s(line,ptr);
            self->alloc->free(line);
            return ret;
        }
        pub bool float_s(IStream* self,f64* ptr){
            i8* line = self->line();
            u64 size = cstr::len(line)-1;
            line[size] = 0;
        
            bool ret = parse::float::by_s(line,ptr);
            self->alloc->free(line);
            return ret;
        }
    
        pub bool hex_s(IStream* self,u64* ptr){
            i8* line = self->line();
            u64 size = cstr::len(line)-1;
            line[size] = 0;
        
            bool ret = parse::hex::by_s(line,ptr);
            self->alloc->free(line);
            return ret;
        }
        pub bool oct_s(IStream* self,u64* ptr){
            i8* line = self->line();
            u64 size = cstr::len(line)-1;
            line[size] = 0;
        
            bool ret = parse::oct::by_s(line,ptr);
            self->alloc->free(line);
            return ret;
        }
        pub bool bin_s(IStream* self,u64* ptr){
            i8* line = self->line();
            u64 size = cstr::len(line)-1;
            line[size] = 0;
        
            bool ret = parse::bin::by_s(line,ptr);
            self->alloc->free(line);
            return ret;
        }
    }
}
