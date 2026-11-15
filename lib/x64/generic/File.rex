

import System;

namespace fsys {
    struct FileStat [
        pub u64 st_dev,
        pub u64 st_ino,
        pub u64 st_nlink,
        pub u32 st_mode,
        pub u32 st_uid,
        pub u32 st_gid,
        pub u32 padding1,
        pub u64 st_rdev,
        pub u64 st_size,
        pub u64 st_blksize,
        pub u64 st_blocks,
        pub u64 st_atime,
        pub u64 st_atime_nsec,
        pub u64 st_mtime,
        pub u64 st_mtime_nsec,
        pub u64 st_ctime,
        pub u64 st_ctime_nsec,
        pub u64 padding2,
        pub u64 padding3,
        pub u64 padding4
    ];

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
        pub i8 isopen(File* self){
            return (self->fd >> 63) == 0;
        }
        pub void close(File* self){
            if self->fd > 2 && self->isopen() {
                sys::close(self->fd);
            }
            i64 invalid = -1;
            self->fd = (u64)invalid;
        }

        pub u64 size(File* self){
            FileStat filestate;
            sys::fstat(self->fd,(void*)&filestate);
            return filestate.st_size;
        }
        pub void set_pos_start(File* self,u64 pos){
            sys::lseek(self->fd,pos,0);
        }
        pub void set_pos_rel(File* self,u64 pos){
            sys::lseek(self->fd,pos,1);
        }
        pub void set_pos_end(File* self,u64 pos){
            sys::lseek(self->fd,pos,2);
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