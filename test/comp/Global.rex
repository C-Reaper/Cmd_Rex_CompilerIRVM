
i8 value0;
u64 value1;
i8* value2;
i8* value3;
i8[10] buffer;

global i64 main(){
    value2 = buffer;
    value3 = "Hello World!";
    return value0 + value1 + buffer[0] + value2[1] + value3[2];
}