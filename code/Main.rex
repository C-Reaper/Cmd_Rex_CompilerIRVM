// TOKEN_IRVM_ALLOCA
// TOKEN_IRVM_DESTROY
// TOKEN_IRVM_LOAD
// TOKEN_IRVM_STORE
// TOKEN_IRVM_ADR
// TOKEN_IRVM_ASS
// TOKEN_IRVM_ADD
// TOKEN_IRVM_SUB
// TOKEN_IRVM_MUL
// TOKEN_IRVM_DIV
// TOKEN_IRVM_NEG
// TOKEN_IRVM_INC
// TOKEN_IRVM_DEC
// TOKEN_IRVM_NOT
// TOKEN_IRVM_AND
// TOKEN_IRVM_OR
// TOKEN_IRVM_XOR
// TOKEN_IRVM_SHL
// TOKEN_IRVM_SHR
// TOKEN_IRVM_CALL
// TOKEN_IRVM_RET
// TOKEN_IRVM_TAR
// TOKEN_IRVM_JMP
// TOKEN_IRVM_BRC
// TOKEN_IRVM_PHI
// TOKEN_IRVM_EQ
// TOKEN_IRVM_NQ
// TOKEN_IRVM_LT
// TOKEN_IRVM_GT
// TOKEN_IRVM_LQ
// TOKEN_IRVM_GQ

extern i64 fn_ext(i64 a,i64 b);

global i64 fn_add(i64 a,i64 b){
    i64 c = a + b;
    i64 d = a - b;
    i64 e = a * b;
    i64 f = a / b;
    i64 g = a + b;
    i64 h = a + b;
    i64 i = a + b;
    return c;
}

global i64 main(){
    i64 a = fn_add(1,2);
    return a;
}