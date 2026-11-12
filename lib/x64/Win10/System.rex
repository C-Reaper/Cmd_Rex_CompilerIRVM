// Windows x64 native syscall layer
// Target: Windows 10 22H2 x64 (build 19045)
// Syscall IDs: x64 ntoskrnl service table, verified against the j00ru-derived
// SysWhispers4 table. These numbers are build-specific and are NOT a stable
// Microsoft ABI.
//
// IMPORTANT:
// - syscall(number, a0..a10) must be implemented by your own x64 assembly stub.
// - NTSTATUS is returned directly; STATUS_SUCCESS == 0, failure has bit 31 set.
// - Handles are NT handles, not POSIX file descriptors.
// - File paths passed to open() must be UTF-16 and normally start with
//   "\\??\\C:\\" (or another Object Manager path).
// - No Win32/WinAPI functions are used here.

extern u64 syscall(
    u64 number,
    u64 a0, u64 a1, u64 a2, u64 a3,
    u64 a4, u64 a5, u64 a6, u64 a7,
    u64 a8, u64 a9, u64 a10
);

// Pseudo handle for the current process.
define CURRENT_PROCESS              0xFFFFFFFFFFFFFFFF;

// Object Manager
define OBJ_CASE_INSENSITIVE         0x00000040;

// File access masks
define FILE_READ_DATA               0x00000001;
define FILE_WRITE_DATA              0x00000002;
define FILE_APPEND_DATA             0x00000004;
define SYNCHRONIZE                  0x00100000;
define GENERIC_READ                 0x80000000;
define GENERIC_WRITE                0x40000000;

// File sharing
define FILE_SHARE_READ              0x00000001;
define FILE_SHARE_WRITE             0x00000002;
define FILE_SHARE_DELETE            0x00000004;

// NtCreateFile disposition
define FILE_SUPERSEDE               0;
define FILE_OPEN                    1;
define FILE_CREATE                  2;
define FILE_OPEN_IF                 3;
define FILE_OVERWRITE               4;
define FILE_OVERWRITE_IF            5;

// NtCreateFile options
define FILE_DIRECTORY_FILE          0x00000001;
define FILE_SYNCHRONOUS_IO_NONALERT 0x00000020;
define FILE_NON_DIRECTORY_FILE      0x00000040;

// Virtual memory
define MEM_COMMIT                   0x00001000;
define MEM_RESERVE                  0x00002000;
define MEM_RELEASE                  0x00008000;

// Windows page protection values
define PAGE_NOACCESS                0x01;
define PAGE_READONLY                0x02;
define PAGE_READWRITE               0x04;
define PAGE_EXECUTE                 0x10;
define PAGE_EXECUTE_READ            0x20;
define PAGE_EXECUTE_READWRITE       0x40;

// Linux-like PROT bits accepted by mmap/mprotect.
define PROT_READ                    1;
define PROT_WRITE                   2;
define PROT_EXEC                    4;

// ProcessBasicInformation
define ProcessBasicInformation      0;

namespace sys {
    struct LARGE_INTEGER [
        pub i64 QuadPart;
    ];

    struct IO_STATUS_BLOCK [
        pub u64 StatusOrPointer;
        pub u64 Information;
    ];

    struct UNICODE_STRING [
        pub u16 Length;
        pub u16 MaximumLength;
        pub u32 Padding;
        pub u64 Buffer;
    ];

    struct OBJECT_ATTRIBUTES [
        pub u32 Length;
        pub u32 Padding0;
        pub u64 RootDirectory;
        pub u64 ObjectName;
        pub u32 Attributes;
        pub u32 Padding1;
        pub u64 SecurityDescriptor;
        pub u64 SecurityQualityOfService;
    ];

    struct PROCESS_BASIC_INFORMATION [
        pub i32 ExitStatus;
        pub u32 Padding0;
        pub u64 PebBaseAddress;
        pub u64 AffinityMask;
        pub i32 BasePriority;
        pub u32 Padding1;
        pub u64 UniqueProcessId;
        pub u64 InheritedFromUniqueProcessId;
    ];

    u32 page_protection(u64 prot) {
        if (prot & PROT_EXEC) != 0 {
            if (prot & PROT_WRITE) != 0 {
                return PAGE_EXECUTE_READWRITE;
            }
            if (prot & PROT_READ) != 0 {
                return PAGE_EXECUTE_READ;
            }
            return PAGE_EXECUTE;
        }
        if (prot & PROT_WRITE) != 0 {
            return PAGE_READWRITE;
        }
        if (prot & PROT_READ) != 0 {
            return PAGE_READONLY;
        }
        return PAGE_NOACCESS;
    }

    // NtReadFile = 6
    // NT signature:
    // (FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock,
    //  Buffer, Length, ByteOffset, Key)
    i32 read(u64 handle, i8* buffer, u64 count) {
        IO_STATUS_BLOCK iosb;
        //if (bytes_read != 0) *bytes_read = iosb.Information;
        return (i32)syscall(6,
            handle, 0, 0, 0, (u64)&iosb,
            (u64)buffer, count, 0, 0, 0, 0
        );
    }
    i32 write(u64 handle, i8* buffer, u64 count) {
        IO_STATUS_BLOCK iosb;
        //if (bytes_written != 0) *bytes_written = iosb.Information;
        return (i32)syscall(8,
            handle, 0, 0, 0, (u64)&iosb,
            (u64)buffer, count, 0, 0, 0, 0
        );
    }

