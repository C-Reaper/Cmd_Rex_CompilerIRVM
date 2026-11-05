#ifndef REXLANGINT_H
#define REXLANGINT_H

#include "RexLangDefines.h"
#include "RexLangASM.h"

Token Int_Int_Handler_Asd(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteAss5(ll,IRVM_OP_ADD,a,b);
}
Token Int_Int_Handler_Asu(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteAss5(ll,IRVM_OP_SUB,a,b);
}
Token Int_Int_Handler_Asm(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteAss5(ll,IRVM_OP_MUL,a,b);
}
Token Int_Int_Handler_Asv(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteAss5(ll,IRVM_OP_DIV,a,b);
}
Token Int_Int_Handler_Ass(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteAss(ll,a,b);
}

Token Int_Int_Handler_Add(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_ADD,a,b);
}
Token Int_Int_Handler_Sub(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_SUB,a,b);
}
Token Int_Int_Handler_Mul(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_MUL,a,b);
}
Token Int_Int_Handler_Div(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_DIV,a,b);
}
Token Int_Int_Handler_Mod(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_MOD,a,b);
}
Token Int_Handler_Neg(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_Execute4(ll,IRVM_OP_NEG,a);
}
Token Int_Handler_Inc(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_ExecuteAss4(ll,IRVM_OP_INC,a);
}
Token Int_Handler_Dec(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_ExecuteAss4(ll,IRVM_OP_DEC,a);
}

Token Int_Int_Handler_And(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_AND,a,b);
}
Token Int_Int_Handler_Or(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_OR,a,b);
}
Token Int_Int_Handler_Xor(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_XOR,a,b);
}
Token Int_Handler_Not(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_Execute4(ll,IRVM_OP_NOT,a);
}
Token Int_Int_Handler_Shl(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_SHL,a,b);
}
Token Int_Int_Handler_Shr(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_SHR,a,b);
}

Token Int_Int_Handler_Lnd(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteLnd(ll,a,b);
}
Token Int_Int_Handler_Lor(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteLor(ll,a,b);
}
Token Int_Handler_Lot(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_ExecuteLot(ll,a);
}

Token Int_Int_Handler_Equ(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_EQ,a,b);
}
Token Int_Int_Handler_Neq(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_NQ,a,b);
}
Token Int_Int_Handler_Les(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_LT,a,b);
}
Token Int_Int_Handler_Grt(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_GT,a,b);
}
Token Int_Int_Handler_Leq(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_LQ,a,b);
}
Token Int_Int_Handler_Grq(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_GQ,a,b);
}

Token Int_Handler_Adr(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_ExecuteAdr(ll,a);
}

Token Int_Null_Handler_Cast(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    String ret = String_New();

    if(a->tt==TOKEN_NUMBER){
        String_AppendNumber(&ret,a->v_i64);
    }else{
        Variable* v = Scope_FindVariable(&ll->ev.sc,a->str);
        if(v){
            RexLangVariable* sv = (RexLangVariable*)Variable_Data(v);
            String_Appendf(&ret,"T: %s [&:%d]",v->typename,v->destroy);
        }else{
            Environment_ErrorHandler(&ll->ev,"Cast -> Error!");
            String_Append(&ret,"ERROR");
        }
    }

    String_AppendChar(&ret,'\0');
    return Token_Move(TOKEN_CONSTSTRING_DOUBLE,(CStr)ret.Memory);
}
Token Int_Handler_Cast(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_ExecuteCast(ll,op->str,a);
}

Token Int_Handler_Size1(RexLang* ll,Token* op,Vector* args){
    return Token_New_I64(TOKEN_NUMBER,I8_SIZE);
}
Token Int_Handler_Size2(RexLang* ll,Token* op,Vector* args){
    return Token_New_I64(TOKEN_NUMBER,I16_SIZE);
}
Token Int_Handler_Size4(RexLang* ll,Token* op,Vector* args){
    return Token_New_I64(TOKEN_NUMBER,I32_SIZE);
}
Token Int_Handler_Size8(RexLang* ll,Token* op,Vector* args){
    return Token_New_I64(TOKEN_NUMBER,I64_SIZE);
}

#endif //!REXLANGINT_H