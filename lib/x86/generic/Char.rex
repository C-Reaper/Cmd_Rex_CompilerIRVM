
namespace char {
    bool alpha(i8 ch){
        return (ch>=65 && ch<91) || (ch>=97 && ch<123); 
    }
    bool num(i8 ch){
        return ch>=48 && ch<58; 
    }
    bool alphanum(i8 ch){
        return char::alpha(ch) || char::num(ch);
    }
    
    namespace to {
        i8 dec(i32 num){
            if num>=0 && num<10 {
                return num + 48;
            }
            return 0; 
        }
        i8 hex(i32 num){
            if num>=0 && num<10 {
                return num + 48;
            }
            if num>=10 && num<16 {
                return num + 55;
            }
            return 0; 
        }
        i8 oct(i32 num){
            if num>=0 && num<8 {
                return num + 48;
            }
            return 0; 
        }
        i8 bin(i32 num){
            if num>=0 && num<2 {
                return num + 48;
            }
            return 0; 
        }
    }

    namespace by {
        i32 dec(i8 ch){
            if ch>=48 && ch<58 {
                return (ch - 48);
            }
            return 0; 
        }
        i32 hex(i8 ch){
            if ch>=48 && ch<58 {
                return (ch - 48);
            }
            if ch>=65 && ch<71 {
                return (ch - 55);
            }
            if ch>=97 && ch<103 {
                return (ch - 87);
            }
            return 0; 
        }
        i32 oct(i8 ch){
            if ch>=48 && ch<56 {
                return (ch - 48);
            }
            return 0; 
        }
        i32 bin(i8 ch){
            if ch>=48 && ch<50 {
                return (ch - 48);
            }
            return 0; 
        }
    }
}