// Windows x64 native syscall layer
// Target: Windows 11 24H2 x64 (build 26100)
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

namespace sys {

    // ---------------------------------------------------------------------
    // NT data types / structures (Windows x64 layout)
    // ---------------------------------------------------------------------

    struct Timeval [
        pub i64 tv_sec;
        pub i64 tv_usec;
    ];

    struct Timespec [
        pub i64 tv_sec;
        pub i64 tv_nsec;
    ];

    struct LARGE_INTEGER [
        pub i64 QuadPart;
    ];

    // x64: union(Status/Pointer) occupies 8 bytes, followed by Information.
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
        if ((prot & PROT_EXEC) != 0) {
            if ((prot & PROT_WRITE) != 0) return PAGE_EXECUTE_READWRITE;
            if ((prot & PROT_READ) != 0) return PAGE_EXECUTE_READ;
            return PAGE_EXECUTE;
        }
        if ((prot & PROT_WRITE) != 0) return PAGE_READWRITE;
        if ((prot & PROT_READ) != 0) return PAGE_READONLY;
        return PAGE_NOACCESS;
    }

    i32 read(u64 handle, i8* buffer, u64 count, u64* bytes_read) {
        IO_STATUS_BLOCK iosb;
        i32 status = (i32)syscall(6,
            handle, 0, 0, 0, (u64)&iosb,
            (u64)buffer, count, 0, 0, 0, 0
        );
        if (bytes_read != 0) *bytes_read = iosb.Information;
        return status;
    }
    i32 write(u64 handle, i8* buffer, u64 count, u64* bytes_written) {
        IO_STATUS_BLOCK iosb;
        i32 status = (i32)syscall(8,
            handle, 0, 0, 0, (u64)&iosb,
            (u64)buffer, count, 0, 0, 0, 0
        );
        if (bytes_written != 0) *bytes_written = iosb.Information;
        return status;
    }
    i32 open(
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

        if (status == 0 && out_handle != 0) *out_handle = handle;
        return status;
    }

    i32 close(u64 handle) {
        return (i32)syscall(15,
            handle, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        );
    }
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

    void* mmap(void* addr, u64 len, u64 prot, u64 flags, u64 fd, u64 offset) {
        void* base = addr;
        u64 region_size = len;
        u32 protection = page_protection(prot);

        i32 status = (i32)syscall(24,
            CURRENT_PROCESS, (u64)&base, 0, (u64)&region_size,
            MEM_RESERVE | MEM_COMMIT, protection,
            0, 0, 0, 0, 0
        );

        if (status < 0) return (void*)0;
        return base;
    }
    i32 mprotect(void* addr, u64 len, u64 prot) {
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
    i32 munmap(void* addr, u64 len) {
        void* base = addr;
        u64 region_size = 0;

        return (i32)syscall(30,
            CURRENT_PROCESS, (u64)&base, (u64)&region_size,
            MEM_RELEASE,
            0, 0, 0, 0, 0, 0, 0
        );
    }
    void brk(u64 start) {
        // Not implemented: process heap / program break is not an NT syscall.
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

        if (status < 0) return 0;
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
    // req is relative; Linux-like Timespec is converted to negative 100-ns units.
    i32 nanosleep(Timespec* req, Timespec* rem) {
        if (req == 0) return -1;

        LARGE_INTEGER interval;
        i64 ticks = req.tv_sec * 10000000 + req.tv_nsec / 100;
        interval.QuadPart = -ticks;

        return (i32)syscall(52,
            0, (u64)&interval,
            0, 0, 0, 0, 0, 0, 0, 0, 0
        );
    }

    // NtQuerySystemTime = 90
    // clockid 0 = realtime. NT epoch starts 1601-01-01; Unix starts 1970-01-01.
    // NtQueryPerformanceCounter = 49
    // clockid 1 = monotonic.
    i32 clock_gettime(u64 clockid, Timespec* tp) {
        if (tp == 0) return -1;

        if (clockid == 0) {
            LARGE_INTEGER system_time;
            i32 status = (i32)syscall(90,
                (u64)&system_time,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            );
            if (status < 0) return status;

            u64 unix_100ns = (u64)(system_time.QuadPart - 116444736000000000);
            tp.tv_sec = (i64)(unix_100ns / 10000000);
            tp.tv_nsec = (i64)((unix_100ns % 10000000) * 100);
            return 0;
        }

        if (clockid == 1) {
            LARGE_INTEGER counter;
            LARGE_INTEGER frequency;
            i32 status = (i32)syscall(49,
                (u64)&counter, (u64)&frequency,
                0, 0, 0, 0, 0, 0, 0, 0, 0
            );
            if (status < 0 || frequency.QuadPart <= 0) return status;

            u64 ticks = (u64)counter.QuadPart;
            u64 freq = (u64)frequency.QuadPart;
            tp.tv_sec = (i64)(ticks / freq);
            tp.tv_nsec = (i64)(((ticks % freq) * 1000000000) / freq);
            return 0;
        }

        return -1;
    }
}
