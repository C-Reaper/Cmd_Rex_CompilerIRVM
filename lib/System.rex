extern void syscall(i64 rax,i64 rdi,i64 rsi,i64 rdx,i64 r10,i64 r8);

void read(i64 fd,i8* buf,i64 count){
    syscall(0,fd,(i64)buf,count,0,0);
}
void write(i64 fd,i8* buf,i64 count){
    syscall(1,fd,(i64)buf,count,0,0);
}
void open(i64 fd,i64 flags,i64 mode){
    syscall(2,fd,flags,mode,0,0);
}
void close(i64 fd){
    syscall(3,fd,0,0,0,0);
}
void exit(i64 code){
    syscall(60,code,0,0,0,0);
}