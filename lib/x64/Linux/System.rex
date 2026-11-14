extern u64 syscall(i64 a,i64 b,i64 c,i64 d,i64 e,i64 f);

extern u64 stdin();
extern u64 stdout();
extern u64 stderr();

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

define O_RDONLY                 0;        // Datei nur zum Lesen.
define O_WRONLY                 1;        // Datei nur zum Schreiben.
define O_RDWR                   2;        // Datei sowohl zum Lesen als auch zum Schreiben.
define O_CREAT                  64;       // Erzeugt die Datei, falls sie nicht existiert. Wenn dieses Flag gesetzt ist, muss auch ein Berechtigungs-Mode angegeben werden (z. B. 0644 für Lese-/Schreibrechte).
define O_EXCL                   128;      // Wird mit O_CREAT verwendet, um sicherzustellen, dass der Syscall fehlschlägt, wenn die Datei bereits existiert.
define O_TRUNC                  512;      // Schneidet die Datei auf eine Länge von 0 ab, falls sie existiert und zum Schreiben oder zum Lesen/Schreiben geöffnet wurde.
define O_APPEND                 1024;     // Setzt den Schreibzeiger an das Ende der Datei, sodass alle Schreibvorgänge ans Ende der Datei angehängt werden.
define O_NONBLOCK               2048;     // Öffnet die Datei im nicht-blockierenden Modus. Wird häufig für Spezialdateien wie Pipes und FIFO-Dateien verwendet.
define O_DSYNC                  4096;     // Sorgt dafür, dass Schreiboperationen direkt auf die Festplatte geschrieben werden, sodass die Daten synchron sind, aber Metadaten (wie Änderungszeit) möglicherweise nicht.
define O_SYNC                   1052672;  // Wie O_DSYNC, aber garantiert zusätzlich, dass alle Metadaten aktualisiert werden.
define O_NOFOLLOW               256;      // Verhindert das Folgen von symbolischen Links. Falls die Datei ein symbolischer Link ist, schlägt open mit einem Fehler fehl.
define O_CLOEXEC                524288;   // Setzt das FD_CLOEXEC-Flag, welches den Dateideskriptor automatisch schließt, wenn ein exec-Aufruf ausgeführt wird.
define O_DIRECTORY              65536;    // Öffnet die Datei nur, wenn sie ein Verzeichnis ist; anderenfalls schlägt der Syscall mit einem Fehler fehl.
define O_TMPFILE                4259840;  // Erzeugt eine temporäre Datei, die beim Schließen automatisch gelöscht wird, falls sie keinen Link im Dateisystem hat.

define F_GETFL                  3;
define F_SETFL                  4;

define SEEK_SET                 0;        // Datei Anfang.
define SEEK_CUR                 1;        // Datei relativ.
define SEEK_END                 2;        // Datei Ende.

define STDIN                    0;
define STDOUT                   1;
define STDERR                   2;

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

    struct Stat [
        pub u64 st_dev,
        pub u64 st_ino,
        pub u64 st_nlink,
        pub u32 st_mode,
        pub u32 st_uid,
        pub u32 st_gid,
        u32 padding1,
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
        u64 padding2,
        u64 padding3,
        u64 padding4
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

    void stat(i8* path,Stat* ptr){
        syscall(4,(u64)path,(u64)ptr,0,0,0);
    }
    void fstat(u64 fd,Stat* ptr){
        syscall(5,fd,(u64)ptr,0,0,0);
    }
    void lseek(u64 fd,u64 offset,u64 flags){
        syscall(8,fd,offset,flags,0,0);
    }

    void* alloc(u64 len){
        return (void*)syscall(9,0,len,3,34,-1);
    }
    void free(void* addr,u64 len){
        syscall(11,(u64)addr,len,0,0,0);
    }

    u64 ioctrl(u64 fd,u64 cmd,void* t,u64 ap){
        return syscall(16,fd,cmd,(u64)t,ap,0);
    }

    void pause(){
        syscall(34,0,0,0,0,0);
    }
    void nanosleep(u64 nanos){
        Timespec rqtp;
        rqtp.tv_sec = nanos / 1000000000;
        rqtp.tv_nsec = nanos % 1000000000;
        syscall(35,(u64)&rqtp,0,0,0,0);
    }
    u64 getpid(){
        return syscall(39,0,0,0,0,0);
    }

    global void exit(u64 code){
        syscall(60,code,0,0,0,0);
    }

    u64 waitpid(u64 pid,i32* status,u64 options){
        return syscall(61,pid,(u64)status,options,0,0);
    }
    u64 kill(u64 pid,u64 flags){
        return syscall(62,pid,flags,0,0,0);
    }

    u64 fcntl(u64 fd,u64 cmd,u64 flags){
        return syscall(72,fd,cmd,flags,0,0);
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
