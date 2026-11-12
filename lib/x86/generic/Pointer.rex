
namespace ptr {
    void set(void* ptr,i8 value,i32 size){
        //i8* cptr = ptr;
        //i32 todo = size / 8 * 8;
        //i32 rest = size - todo;
        //
        //i32* dst64 = cptr;
        //i32 value64 = value;
        //i32 comp = value64 | (value64<<8) | (value64<<16) | (value64<<24) | (value64<<32) | (value64<<40) | (value64<<48) | (value64<<56);
        //
        //i32 i = 0;
        //for ,i<todo,i+=8 {
        //    *(dst64+i) = comp;
        //}
        //for ,i<size,i+=1 {
        //    cptr[i] = value;
        //}

        i8* cdst = ptr;
        for i32 i = 0,i<size,i+=1 {
            cdst[i] = value;
        }
    }
    void cpy(void* dst,void* src,i32 size){
        //i32 todo = (size / 8) * 8;
        //i32 rest = size - todo;
        //i32* dst64 = dst;
        //i32* src64 = src;
        //i32 index = 0;
        //for i32 i = 0,i<todo,i+=1 {
        //    dst64[i] = src64[i];
        //    index += 8;
        //}

        //i8* cdst = dst;
        //i8* csrc = src;
        //for i32 i = 0,i<size,i+=1 {
        //    cdst[index] = csrc[index];
        //    index += 1;
        //}

        i8* cdst = dst;
        i8* csrc = src;
        for i32 i = 0,i<size,i+=1 {
            cdst[i] = csrc[i];
        }
    }
    void mov(void* dst,void* src,i32 size){
        i8* cdst = dst;
        i8* csrc = src;
        
        i32 todo = size / 8 * 8;
        i32 rest = size - todo;
        
        i32* dst64 = dst;
        i32* src64 = src;
        i32 i = 0;
        for ,i<todo,i+=8 {
            *(dst64+i) = *(src64+i);
            *(src64+i) = 0;
        }
        for ,i<size,i+=1 {
            cdst[i] = csrc[i];
            csrc[i] = 0;
        }
    }
    void shl(void* dst,i32 count,i32 size){
        i8* cdst = dst;
        
        i32 todo = size / 8 * 8;
        i32 rest = size - todo;
        
        i8* src = (dst - count);
        i32* dst64 = src;
        i32* src64 = dst;
        i32 i = 0;
        for ,i<todo,i+=8 {
            *(dst64+i) = *(src64+i);
        }
        for ,i<size,i+=1 {
            cdst[i] = src[i];
        }
    }
    void shr(void* dst,i32 count,i32 size){
        i8* cdst = dst;
        
        i32 todo = size / 8 * 8;
        i32 rest = size - todo;
        
        i8* src = (dst + count);
        i32* dst64 = src;
        i32* src64 = dst;
        i32 i = size-1;
        for ,i>todo,i-=1 {
            cdst[i] = src[i];
        }
        for ,i>=0,i-=8 {
            *(dst64+i) = *(src64+i);
        }
    }
}
