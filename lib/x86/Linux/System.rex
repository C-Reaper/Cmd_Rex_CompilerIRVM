
extern u32 syscall(i32 nr,i32 a,i32 b,i32 c,i32 d,i32 e,i32 f);

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
        pub i32 tv_sec,
        pub i32 tv_usec
    ];

    struct Timespec [
        pub i64 tv_sec,
        pub i64 tv_nsec
    ];

    struct RUsage [
        pub Timeval ru_utime,
        pub Timeval ru_stime,

        pub i32 ru_maxrss,
        pub i32 ru_ixrss,
        pub i32 ru_idrss,
        pub i32 ru_isrss,

        pub i32 ru_minflt,
        pub i32 ru_majflt,
        pub i32 ru_nswap,

        pub i32 ru_inblock,
        pub i32 ru_oublock,

        pub i32 ru_msgsnd,
        pub i32 ru_msgrcv,

        pub i32 ru_nsignals,
        pub i32 ru_nvcsw,
        pub i32 ru_nivcsw
    ];

    u32 read(u32 fd,i8* buf,u32 count) {
        return syscall(3,fd,(i32)buf,count,0,0,0);
    }

    u32 write(u32 fd,i8* buf,u32 count) {
        return syscall(4,fd,(i32)buf,count,0,0,0);
    }

    u32 open(i8* path,u32 flags,u32 mode) {
        return syscall(5,(i32)path,flags,mode,0,0,0);
    }

    void close(u32 fd) {
        syscall(6,fd,0,0,0,0,0);
    }

    void* alloc(u32 len) {
        return (void*)syscall(192,0,len,3,34,-1,0);
    }

    void free(void* addr,u32 len) {
        syscall(91,(i32)addr,len,0,0,0,0);
    }

    void ioctl(u32 fd,u32 cmd,u32 arg) {
        syscall(54,fd,cmd,arg,0,0,0);
    }

    void nanosleep(u64 nanos) {
        Timespec rqtp;

        rqtp.tv_sec = nanos / 1000000000;
        rqtp.tv_nsec = nanos % 1000000000;

        syscall(407,(i32)&rqtp,0,0,0,0,0);
    }

    void exit(u32 code) {
        syscall(252,code,0,0,0,0,0);
    }

    u64 time_nano() {
        Timespec sp;
        syscall(403,CLOCK_MONOTONIC,(i32)&sp,0,0,0,0);
        return (u64)sp.tv_sec * 1000000000 + (u64)sp.tv_nsec;
    }
    u64 time_micro() {
        return sys::time_nano() / 1000;
    }
    u64 time_milli() {
        return sys::time_nano() / 1000000;
    }
}
