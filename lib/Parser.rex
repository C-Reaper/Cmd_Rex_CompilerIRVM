import "../lib/CStr.rex";
import "../lib/Char.rex";
import "../lib/Math.rex";
import "../lib/IO.rex";

namespace parse {
    namespace boolean {
        void get(i8* out,bool b){
            if b==false {
                cstr::cpy(out,"false",5);
            }elif b==true {
                cstr::cpy(out,"true",4);
            }else{
                cstr::cpy(out,"none",4);
            }
        }
        bool by(i8* cstr){
            if cstr::cmp(cstr,"false") || cstr::cmp(cstr,"0") {
                return false;
            }
            if cstr::cmp(cstr,"true") || cstr::cmp(cstr,"1") {
                return true;
            }
            return false;
        }
        bool by_s(i8* cstr,bool* ptr){
            if ptr==null {
                return false;
            }
            if cstr::cmp(cstr,"false") || cstr::cmp(cstr,"0") {
                *ptr = false;
                return true;
            }
            if cstr::cmp(cstr,"true") || cstr::cmp(cstr,"1") {
                *ptr = true;
                return true;
            }
            return false;
        }
    }

    namespace uint {
        void get(i8* out,u64 num){
            if num==0 {
                *out = 48;
            }else{
                u64 size = math::uint::log10(num);
                for i64 i = size,i>=0,i-=1 {
                    u64 dig = num % 10;
                    out[i] = (dig + 48);
                    num /= 10;
                }
            }
        }
        u64 by(i8* cstr){
            i64 size = cstr::len(cstr);
            u64 out = 0;
            
            i64 startdigit = 0;
            i64 digits = 0;
        
            i64 i = size-1;
            while i>=0 {
                i8 c = *(cstr+i);
                if c>=48 && c<=57 {
                    out += (c-48) * math::uint::pow(10,digits);
                    digits+=1;
                    i-=1;
                }else{
                    return 0;
                }
            }
            return out;
        }
        bool by_s(i8* cstr,u64* ptr){
            if ptr==null {
                return false;
            }
            
            i64 size = cstr::len(cstr);
            u64 out = 0;
            
            i64 startdigit = 0;
            i64 digits = 0;
        
            i64 i = size-1;
            while i>=0 {
                i8 c = *(cstr+i);
                if c>=48 && c<=57 {
                    out += (c-48) * math::uint::pow(10,digits);
                    digits+=1;
                    i-=1;
                }else{
                    return false;
                }
            }
            *ptr = out;
            return true;
        }
    }
    
    namespace int {
        void get(i8* out,i64 num){
            if num==0 {
                *out = 48;
            }else{
                i64 minus = 0;
                if num<0 {
                    minus = 1;
                    out[0] = '-';
                    num = -num;
                }
                u64 size = math::uint::log10(num);
            
                for i64 i = minus+size,i>=minus,i-=1 {
                    u64 dig = num % 10;
                    *(out+i) = (dig + 48);
                    num /= 10;
                }
            }
        }
        i64 by(i8* cstr){
            i64 size = cstr::len(cstr);
            i64 out = 0;
            
            i64 startdigit = 0;
            i64 digits = 0;

            i64 sign = 0;
            if cstr[0]=='-' {
                sign = 1;
            }
        
            i64 i = size-1;
            while i>=sign {
                i8 c = *(cstr+i);
                if c>=48 && c<=57 {
                    out += (c-48) * math::uint::pow(10,digits);
                    digits+=1;
                    i-=1;
                }else{
                    return 0;
                }
            }
            if sign>0 {
                out = -out;
            }
            return out;
        }
        bool by_s(i8* cstr,i64* ptr){
            if ptr==null {
                return false;
            }
            
            i64 size = cstr::len(cstr);
            i64 out = 0;
            
            i64 startdigit = 0;
            i64 digits = 0;

            i64 sign = 0;
            if cstr[0]=='-' {
                sign = 1;
            }
        
            i64 i = size-1;
            while i>=sign {
                i8 c = *(cstr+i);
                if c>=48 && c<=57 {
                    out += (c-48) * math::uint::pow(10,digits);
                    digits+=1;
                    i-=1;
                }else{
                    return false;
                }
            }
            if sign>0 {
                out = -out;
            }
            *ptr = out;
            return true;
        }
    }

