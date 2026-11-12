
namespace cstr {
    i32 len(i8* str){
        i32 size = 0;
        while str[size] != 0 {
            size = size + 1;
        }
        return size;
    }
    void set(i8* ptr,i8 value,i32 size){
        i32 i = 0;
        while i<size {
            ptr[i] = value;
            i = i + 1;
        }
    }
    void cpy(i8* dst,i8* src,i32 size){
        i32 i = 0;
        while i<size {
            dst[i] = src[i];
            i = i + 1;
        }
    }
    void mov(i8* dst,i8* src,i32 size){
        for i32 i = 0,i<size,i+=1 {
            dst[i] = src[i];
            src[i] = 0;
        }
    }

    i8 cmp(i8* cstr1,i8* cstr2){
        i32 size1 = cstr::len(cstr1);
        i32 size2 = cstr::len(cstr2);
        
        if size1 != size2 {
            return false;
        }
        for i32 i = 0,i<size1,i+=1 {
            if cstr1[i]!=cstr2[i] {
                return false;
            }
        }
        return true;
    }
    
    i32 find(i8* cstr,i8 ch){
        i32 size1 = cstr::len(cstr);
        
        for i32 i = 0,i<size1,i+=1 {
            if cstr[i] == ch {
                return i;
            }
        }
        
        return size1;
    }
    i32 findc(i8* src,i8* paddern){
        i32 psize = cstr::len(paddern);
        i32 ssize = cstr::len(src);

        for i32 i = 0,i<ssize,i+=1 {
            i8 ch = src[i];
            for i32 j = 0,j<psize,j+=1 {
                if ch==paddern[j] {
                    return i;
                }
            }
        }
        return ssize;
    }
    i32 findnot(i8* src,i8* paddern){
        u32 psize = cstr::len(paddern);
        u32 ssize = cstr::len(src);

        for u32 i = 0,i<ssize,i+=1 {
            i8 ch = src[i];
            
            for u32 j = 0,j<psize,j+=1 {
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