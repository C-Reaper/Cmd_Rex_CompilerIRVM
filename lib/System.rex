extern void syscall(i64 rax,i64 rdi,i64 rsi,i64 rdx,i64 r10,i64 r8);

namespace sys {
    struct Timeval [
        i64 tv_sec,   // seconds
        i64 tv_usec   // micro-seconds (0–999999)
    ];

    struct Timespec [
        i64 tv_sec,   // seconds
        i64 tv_nsec   // nano-seconds (0–999999999)
    ];

    struct RUsage [
        Timeval ru_utime,  // User CPU time
        Timeval ru_stime,  // System CPU time

        i64 ru_maxrss,     // Maximum resident set size (KiB)
        i64 ru_ixrss,      // Integral shared memory size
        i64 ru_idrss,      // Integral unshared data size
        i64 ru_isrss,      // Integral unshared stack size

        i64 ru_minflt,     // Minor page faults
        i64 ru_majflt,     // Major page faults
        i64 ru_nswap,      // Swaps

        i64 ru_inblock,    // Block input operations
        i64 ru_oublock,    // Block output operations

        i64 ru_msgsnd,     // Messages sent
        i64 ru_msgrcv,     // Messages received

        i64 ru_nsignals,   // Signals received
        i64 ru_nvcsw,      // Voluntary context switches
        i64 ru_nivcsw      // Involuntary context switches
    ];

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

    // PROT_READ, PROT_WRITE
    // MAP_PRIVATE, MAP_ANONYMOUS
    void mmap(void* addr,i64 len,i64 prot,i64 flags,i64 fd,i64 offset){
        syscall(9,(i64)addr,len,prot,flags,fd);
    }
    void mprotect(u64 start,u64 len,u64 prot){
        syscall(11,start,len,prot,0,0);
    }
    void munmap(u64 start,u64 len){
        syscall(11,start,len,0,0,0);
    }
    void brk(u64 start){
        syscall(12,start,0,0,0,0);
    }

    void ioctrl(u64 fd,u64 cmd,u64 arg){
        syscall(16,fd,cmd,arg,0,0);
    }
    void pipe(i32* fildes){
        syscall(22,(i64)fildes,0,0,0,0);
    }
    void select(i32 n,u32* inp,u32* outp,u32* exp,Timeval* tvp){
        syscall(23,n,(i64)inp,(i64)outp,(i64)exp,(i64)tvp);
    }
    void pause(){
        syscall(34,0,0,0,0,0);
    }
    void nanosleep(Timespec* rqtp,Timespec* rmtp){
        syscall(35,(i64)rqtp,(i64)rmtp,0,0,0);
    }
    void alarm(u64 sec){
        syscall(37,sec,0,0,0,0);
    }
    void getpid(){
        syscall(39,0,0,0,0,0);
    }

    void clone(u64 a0,u64 a1,i32* a2,i32* a3,u64 a4){
        syscall(56,a0,a1,(i64)a2,(i64)a3,a4);
    }
    void fork(){
        syscall(57,0,0,0,0,0);
    }
    void vfork(){
        syscall(58,0,0,0,0,0);
    }
    void execve(i8* filename,i8** argv,i8** envp){
        syscall(59,(i64)filename,(i64)argv,(i64)envp,0,0);
    }
    void exit(i64 code){
        syscall(60,code,0,0,0,0);
    }
    void wait4(u64 pid,i32* stat_addr,i32 options,RUsage* ru){
        syscall(61,pid,(i64)stat_addr,options,(i64)ru,0);
    }

    void fcntl(u64 fd,u64 cmd,u64 arg){
        syscall(72,fd,cmd,arg,0,0);
    }
}
