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

// NT functions, exposed through your ABI adapters.
extern i32 NtReadFile(
    u64 a, u64 b, u64 c, u64 d, IO_STATUS_BLOCK* e,
    i8* f, u32 g, LARGE_INTEGER* h, u32* i
);

extern i32 NtWriteFile(
    u64 a, u64 b, u64 c, u64 d, IO_STATUS_BLOCK* e,
    i8* f, u32 g, LARGE_INTEGER* h, u32* i
);

extern i32 NtCreateFile(
    u64* a, u32 b, OBJECT_ATTRIBUTES* c, IO_STATUS_BLOCK* d,
    LARGE_INTEGER* e, u32 f, u32 g, u32 h, u32 i, void* j, u32 k
);

extern i32 NtClose(u64 a);

extern i32 NtDeviceIoControlFile(
    u64 a, u64 b, u64 c, u64 d, IO_STATUS_BLOCK* e,
    u32 f, void* g, u32 h, void* j, u32 i
);
extern i32 NtAllocateVirtualMemory(u64 a, void** b, u64 c, u64* d, u32 e, u32 f);
extern i32 NtProtectVirtualMemory(u64 a, void** b, u64* c, u32 d, u32* e);
extern i32 NtFreeVirtualMemory(u64 a, void** b, u64* c, u32 d);
extern i32 NtQueryInformationProcess(u64 a, u32 b, void* c, u32 d, u32* e);
extern i32 NtTerminateProcess(u64 a, i32 b);
extern i32 NtWaitForSingleObject(u64 a, u32 b, LARGE_INTEGER* c);
extern i32 NtDelayExecution(u32 a, LARGE_INTEGER* b);
extern i32 NtQueryPerformanceCounter(LARGE_INTEGER* a, LARGE_INTEGER* b);

extern u64 stdin();
extern u64 stdout();
extern u64 stderr();

namespace sys {
    u32 page_protection(u64 prot) {
        if prot & 4 {
            if prot & 2 { return 64; }
            if prot & 1 { return 32; }
            return 16;
        }

        if prot & 3 { return 4; }
        return 1;
    }

    i32 read(u64 handle, i8* buffer, u64 count) {
        IO_STATUS_BLOCK iosb;

        i32 status = NtReadFile(
            handle, 0, 0, 0, &iosb,
            buffer, (u32)count, null, null
        );

        if status >= 0 {
            return (i32)iosb.Information;
        }

        return status;
    }

    i32 write(u64 handle, i8* buffer, u64 count) {
        IO_STATUS_BLOCK iosb;

        i32 status = NtWriteFile(
            handle, 0, 0, 0, &iosb,
            buffer, (u32)count, null, null
        );

        if status >= 0 {
            return (i32)iosb.Information;
        }

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
        oa.Attributes = 64;
        oa.Padding1 = 0;
        oa.SecurityDescriptor = 0;
        oa.SecurityQualityOfService = 0;

        i32 status = NtCreateFile(
            &handle, desired_access, &oa, &iosb,
            null, 0, share_access, disposition,
            options, null, 0
        );

        if status >= 0 && out_handle != null {
            *out_handle = handle;
        }

        return status;
    }

    i32 close(u64 handle) {
        return NtClose(handle);
    }

    i32 ioctrl(
        u64 handle, u32 control_code,
        void* input_buffer, u32 input_length,
        void* output_buffer, u32 output_length,
        IO_STATUS_BLOCK* iosb
    ) {
        return NtDeviceIoControlFile(
            handle, 0, 0, 0, iosb,
            control_code,
            input_buffer, input_length,
            output_buffer, output_length
        );
    }

    void* alloc(u64 len) {
        void* base = null;
        u64 size = len;

        i32 status = NtAllocateVirtualMemory(
            -1,
            &base, 0, &size, 0x3000, 4
        );

        if status < 0 { return null; }
        return base;
    }

    i32 protect(void* addr, u64 len, u64 prot) {
        void* base = addr;
        u64 size = len;
        u32 old_protection;

        return NtProtectVirtualMemory(
            -1,
            &base, &size,
            sys::page_protection(prot),
            &old_protection
        );
    }

    i32 munmap(void* addr, u64 len) {
        void* base = addr;
        u64 size = 0;

        return NtFreeVirtualMemory(
            -1,
            &base, &size, 0x8000
        );
    }

    u64 getpid() {
        PROCESS_BASIC_INFORMATION pbi;
        u32 returned_length;

        i32 status = NtQueryInformationProcess(
            -1,
            0, &pbi, 48, &returned_length
        );

        if status < 0 { return 0; }
        return pbi.UniqueProcessId;
    }

    void term(u64 pid,i32 code) {
        NtTerminateProcess(pid, code);
    }

    global void exit(i32 code) {
        NtTerminateProcess(-1, code);
    }

    i32 wait_handle(u64 handle, u64 alertable,LARGE_INTEGER* timeout) {
        return NtWaitForSingleObject(
            handle, (u32)alertable, timeout
        );
    }

    i32 nanosleep(u64 nanos) {
        if nanos == 0 { return 0; }

        LARGE_INTEGER interval;
        interval.QuadPart = -((i64)(nanos / 100 + (nanos % 100 != 0)));
        return NtDelayExecution(0, &interval);
    }

    u64 time_nano() {
        LARGE_INTEGER counter;
        LARGE_INTEGER frequency;
        i32 status = NtQueryPerformanceCounter(&counter, &frequency);

        if status < 0 || frequency.QuadPart <= 0 {
            return 0;
        }

        u64 ticks = (u64)counter.QuadPart;
        u64 freq = (u64)frequency.QuadPart;
        return ticks / freq * 1000000000 + (ticks % freq) * 1000000000 / freq;
    }
    u64 time_micro() {
        return sys::time_nano() / 1000;
    }
    u64 time_milli() {
        return sys::time_nano() / 1000000;
    }
}