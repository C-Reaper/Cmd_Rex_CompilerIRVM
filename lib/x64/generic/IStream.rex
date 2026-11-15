import System;
import IO;
import Memory;
import Vector;

namespace ios {
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
        //pub IStream open(mem::Allocator* alloc,i8* path){
        //    return ios::IStream::new(alloc,sys::open(path,O_RDONLY));
        //}
        pub void delete(IStream* self){
            if self->file > 2 {
                if self->file != (i64)stdin() {
                    if self->file != (i64)stdout() {
                        if self->file != (i64)stderr() {
                            sys::close(self->file);
                        }
                    }
                }
            }
            self->file = -1;
        }

        u64 trim_line(IStream* self,i8* line){
            u64 size = cstr::len(line);
            if size > 0 && line[size - 1] == 10 {
                size -= 1;
            }
            if size > 0 && line[size - 1] == 13 {
                size -= 1;
            }
            line[size] = 0;
            return size;
        }

        //pub u64 get_size(IStream* self){
        //    sys::Stat filestate;
        //    sys::fstat(self->file,&filestate);
        //    return filestate.st_size;
        //}
        //pub void set_pos_start(IStream* self,u64 pos){
        //    sys::lseek(self->file,pos,SEEK_SET);
        //}
        //pub void set_pos_rel(IStream* self,u64 pos){
        //    sys::lseek(self->file,pos,SEEK_CUR);
        //}
        //pub void set_pos_end(IStream* self,u64 pos){
        //    sys::lseek(self->file,pos,SEEK_END);
        //}

        pub i8* line(IStream* self){
            i8[1] buffer;
            con::Vector vline = con::Vector::new(self->alloc,1);
            u64 reading = 1;

            while reading {
                u64 bytes_read = sys::read(self->file,buffer,1);
                if bytes_read >> 63 {
                    reading = 0;
                } elif bytes_read == 0 {
                    reading = 0;
                } else {
                    vline.push_c(buffer,1);
                    if buffer[0] == 10 {
                        reading = 0;
                    }
                }
            }

            u64 size = vline.get_size();
            i8* out = self->alloc->alloc(size + 1);
            ptr::cpy(out,vline.get_data(),size);
            out[size] = 0;

            vline.delete();
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
            u64 size = self->trim_line(line);
            if size == 0 {
                self->alloc->free(line);
                return 0;
            }

            u64 num = parse::uint::by(line);
            self->alloc->free(line);
            return num;
        }
        pub i64 int(IStream* self){
            i8* line = self->line();
            u64 size = self->trim_line(line);
            if size == 0 {
                self->alloc->free(line);
                return 0;
            }

            i64 num = parse::int::by(line);
            self->alloc->free(line);
            return num;
        }
        pub bool boolean(IStream* self){
            i8* line = self->line();
            u64 size = self->trim_line(line);
            if size == 0 {
                self->alloc->free(line);
                return false;
            }

            bool b = parse::boolean::by(line);
            self->alloc->free(line);
            return b;
        }
        //pub f64 float(IStream* self){
        //    i8* line = self->line();
        //    u64 size = cstr::len(line)-1;
        //    line[size] = 0;
        //
        //    i32 i = cstr::findnot(line,"-.0123456789");
        //    f64 num = parse::float::by(line);
        //    self->alloc->free(line);
        //    return num;
        //}
    
        pub u64 hex(IStream* self){
            i8* line = self->line();
            u64 size = self->trim_line(line);
            if size == 0 {
                self->alloc->free(line);
                return 0;
            }

            u64 num = parse::hex::by(line);
            self->alloc->free(line);
            return num;
        }
        pub u64 oct(IStream* self){
            i8* line = self->line();
            u64 size = self->trim_line(line);
            if size == 0 {
                self->alloc->free(line);
                return 0;
            }

            u64 num = parse::oct::by(line);
            self->alloc->free(line);
            return num;
        }
        pub u64 bin(IStream* self){
            i8* line = self->line();
            u64 size = self->trim_line(line);
            if size == 0 {
                self->alloc->free(line);
                return 0;
            }

            u64 num = parse::bin::by(line);
            self->alloc->free(line);
            return num;
        }


        pub bool uint_s(IStream* self,u64* ptr){
            i8* line = self->line();
            u64 size = self->trim_line(line);
            if size == 0 {
                self->alloc->free(line);
                return false;
            }

            bool ret = parse::uint::by_s(line,ptr);
            self->alloc->free(line);
            return ret;
        }
        pub bool int_s(IStream* self,i64* ptr){
            i8* line = self->line();
            u64 size = self->trim_line(line);
            if size == 0 {
                self->alloc->free(line);
                return false;
            }

            bool ret = parse::int::by_s(line,ptr);
            self->alloc->free(line);
            return ret;
        }
        pub bool boolean_s(IStream* self,bool* ptr){
            i8* line = self->line();
            u64 size = self->trim_line(line);
            if size == 0 {
                self->alloc->free(line);
                return false;
            }

            bool ret = parse::boolean::by_s(line,ptr);
            self->alloc->free(line);
            return ret;
        }
        //pub bool float_s(IStream* self,f64* ptr){
        //    i8* line = self->line();
        //    u64 size = cstr::len(line)-1;
        //    line[size] = 0;
        //
        //    bool ret = parse::float::by_s(line,ptr);
        //    self->alloc->free(line);
        //    return ret;
        //}
    
        pub bool hex_s(IStream* self,u64* ptr){
            i8* line = self->line();
            u64 size = self->trim_line(line);
            if size == 0 {
                self->alloc->free(line);
                return false;
            }

            bool ret = parse::hex::by_s(line,ptr);
            self->alloc->free(line);
            return ret;
        }
        pub bool oct_s(IStream* self,u64* ptr){
            i8* line = self->line();
            u64 size = self->trim_line(line);
            if size == 0 {
                self->alloc->free(line);
                return false;
            }

            bool ret = parse::oct::by_s(line,ptr);
            self->alloc->free(line);
            return ret;
        }
        pub bool bin_s(IStream* self,u64* ptr){
            i8* line = self->line();
            u64 size = self->trim_line(line);
            if size == 0 {
                self->alloc->free(line);
                return false;
            }

            bool ret = parse::bin::by_s(line,ptr);
            self->alloc->free(line);
            return ret;
        }
    }
}