    // NtCreateFile = 85
    // path is a UTF-16 string; path_bytes is its byte length (not char count).
    // The caller supplies NT access/share/disposition/options values.
    u64 open(
        u16* path, u16 path_bytes,
        u32 desired_access, u32 share_access,
        u32 disposition, u32 options,
        u64* out_handle
    ) {
        UNICODE_STRING name;
        OBJECT_ATTRIBUTES oa;
        IO_STATUS_BLOCK iosb;
        u64 handle = 0;

        name.Length = path_bytes;
        name.MaximumLength = path_bytes;
        name.Padding = 0;
        name.Buffer = (u64)path;

        oa.Length = 48;
        oa.Padding0 = 0;
        oa.RootDirectory = 0;
        oa.ObjectName = (u64)&name;
        oa.Attributes = OBJ_CASE_INSENSITIVE;
        oa.Padding1 = 0;
        oa.SecurityDescriptor = 0;
        oa.SecurityQualityOfService = 0;

        i32 status = (i32)syscall(85,
            (u64)&handle,
            desired_access,
            (u64)&oa,
            (u64)&iosb,
            0,                         // AllocationSize
            0,                         // FileAttributes
            share_access,
            disposition,
            options,
            0,                         // EaBuffer
            0                          // EaLength
        );

        if status == 0 && out_handle != 0 {
            *out_handle = handle;
        }
        return status;
    }

    // NtClose = 15
    i32 close(u64 handle) {
        return (i32)syscall(15,
            handle, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        );
    }

    // NtDeviceIoControlFile = 7
    // Full NT form; replaces the underspecified Linux-style ioctl(fd,cmd,arg).
    i32 ioctrl(
        u64 handle, u32 control_code,
        void* input_buffer, u32 input_length,
        void* output_buffer, u32 output_length,
        IO_STATUS_BLOCK* iosb
    ) {
        return (i32)syscall(7,
            handle, 0, 0, 0, (u64)iosb,
            control_code, (u64)input_buffer, input_length,
            (u64)output_buffer, output_length, 0
        );
    }

    // ---------------------------------------------------------------------
    // Virtual memory
    // ---------------------------------------------------------------------

    // NtAllocateVirtualMemory = 24
    void* alloc(u64 len) {
        void* base = null;
        u64 region_size = len;
        u32 protection = 0;

        i32 status = (i32)syscall(24,
            CURRENT_PROCESS, (u64)&base, 0, (u64)&region_size,
            MEM_RESERVE | MEM_COMMIT, protection,
            0, 0, 0, 0, 0
        );

        if status < 0 {
            return null;
        }
        return base;
    }

    // NtProtectVirtualMemory = 80
    i32 protect(void* addr, u64 len, u64 prot) {
        void* base = addr;
        u64 region_size = len;
        u32 old_protection = 0;
        u32 new_protection = page_protection(prot);

        return (i32)syscall(80,
            CURRENT_PROCESS, (u64)&base, (u64)&region_size,
            new_protection, (u64)&old_protection,
            0, 0, 0, 0, 0, 0
        );
    }

    // NtFreeVirtualMemory = 30
    i32 munmap(void* addr, u64 len) {
        void* base = addr;
        u64 region_size = 0;

        return (i32)syscall(30,
            CURRENT_PROCESS, (u64)&base, (u64)&region_size,
            MEM_RELEASE,
            0, 0, 0, 0, 0, 0, 0
        );
    }
    
    // NtQueryInformationProcess = 25
    u64 getpid() {
        PROCESS_BASIC_INFORMATION pbi;
        i32 returned_length = 0;

        i32 status = (i32)syscall(25,
            CURRENT_PROCESS,
            ProcessBasicInformation,
            (u64)&pbi,
            48,
            (u64)&returned_length,
            0, 0, 0, 0, 0, 0
        );

        if status < 0 {
            return 0;
        }
        return pbi.UniqueProcessId;
    }

    // NtTerminateProcess = 44
    void exit(u64 code) {
        syscall(44,
            CURRENT_PROCESS, code,
            0, 0, 0, 0, 0, 0, 0, 0, 0
        );
    }

    // NtWaitForSingleObject = 4
    // This waits on an NT HANDLE, not a POSIX pid.
    i32 wait_handle(u64 handle, u64 alertable, LARGE_INTEGER* timeout) {
        return (i32)syscall(4,
            handle, alertable, (u64)timeout,
            0, 0, 0, 0, 0, 0, 0, 0
        );
    }

    // NtDelayExecution = 52
    i32 nanosleep(u64 nanos) {
        if req == 0 {
            return -1;
        }

        LARGE_INTEGER interval;
        i64 ticks = nanos / 100;
        interval.QuadPart = -ticks;

        return (i32)syscall(52,
            0, (u64)&interval,
            0, 0, 0, 0, 0, 0, 0, 0, 0
        );
    }

    u64 time_nano(){
        LARGE_INTEGER counter;
        LARGE_INTEGER frequency;
        
        i32 status = (i32)syscall(49,
            (u64)&counter, (u64)&frequency,
            0, 0, 0, 0, 0, 0, 0, 0, 0
        );

        if status < 0 || frequency.QuadPart <= 0 {
            return status;
        }

        u64 ticks = (u64)counter.QuadPart;
        u64 freq = (u64)frequency.QuadPart;
        return (u64)(ticks / freq) * 1000000000 + (((ticks % freq) * 1000000000) / freq);
    }
    u64 time_micro(){
        return sys::time_nano() / 1000;
    }
    u64 time_milli(){
        return sys::time_nano() / 1000000;
    }
}
