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

struct FILE_STANDARD_INFORMATION [
    pub i64 AllocationSize,
    pub i64 EndOfFile,
    pub u32 NumberOfLinks,
    pub u8 DeletePending,
    pub u8 Directory,
    pub u16 Padding
];

struct FILE_POSITION_INFORMATION [
    pub i64 CurrentByteOffset
];

struct Stat [
    pub u64 st_dev,
    pub u64 st_ino,
    pub u64 st_nlink,
    pub u32 st_mode,
    pub u32 st_uid,
    pub u32 st_gid,
    pub u32 padding1,
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
    pub u64 padding2,
    pub u64 padding3,
    pub u64 padding4
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
extern i32 NtQueryInformationFile(u64 a, IO_STATUS_BLOCK* b, void* c, u32 d, u32 e);
extern i32 NtSetInformationFile(u64 a, IO_STATUS_BLOCK* b, void* c, u32 d, u32 e);
extern i32 NtTerminateProcess(u64 a, i32 b);
extern i32 NtWaitForSingleObject(u64 a, u32 b, LARGE_INTEGER* c);
extern i32 NtDelayExecution(u32 a, LARGE_INTEGER* b);
extern i32 NtQueryPerformanceCounter(LARGE_INTEGER* a, LARGE_INTEGER* b);
extern i32 RtlUTF8ToUnicodeN(u16* a, u32 b, u32* c, i8* d, u32 e);
extern i32 RtlDosPathNameToNtPathName_U(u16* a, UNICODE_STRING* b, u16** c, void* d);
extern void RtlFreeUnicodeString(UNICODE_STRING* a);

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

    u64 complete_io(u64 handle, i32 status, IO_STATUS_BLOCK* iosb) {
        i32 final_status = status;
        if final_status == 259 {
            i32 wait_status = NtWaitForSingleObject(handle, 0, null);
            if wait_status < 0 {
                i64 error = -1;
                return (u64)error;
            }
            final_status = (i32)iosb->StatusOrPointer;
        }

        if final_status >= 0 {
            return iosb->Information;
        }

        i64 error = -1;
        return (u64)error;
    }

    u64 read(u64 handle, i8* buffer, u64 count) {
        if (count >> 32) != 0 {
            i64 error = -1;
            return (u64)error;
        }

        IO_STATUS_BLOCK iosb;

        i32 status = NtReadFile(
            handle, 0, 0, 0, &iosb,
            buffer, (u32)count, null, null
        );

        return sys::complete_io(handle, status, &iosb);
    }

    u64 write(u64 handle, i8* buffer, u64 count) {
        if (count >> 32) != 0 {
            i64 error = -1;
            return (u64)error;
        }

        IO_STATUS_BLOCK iosb;

        i32 status = NtWriteFile(
            handle, 0, 0, 0, &iosb,
            buffer, (u32)count, null, null
        );

        return sys::complete_io(handle, status, &iosb);
    }

    u64 open(i8* path, u64 flags, u64 mode) {
        if path == null {
            i64 error = -1;
            return (u64)error;
        }
        if flags & 2048 || flags & 131072 || flags & 4194304 {
            i32 error = -1073741637;
            return (u64)(i64)error;
        }

        u64 path_length = 0;
        while path[path_length] != 0 {
            path_length += 1;
        }
        if path_length == 0 || path_length > 32760 {
            i64 error = -1;
            return (u64)error;
        }

        u64 buffer_size = path_length * 2 + 2;
        i8* utf16_path = (i8*)sys::alloc(buffer_size);
        if utf16_path == null {
            i64 error = -1;
            return (u64)error;
        }

        u32 utf16_bytes = 0;
        i32 status = RtlUTF8ToUnicodeN(
            (u16*)utf16_path, (u32)(path_length * 2), &utf16_bytes,
            path, (u32)path_length
        );
        if status < 0 {
            sys::free(utf16_path, buffer_size);
            return (u64)(i64)status;
        }
        utf16_path[utf16_bytes] = 0;
        utf16_path[utf16_bytes + 1] = 0;

        UNICODE_STRING nt_name;
        nt_name.Length = 0;
        nt_name.MaximumLength = 0;
        nt_name.Padding = 0;
        nt_name.Buffer = 0;

        i32 path_status = RtlDosPathNameToNtPathName_U(
            (u16*)utf16_path, &nt_name, null, null
        );
        if path_status == 0 {
            sys::free(utf16_path, buffer_size);
            i32 error = -1073741765;
            return (u64)(i64)error;
        }

        OBJECT_ATTRIBUTES oa;
        IO_STATUS_BLOCK iosb;
        u64 handle = 0;

        oa.Length = 48;
        oa.Padding0 = 0;
        oa.RootDirectory = 0;
        oa.ObjectName = (u64)&nt_name;
        oa.Attributes = 64;
        oa.Padding1 = 0;
        oa.SecurityDescriptor = 0;
        oa.SecurityQualityOfService = 0;

        u64 access_mode = flags & 3;
        if access_mode > 2 {
            RtlFreeUnicodeString(&nt_name);
            sys::free(utf16_path, buffer_size);
            i32 error = -1073741811;
            return (u64)(i64)error;
        }

        u32 desired_access = 0x00100000;
        if flags & 1024 {
            desired_access = desired_access | 4;
            if access_mode == 0 || access_mode == 2 {
                desired_access = desired_access | 0x80000000;
            }
        } else {
            if access_mode == 0 || access_mode == 2 {
                desired_access = desired_access | 0x80000000;
            }
            if access_mode == 1 || access_mode == 2 {
                desired_access = desired_access | 0x40000000;
            }
        }

        u32 disposition = 1;
        if flags & 64 {
            if flags & 128 {
                disposition = 2;
            } else {
                if flags & 512 {
                    disposition = 5;
                } else {
                    disposition = 3;
                }
            }
        } else {
            if flags & 512 {
                disposition = 4;
            }
        }

        u32 options = 0x20;
        if flags & 65536 { options = options | 1; }
        if flags & 4096 || flags & 1048576 { options = options | 2; }

        status = NtCreateFile(
            &handle, desired_access, &oa, &iosb,
            null, 0, 7, disposition, options, null, 0
        );
        RtlFreeUnicodeString(&nt_name);
        sys::free(utf16_path, buffer_size);

        if status >= 0 {
            return handle;
        }

        return (u64)(i64)status;
    }

    void close(u64 handle) {
        NtClose(handle);
    }

    void fstat(u64 handle, void* result_ptr) {
        Stat* result = (Stat*)result_ptr;
        FILE_STANDARD_INFORMATION info;
        IO_STATUS_BLOCK iosb;
        i32 status = NtQueryInformationFile(
            handle, &iosb, &info, 24, 5
        );
        if status < 0 { return; }

        result->st_dev = 0;
        result->st_ino = 0;
        result->st_nlink = info.NumberOfLinks;
        result->st_mode = 0;
        result->st_uid = 0;
        result->st_gid = 0;
        result->padding1 = 0;
        result->st_rdev = 0;
        result->st_size = (u64)info.EndOfFile;
        result->st_blksize = 4096;
        result->st_blocks = (result->st_size + 511) / 512;
        result->st_atime = 0;
        result->st_atime_nsec = 0;
        result->st_mtime = 0;
        result->st_mtime_nsec = 0;
        result->st_ctime = 0;
        result->st_ctime_nsec = 0;
        result->padding2 = 0;
        result->padding3 = 0;
        result->padding4 = 0;
    }

    void lseek(u64 handle, u64 offset, u64 whence) {
        FILE_POSITION_INFORMATION position;
        IO_STATUS_BLOCK iosb;

        if whence == 0 {
            position.CurrentByteOffset = (i64)offset;
        } else {
            if whence == 1 {
                i32 status = NtQueryInformationFile(
                    handle, &iosb, &position, 8, 14
                );
                if status < 0 { return; }
                position.CurrentByteOffset += (i64)offset;
            } else {
                if whence == 2 {
                    FILE_STANDARD_INFORMATION info;
                    i32 status = NtQueryInformationFile(
                        handle, &iosb, &info, 24, 5
                    );
                    if status < 0 { return; }
                    position.CurrentByteOffset = info.EndOfFile + (i64)offset;
                } else {
                    return;
                }
            }
        }

        NtSetInformationFile(handle, &iosb, &position, 8, 14);
    }

    u64 fcntl(u64 handle, u64 command, u64 flags) {
        i32 status = -1073741637;
        if command == 3 || command == 4 { return (u64)(i64)status; }
        status = -1073741811;
        return (u64)(i64)status;
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

    void free(void* addr,u64 len) {
        void* base = addr;
        u64 size = 0;

        NtFreeVirtualMemory(
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

    global void exit(u64 code) {
        NtTerminateProcess(-1, (i32)code);
    }

    i32 wait_handle(u64 handle, u64 alertable,LARGE_INTEGER* timeout) {
        return NtWaitForSingleObject(
            handle, (u32)alertable, timeout
        );
    }

    void nanosleep(u64 nanos) {
        if nanos == 0 { return; }

        LARGE_INTEGER interval;
        interval.QuadPart = -((i64)(nanos / 100 + (nanos % 100 != 0)));
        NtDelayExecution(0, &interval);
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