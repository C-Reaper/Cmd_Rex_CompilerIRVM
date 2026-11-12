
namespace ptr {
    void set(void* ptr,i8 value,i64 size){
        //i8* cptr = ptr;
        //i64 todo = size / 8 * 8;
        //i64 rest = size - todo;
        //
        //i64* dst64 = cptr;
        //i64 value64 = value;
        //i64 comp = value64 | (value64<<8) | (value64<<16) | (value64<<24) | (value64<<32) | (value64<<40) | (value64<<48) | (value64<<56);
        //
        //i64 i = 0;
        //for ,i<todo,i+=8 {
        //    *(dst64+i) = comp;
        //}
        //for ,i<size,i+=1 {
        //    cptr[i] = value;
        //}

        i8* cdst = ptr;
        for i64 i = 0,i<size,i+=1 {
            cdst[i] = value;
        }
    }
    void cpy(void* dst,void* src,i64 size){
        //i64 todo = (size / 8) * 8;
        //i64 rest = size - todo;
        //i64* dst64 = dst;
        //i64* src64 = src;
        //i64 index = 0;
        //for i64 i = 0,i<todo,i+=1 {
        //    dst64[i] = src64[i];
        //    index += 8;
        //}

        //i8* cdst = dst;
        //i8* csrc = src;
        //for i64 i = 0,i<size,i+=1 {
        //    cdst[index] = csrc[index];
        //    index += 1;
        //}

        i8* cdst = dst;
        i8* csrc = src;
        for i64 i = 0,i<size,i+=1 {
            cdst[i] = csrc[i];
        }
    }
    void mov(void* dst,void* src,i64 size){
        i8* cdst = dst;
        i8* csrc = src;
        
        i64 todo = size / 8 * 8;
        i64 rest = size - todo;
        
        i64* dst64 = dst;
        i64* src64 = src;
        i64 i = 0;
        for ,i<todo,i+=8 {
            *(dst64+i) = *(src64+i);
            *(src64+i) = 0;
        }
        for ,i<size,i+=1 {
            cdst[i] = csrc[i];
            csrc[i] = 0;
        }
    }
    void shl(void* dst,i64 count,i64 size){
        i8* cdst = dst;
        
        i64 todo = size / 8 * 8;
        i64 rest = size - todo;
        
        i8* src = (dst - count);
        i64* dst64 = src;
        i64* src64 = dst;
        i64 i = 0;
        for ,i<todo,i+=8 {
            *(dst64+i) = *(src64+i);
        }
        for ,i<size,i+=1 {
            cdst[i] = src[i];
        }
    }
    void shr(void* dst,i64 count,i64 size){
        i8* cdst = dst;
        
        i64 todo = size / 8 * 8;
        i64 rest = size - todo;
        
        i8* src = (dst + count);
        i64* dst64 = src;
        i64* src64 = dst;
        i64 i = size-1;
        for ,i>todo,i-=1 {
            cdst[i] = src[i];
        }
        for ,i>=0,i-=8 {
            *(dst64+i) = *(src64+i);
        }
    }
}