    namespace float {
        void get(i8* out,f64 n,u64 postdigits){ // f32 -> 7-9  |  f64 -> 15-17
            if n<0.0 && (i64)n==0 {
                *out = '-';
                out = out + 1;
            }
            parse::int::get(out,(i64)n);
            u64 len = cstr::len(out);
            *(out+len) = '.';
            len+=1;

            u64 max = 15 - math::uint::min(15,len-1);
            postdigits = math::uint::min(postdigits,max);

            n = n - (f64)((i64)n);
            if n<0.0 {
                n = -n;
            }
            n *= math::float::powi(10.0,postdigits);
            parse::uint::get(out + len,(i64)n);
        }
        f64 by(i8* cstr){
            u64 len = cstr::len(cstr);
            i64 dot = cstr::find(cstr,'.');
            if dot>=0 {
                *(cstr+dot) = '\0';
            }

            f64 out = (f64)parse::int::by(cstr);

            u64 max = math::uint::min(15,len) - dot; // MAXIMUS otherwise errors because of precision
            *(cstr+dot+1+max) = '\0';
            if dot>=0 {
                f64 comma = (f64)parse::int::by(cstr+dot+1) * math::float::powi(0.1,max);
                if out != 0.0 {
                    comma *= math::float::sign(out);
                }elif (*cstr)=='-' {
                    comma = -comma;
                }
                out += comma;
            }
            return out;
        }
        bool by_s(i8* cstr,f64* ptr){
            if ptr==null {
                return false;
            }
            
            u64 len = cstr::len(cstr);
            i64 dot = cstr::find(cstr,'.');
            if dot>=0 {
                *(cstr+dot) = '\0';
            }

            i64 n;
            if parse::int::by_s(cstr,&n)!=true {
                return false;
            }
            f64 out = (f64)n;

            u64 max = math::uint::min(15,len) - dot; // MAXIMUS otherwise errors because of precision
            *(cstr+dot+1+max) = '\0';
            if dot>=0 {
                if parse::int::by_s(cstr+dot+1,&n)!=true {
                    return false;
                }
                f64 comma = (f64)n * math::float::powi(0.1,max);
                if out != 0.0 {
                    comma *= math::float::sign(out);
                }elif (*cstr)=='-' {
                    comma = -comma;
                }
                out += comma;
            }
            *ptr = out;
            return true;
        }
    }
    
    namespace hex {
        void get(i8* out,u64 num){
            *(out) = '0';
            *(out+1) = 'x';
            if num==0 {
                out[2] = 48;
            }else{
                u64 size = math::uint::logb(16,num);
                for i64 i = size,i>=0,i-=1 {
                    u64 dig = num % 16;
                    *(out+i+2) = char::to::hex(dig);
                    num /= 16;
                }
            }
        }
        u64 by(i8* cstr){
            if (*cstr != '0' || *(cstr+1) != 'x') {
                return 0;
            }
            
            i64 size = cstr::len(cstr);
            u64 out = 0;
            
            i64 startdigit = 0;
            i64 digits = 0;
        
            i64 i = size-1;
            while i>=2 {
                i8 c = *(cstr+i);
                i64 num = char::by::hex(c);
                if num>=0 {
                    out += num * math::uint::pow(16,digits);
                    digits+=1;
                    i-=1;
                }else{
                    return 0;
                }
            }
            return out;
        }
        bool by_s(i8* cstr,u64* ptr){
            if ptr==null {
                return false;
            }
            if (*cstr != '0' || *(cstr+1) != 'x') {
                return false;
            }
            
            i64 size = cstr::len(cstr);
            u64 out = 0;
            
            i64 startdigit = 0;
            i64 digits = 0;
        
            i64 i = size-1;
            while i>=2 {
                i8 c = *(cstr+i);
                i64 num = char::by::hex(c);
                if num>=0 {
                    out += num * math::uint::pow(16,digits);
                    digits+=1;
                    i-=1;
                }else{
                    return false;
                }
            }
            *ptr = out;
            return true;
        }
    }

