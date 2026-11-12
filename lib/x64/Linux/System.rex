extern u64 syscall(u64 a,u64 b,u64 c,u64 d,u64 e,u64 f);

define CLOCK_REALTIME           0;
define CLOCK_MONOTONIC          1;
define CLOCK_PROCESS_CPUTIME_ID 2;
define CLOCK_THREAD_CPUTIME_ID  3;
define CLOCK_MONOTONIC_RAW      4;
define CLOCK_REALTIME_COARSE    5;
define CLOCK_MONOTONIC_COARSE   6;
define CLOCK_BOOTTIME           7;
define CLOCK_REALTIME_ALARM     8;
define CLOCK_BOOTTIME_ALARM     9;
define CLOCK_TAI               11;

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

    u64 read(u64 fd,i8* buf,u64 count){
        return (u64)syscall(0,fd,(u64)buf,count,0,0);
    }
    u64 write(u64 fd,i8* buf,u64 count){
        return (u64)syscall(1,fd,(u64)buf,count,0,0);
    }
    u64 open(i8* path,u64 flags,u64 mode){
        return (u64)syscall(2,(u64)path,flags,mode,0,0);
    }
    void close(u64 fd){
        syscall(3,fd,0,0,0,0);
    }

    void* alloc(u64 len){
        return (void*)syscall(9,0,len,3,34,0);
    }
    void free(void* addr,u64 len){
        syscall(11,(u64)addr,len,0,0,0);
    }

    void ioctrl(u64 fd,u64 cmd,u64 arg){
        syscall(16,fd,cmd,arg,0,0);
    }
    void nanosleep(u64 nanos){
        Timespec rqtp;
        rqtp.tv_sec = nanos / 1000000000;
        rqtp.tv_nsec = nanos % 1000000000;
        syscall(35,(u64)&rqtp,0,0,0,0);
    }
    void exit(u64 code){
        syscall(60,code,0,0,0,0);
    }

    u64 time_nano(){
        sys::Timespec sp;
        syscall(228,CLOCK_MONOTONIC,(u64)&sp,0,0,0);
        return (u64)sp.tv_sec * 1000000000 + (u64)sp.tv_nsec;
    }
    u64 time_micro(){
        return sys::time_nano() / 1000;
    }
    u64 time_milli(){
        return sys::time_nano() / 1000000;
    }
}
