extern u64 syscall(u64 rax,u64 rdi,u64 rsi,u64 rdx,u64 r10,u64 r8);

namespace sys {
    struct Timeval [
        pub u64 tv_sec,   // seconds
        pub u64 tv_usec   // micro-seconds (0–999999)
    ];

    struct Timespec [
        pub u64 tv_sec,   // seconds
        pub u64 tv_nsec   // nano-seconds (0–999999999)
    ];

    struct RUsage [
        pub Timeval ru_utime,  // User CPU time
        pub Timeval ru_stime,  // System CPU time

        pub u64 ru_maxrss,     // Maximum resident set size (KiB)
        pub u64 ru_ixrss,      // Integral shared memory size
        pub u64 ru_idrss,      // Integral unshared data size
        pub u64 ru_isrss,      // Integral unshared stack size

        pub u64 ru_minflt,     // Minor page faults
        pub u64 ru_majflt,     // Major page faults
        pub u64 ru_nswap,      // Swaps

        pub u64 ru_inblock,    // Block input operations
        pub u64 ru_oublock,    // Block output operations

        pub u64 ru_msgsnd,     // Messages sent
        pub u64 ru_msgrcv,     // Messages received

        pub u64 ru_nsignals,   // Signals received
        pub u64 ru_nvcsw,      // Voluntary context switches
        pub u64 ru_nivcsw      // Involuntary context switches
    ];

    void read(u64 fd,i8* buf,u64 count){
        syscall(0,fd,(u64)buf,count,0,0);
    }
    void write(u64 fd,i8* buf,u64 count){
        syscall(1,fd,(u64)buf,count,0,0);
    }
    void open(u64 fd,u64 flags,u64 mode){
        syscall(2,fd,flags,mode,0,0);
    }
    void close(u64 fd){
        syscall(3,fd,0,0,0,0);
    }

    // PROT_READ, PROT_WRITE
    // MAP_PRIVATE, MAP_ANONYMOUS
    // 0,...,3,0x22,0,0
    void* mmap(void* addr,u64 len,u64 prot,u64 flags,u64 fd,u64 offset){
        return (void*)syscall(9,(u64)addr,len,prot,flags,fd);
    }
    void mprotect(void* addr,u64 len,u64 prot){
        syscall(10,(u64)addr,len,prot,0,0);
    }
    void munmap(void* addr,u64 len){
        syscall(11,(u64)addr,len,0,0,0);
    }
    void brk(u64 start){
        syscall(12,start,0,0,0,0);
    }

    void ioctrl(u64 fd,u64 cmd,u64 arg){
        syscall(16,fd,cmd,arg,0,0);
    }
    void pipe(i32* fildes){
        syscall(22,(u64)fildes,0,0,0,0);
    }
    void select(u64 n,u32* inp,u32* outp,u32* exp,Timeval* tvp){
        syscall(23,n,(u64)inp,(u64)outp,(u64)exp,(u64)tvp);
    }
    void pause(){
        syscall(34,0,0,0,0,0);
    }
    void nanosleep(Timespec* rqtp,Timespec* rmtp){
        syscall(35,(u64)rqtp,(u64)rmtp,0,0,0);
    }
    void alarm(u64 sec){
        syscall(37,sec,0,0,0,0);
    }
    void getpid(){
        syscall(39,0,0,0,0,0);
    }

    void clone(u64 a0,u64 a1,i32* a2,i32* a3,u64 a4){
        syscall(56,a0,a1,(u64)a2,(u64)a3,a4);
    }
    void fork(){
        syscall(57,0,0,0,0,0);
    }
    void vfork(){
        syscall(58,0,0,0,0,0);
    }
    void execve(i8* filename,i8** argv,i8** envp){
        syscall(59,(u64)filename,(u64)argv,(u64)envp,0,0);
    }
    void exit(u64 code){
        syscall(60,code,0,0,0,0);
    }
    void wait4(u64 pid,i32* stat_addr,i32 options,RUsage* ru){
        syscall(61,pid,(u64)stat_addr,options,(u64)ru,0);
    }

    void fcntl(u64 fd,u64 cmd,u64 arg){
        syscall(72,fd,cmd,arg,0,0);
    }

    void clock_gettime(u64 clockid,Timespec* rmtp){
        syscall(228,clockid,(u64)rmtp,0,0,0);
    }
}