    namespace oct {
        void get(i8* out,u64 num){
            *(out) = '0';
            *(out+1) = 'o';
            if num==0 {
                out[2] = 48;
            }else{
                u64 size = math::uint::logb(8,num);
                for i64 i = size,i>=0,i-=1 {
                    u64 dig = num % 8;
                    *(out+i+2) = char::to::oct(dig);
                    num /= 8;
                }
            }
        }
        u64 by(i8* cstr){
            if (*cstr != '0' || *(cstr+1) != 'o') {
                return 0;
            }
            
            i64 size = cstr::len(cstr);
            u64 out = 0;
            
            i64 startdigit = 0;
            i64 digits = 0;
        
            i64 i = size-1;
            while i>=2 {
                i8 c = *(cstr+i);
                i64 num = char::by::oct(c);
                if num>=0 {
                    out += num * math::uint::pow(8,digits);
                    digits+=1;
                    i-=1;
                }else{
                    return 0;
                }
            }
            return out;
        }
        bool by_s(i8* cstr,u64* ptr){
            if ptr==null {
                return false;
            }
            if (*cstr != '0' || *(cstr+1) != 'o') {
                return false;
            }
            
            i64 size = cstr::len(cstr);
            u64 out = 0;
            
            i64 startdigit = 0;
            i64 digits = 0;
        
            i64 i = size-1;
            while i>=2 {
                i8 c = *(cstr+i);
                i64 num = char::by::oct(c);
                if num>=0 {
                    out += num * math::uint::pow(8,digits);
                    digits+=1;
                    i-=1;
                }else{
                    return false;
                }
            }
            *ptr = out;
            return true;
        }
    }

    namespace bin {
        void get(i8* out,u64 num){
            *(out) = '0';
            *(out+1) = 'b';
            if num==0 {
                out[2] = 48;
            }else{
                u64 size = math::uint::logb(2,num);
                for i64 i = size,i>=0,i-=1 {
                    u64 dig = num % 2;
                    *(out+i+2) = char::to::bin(dig);
                    num /= 2;
                }
            }
        }
        u64 by(i8* cstr){
            if (*cstr != '0' || *(cstr+1) != 'b') {
                return 0;
            }
            
            i64 size = cstr::len(cstr);
            u64 out = 0;
            
            i64 startdigit = 0;
            i64 digits = 0;
        
            i64 i = size-1;
            while i>=2 {
                i8 c = *(cstr+i);
                i64 num = char::by::bin(c);
                if num>=0 {
                    out += num * math::uint::pow(2,digits);
                    digits+=1;
                    i-=1;
                }else{
                    return 0;
                }
            }
            return out;
        }
        bool by_s(i8* cstr,u64* ptr){
            if ptr==null {
                return false;
            }
            if (*cstr != '0' || *(cstr+1) != 'b') {
                return false;
            }
            
            i64 size = cstr::len(cstr);
            u64 out = 0;
            
            i64 startdigit = 0;
            i64 digits = 0;
        
            i64 i = size-1;
            while i>=2 {
                i8 c = *(cstr+i);
                i64 num = char::by::bin(c);
                if num>=0 {
                    out += num * math::uint::pow(2,digits);
                    digits+=1;
                    i-=1;
                }else{
                    return false;
                }
            }
            *ptr = out;
            return true;
        }
    }
}