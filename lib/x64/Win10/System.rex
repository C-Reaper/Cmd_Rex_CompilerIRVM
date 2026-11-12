
extern u64 syscall(u64 number,
    u64 a0, u64 a1, u64 a2, u64 a3, u64 a4,
    u64 a5, u64 a6, u64 a7, u64 a8, u64 a9, u64 a10
);

namespace sys {
    struct LARGE_INTEGER [
        pub i64 QuadPart
    ];

    struct IO_STATUS_BLOCK [
        pub u64 StatusOrPointer,
        pub u64 Information
    ];

    struct UNICODE_STRING [
        pub u16 Length,
        pub u16 MaximumLength,
        pub u32 Padding,
        pub u64 Buffer
    ];

    struct OBJECT_ATTRIBUTES [
        pub u32 Length,
        pub u32 Padding0,
        pub u64 RootDirectory,
        pub u64 ObjectName,
        pub u32 Attributes,
        pub u32 Padding1,
        pub u64 SecurityDescriptor,
        pub u64 SecurityQualityOfService
    ];

    struct PROCESS_BASIC_INFORMATION [
        pub i32 ExitStatus,
        pub u32 Padding0,
        pub u64 PebBaseAddress,
        pub u64 AffinityMask,
        pub i32 BasePriority,
        pub u32 Padding1,
        pub u64 UniqueProcessId,
        pub u64 InheritedFromUniqueProcessId
    ];

    u32 page_protection(u64 prot) {
        if (prot & 4) != 0 {
            if (prot & 2) != 0 {
                return 64;
            }
            if (prot & 1) != 0 {
                return 32;
            }
            return 16;
        }
        if (prot & 3) != 0 {
            return 4;
        }
        return 1;
    }

    // NtReadFile = 6
    i32 read(u64 handle, i8* buffer, u64 count) {
        IO_STATUS_BLOCK iosb;
        return (i32)syscall(6, handle, 0, 0, 0, (u64)&iosb,(u64)buffer, count, 0, 0, 0, 0);
    }

    // NtWriteFile = 8
    i32 write(u64 handle, i8* buffer, u64 count) {
        IO_STATUS_BLOCK iosb;
        return (i32)syscall(8, handle, 0, 0, 0, (u64)&iosb,(u64)buffer, count, 0, 0, 0, 0);
    }

    // NtCreateFile = 85
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
        oa.Attributes = 64;
        oa.Padding1 = 0;
        oa.SecurityDescriptor = 0;
        oa.SecurityQualityOfService = 0;

        i32 status = (i32)syscall(85,
            (u64)&handle, desired_access, (u64)&oa, (u64)&iosb,
            0, 0, share_access, disposition, options, 0, 0
        );

        if status == 0 && out_handle != 0 {
            *out_handle = handle;
        }
        return status;
    }

    // NtClose = 15
    i32 close(u64 handle) {
        return (i32)syscall(15, handle, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }

    // NtDeviceIoControlFile = 7
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

    // NtAllocateVirtualMemory = 24
    void* alloc(u64 len) {
        void* base = null;
        u64 size = len;

        syscall(24,
            0xFFFFFFFFFFFFFFFF, (u64)&base, 0, (u64)&size,
            0x3000, 4, 0, 0, 0, 0, 0
        );

        return base;
    }

    // NtProtectVirtualMemory = 80
    i32 protect(void* addr, u64 len, u64 prot) {
        void* base = addr;
        u64 size = len;
        u32 old_protection;

        return (i32)syscall(80,
            0xFFFFFFFFFFFFFFFF, (u64)&base, (u64)&size,
            sys::page_protection(prot), (u64)&old_protection,
            0, 0, 0, 0, 0, 0
        );
    }

    // NtFreeVirtualMemory = 30
    i32 munmap(void* addr, u64 len) {
        void* base = addr;
        u64 size = 0;

        return (i32)syscall(30,
            0xFFFFFFFFFFFFFFFF, (u64)&base, (u64)&size,
            0x8000, 0, 0, 0, 0, 0, 0, 0
        );
    }

    // NtQueryInformationProcess = 25
    u64 getpid() {
        PROCESS_BASIC_INFORMATION pbi;
        u32 returned_length;

        syscall(25,
            0xFFFFFFFFFFFFFFFF, 0, (u64)&pbi, 48,
            (u64)&returned_length, 0, 0, 0, 0, 0, 0
        );
        
        return pbi.UniqueProcessId;
    }

    // NtTerminateProcess = 44
    global void exit(u64 code) {
        syscall(44,
            0xFFFFFFFFFFFFFFFF, code,
            0, 0, 0, 0, 0, 0, 0, 0, 0
        );
    }

    // NtWaitForSingleObject = 4
    i32 wait_handle(u64 handle, u64 alertable, LARGE_INTEGER* timeout) {
        return (i32)syscall(4,
            handle, alertable, (u64)timeout,
            0, 0, 0, 0, 0, 0, 0, 0
        );
    }

    // NtDelayExecution = 52
    i32 nanosleep(u64 nanos) {
        if nanos == 0 {
            return 0;
        }

        LARGE_INTEGER interval;
        interval.QuadPart = -(i64)(nanos / 100 + (nanos % 100 != 0));

        return (i32)syscall(52,
            0, (u64)&interval,
            0, 0, 0, 0, 0, 0, 0, 0, 0
        );
    }

    // NtQueryPerformanceCounter = 49
    u64 time_nano() {
        LARGE_INTEGER counter;
        LARGE_INTEGER frequency;

        if (i32)syscall(49,
            (u64)&counter, (u64)&frequency,
            0, 0, 0, 0, 0, 0, 0, 0, 0
        ) < 0 || frequency.QuadPart <= 0 {
            return 0;
        }

        u64 ticks = (u64)counter.QuadPart;
        u64 freq = (u64)frequency.QuadPart;

        return ticks / freq * 1000000000
            + (ticks % freq) * 1000000000 / freq;
    }

    u64 time_micro() {
        return sys::time_nano() / 1000;
    }

    u64 time_milli() {
        return sys::time_nano() / 1000000;
    }
}
