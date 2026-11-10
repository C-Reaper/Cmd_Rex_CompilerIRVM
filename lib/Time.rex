import "../lib/System.rex";

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

namespace time {
    u64 nget(){
        sys::Timespec sp;
        sys::clock_gettime(,&sp);
        return (u64)sp.tv_sec * 1000000000 + (u64)sp.tv_nsec;
    }
    
    void msleep(i64 msec){
        sys::Timespec sp;
        sp.tv_sec = msec / 1000;
        sp.tv_nsec = (msec % 1000) * 1000000;

        sys::nanosleep(&sp,0);
    }
    void usleep(i64 usec){
        sys::Timespec sp;
        sp.tv_sec = usec / 1000000;
        sp.tv_nsec = (usec % 1000000) * 1000;

        sys::nanosleep(&sp,0);
    }
    void nsleep(i64 nsec){
        sys::Timespec sp;
        sp.tv_sec = nsec / 1000000000;
        sp.tv_nsec = (nsec % 1000000000);

        sys::nanosleep(&sp,0);
    }
}