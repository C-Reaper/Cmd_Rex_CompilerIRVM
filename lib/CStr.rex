
namespace cstr {
    i64 len(i8* str){
        i64 size = 0;
        while str[size] != 0 {
            size = size + 1;
        }
        return size;
    }
    void set(i8* ptr,i8 value,i64 size){
        i64 i = 0;
        while i<size {
            ptr[i] = value;
            i = i + 1;
        }
    }
    void cpy(i8* dst,i8* src,i64 size){
        i64 i = 0;
        while i<size {
            dst[i] = src[i];
            i = i + 1;
        }
    }
    void mov(i8* dst,i8* src,i64 size){
        for i64 i = 0,i<size,i+=1 {
            dst[i] = src[i];
            src[i] = 0;
        }
    }

    i8 cmp(i8* cstr1,i8* cstr2){
        i64 size1 = cstr::len(cstr1);
        i64 size2 = cstr::len(cstr2);
        
        if size1 != size2 {
            return false;
        }
        for i64 i = 0,i<size1,i+=1 {
            if cstr1[i]!=cstr2[i] {
                return false;
            }
        }
        return true;
    }
    
    i64 find(i8* cstr,i8 ch){
        i64 size1 = cstr::len(cstr);
        
        for i64 i = 0,i<size1,i+=1 {
            if cstr[i] == ch {
                return i;
            }
        }
        
        return size1;
    }
    i64 findc(i8* src,i8* paddern){
        i64 psize = cstr::len(paddern);
        i64 ssize = cstr::len(src);

        for i64 i = 0,i<ssize,i+=1 {
            i8 ch = src[i];
            for i64 j = 0,j<psize,j+=1 {
                if ch==paddern[j] {
                    return i;
                }
            }
        }
        return ssize;
    }
    i64 findnot(i8* src,i8* paddern){
        u64 psize = cstr::len(paddern);
        u64 ssize = cstr::len(src);

        for u64 i = 0,i<ssize,i+=1 {
            i8 ch = src[i];
            
            for u64 j = 0,j<psize,j+=1 {
                if ch==paddern[j] {
                    j = psize;
                }
                if j==psize-1 {
                    return i;
                }
            }
        }
        return ssize;
    }
}