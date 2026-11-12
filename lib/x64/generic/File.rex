

namespace fsys {
    struct File[
        pub u64 fd
    ];

    impl File{
        pub void init(File* f,u64 fd_in){
            f->fd = fd_in;
        }
        pub File new(u64 fd_in){
            File file;
            file.init(fd_in);
            return file;
        }
        pub File open(i8* filepath,u64 flags){
            u64 fd = sys::open(filepath,flags,0b110100100);
            return fsys::File::new(fd);
        }
        pub void close(File* self){
            if self->fd > 2 {
                sys::close(self->fd);
            }
            self->fd = 0;
        }
        pub i8 isopen(File* self){
            return self->fd < 10000000;
        }

        pub u64 size(File* self){
            sys::Stat filestate;
            sys::fstat(self->fd,&filestate);
            return filestate.st_size;
        }
        pub void set_pos_start(File* self,u64 pos){
            sys::lseek(self->fd,pos,SEEK_SET);
        }
        pub void set_pos_rel(File* self,u64 pos){
            sys::lseek(self->fd,pos,SEEK_CUR);
        }
        pub void set_pos_end(File* self,u64 pos){
            sys::lseek(self->fd,pos,SEEK_END);
        }
        pub i32 get_flags(File* self){
            return sys::fcntl(self->fd,3,0);
        }
        pub i32 set_flags(File* self,u64 flags){
            return sys::fcntl(self->fd,4,flags);
        }
        //pub u64 add_flags(File* self,u64 flags){
        //    u64 oldflags = self->get_flags();
        //    return sys::fcntl(self->fd,4,oldflags | flags);
        //}
        //pub u64 rm_flags(File* self,u64 flags){
        //    u64 oldflags = self->get_flags();
        //    return sys::fcntl(self->fd,4,oldflags & (~flags));
        //}

        pub u64 read(File* self,i8* cstr,u64 count){
            return sys::read(self->fd,cstr,count);
        }
        pub void write(File* self,i8* cstr,u64 count){
            sys::write(self->fd,cstr,count);
        }
    }
}