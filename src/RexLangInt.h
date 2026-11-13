#ifndef REXLANGINT_H
#define REXLANGINT_H

#include "RexLangDefines.h"
#include "RexLangASM.h"

Token RexLang_Int_Int_Handler_Asd(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteAss5(ll,IRVM_OP_ADD,a,b);
}
Token RexLang_Int_Int_Handler_Asu(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteAss5(ll,IRVM_OP_SUB,a,b);
}
Token RexLang_Int_Int_Handler_Asm(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteAss5(ll,IRVM_OP_MUL,a,b);
}
Token RexLang_Int_Int_Handler_Asv(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteAss5(ll,IRVM_OP_DIV,a,b);
}
Token RexLang_Int_Int_Handler_Ass(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteAss(ll,a,b);
}

Token RexLang_Int_Int_Handler_Add(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_ADD,a,b);
}
Token RexLang_Int_Int_Handler_Sub(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_SUB,a,b);
}
Token RexLang_Int_Int_Handler_Mul(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_MUL,a,b);
}
Token RexLang_Int_Int_Handler_Div(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_DIV,a,b);
}
Token RexLang_Int_Int_Handler_Mod(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_MOD,a,b);
}
Token RexLang_Int_Handler_Neg(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_Execute4(ll,IRVM_OP_NEG,a);
}
Token RexLang_Int_Handler_Inc(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_ExecuteAss4(ll,IRVM_OP_INC,a);
}
Token RexLang_Int_Handler_Dec(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_ExecuteAss4(ll,IRVM_OP_DEC,a);
}

Token RexLang_Int_Int_Handler_And(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_AND,a,b);
}
Token RexLang_Int_Int_Handler_Or(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_OR,a,b);
}
Token RexLang_Int_Int_Handler_Xor(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_XOR,a,b);
}
Token RexLang_Int_Handler_Not(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_Execute4(ll,IRVM_OP_NOT,a);
}
Token RexLang_Int_Int_Handler_Shl(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_SHL,a,b);
}
Token RexLang_Int_Int_Handler_Shr(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_SHR,a,b);
}

Token RexLang_Int_Int_Handler_Lnd(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteLnd(ll,a,b);
}
Token RexLang_Int_Int_Handler_Lor(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteLor(ll,a,b);
}
Token RexLang_Int_Handler_Lot(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_ExecuteLot(ll,a);
}

Token RexLang_Int_Int_Handler_Equ(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteType5(ll,IRVM_TYPE_BOOL,IRVM_OP_EQ,a,b);
}
Token RexLang_Int_Int_Handler_Neq(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteType5(ll,IRVM_TYPE_BOOL,IRVM_OP_NQ,a,b);
}
Token RexLang_Int_Int_Handler_Les(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteType5(ll,IRVM_TYPE_BOOL,IRVM_OP_LT,a,b);
}
Token RexLang_Int_Int_Handler_Grt(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteType5(ll,IRVM_TYPE_BOOL,IRVM_OP_GT,a,b);
}
Token RexLang_Int_Int_Handler_Leq(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteType5(ll,IRVM_TYPE_BOOL,IRVM_OP_LQ,a,b);
}
Token RexLang_Int_Int_Handler_Grq(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteType5(ll,IRVM_TYPE_BOOL,IRVM_OP_GQ,a,b);
}

Token RexLang_Int_Handler_Adr(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_ExecuteAdr(ll,a);
}

Token RexLang_Int_Null_Handler_Cast(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    String ret = String_New();

    if(a->tt==TOKEN_NUMBER){
        String_AppendNumber(&ret,a->v_i64);
    }else if(a->tt==TOKEN_BOOL){
        String_AppendBoolean(&ret,a->v_b1);
    }else if(a->tt==TOKEN_FLOAT){
        String_AppendDouble(&ret,a->v_f64);
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
Token RexLang_Int_Handler_Cast(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_ExecuteCast(ll,op->str,a);
}

Token RexLang_Int_Handler_Size1(RexLang* ll,Token* op,Vector* args){
    return Token_New_I64(TOKEN_NUMBER,I8_SIZE);
}
Token RexLang_Int_Handler_Size2(RexLang* ll,Token* op,Vector* args){
    return Token_New_I64(TOKEN_NUMBER,I16_SIZE);
}
Token RexLang_Int_Handler_Size4(RexLang* ll,Token* op,Vector* args){
    return Token_New_I64(TOKEN_NUMBER,I32_SIZE);
}
Token RexLang_Int_Handler_Size8(RexLang* ll,Token* op,Vector* args){
    return Token_New_I64(TOKEN_NUMBER,I64_SIZE);
}

void RexLang_LoadIntType(RexLang* ll,CStr type){
    const Number size = IRVM_Impl_Size(type);
    
    TypeMap_Push(&ll->ev.sc.types,(Type[]){
        Type_Make(type,sizeof(RexLangVariable),OperatorInterationMap_Make((OperatorInterater[]){
            OperatorInterater_Make((CStr[]){ NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_NEG,(Token(*)(void*,Token*,Vector*))RexLang_Int_Handler_Neg),
                OperatorDefiner_New(TOKEN_REXLANG_INC,(Token(*)(void*,Token*,Vector*))RexLang_Int_Handler_Inc),
                OperatorDefiner_New(TOKEN_REXLANG_DEC,(Token(*)(void*,Token*,Vector*))RexLang_Int_Handler_Dec),
                OperatorDefiner_New(TOKEN_REXLANG_NOT,(Token(*)(void*,Token*,Vector*))RexLang_Int_Handler_Not),
                OperatorDefiner_New(TOKEN_REXLANG_LOT,(Token(*)(void*,Token*,Vector*))RexLang_Int_Handler_Lot),
                OperatorDefiner_New(TOKEN_REXLANG_ADR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Handler_Adr),
                OperatorDefiner_New(TOKEN_CAST,(Token(*)(void*,Token*,Vector*))RexLang_Int_Handler_Cast),
                OperatorDefiner_New(TOKEN_INIT,(Token(*)(void*,Token*,Vector*))RexLang_Init),
                OperatorDefiner_New(TOKEN_REXLANG_SIZE,
                    (Token(*)(void*,Token*,Vector*))(
                        size == 1 ? RexLang_Int_Handler_Size1 : (
                        size == 2 ? RexLang_Int_Handler_Size2 : (
                        size == 4 ? RexLang_Int_Handler_Size4 :
                        RexLang_Int_Handler_Size8))
                    )
                ),
                //OperatorDefiner_New(TOKEN_DESTROY,NULL),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ BOOL_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ASS,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Ass),
                OperatorDefiner_New(TOKEN_REXLANG_ASD,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asd),
                OperatorDefiner_New(TOKEN_REXLANG_ASU,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asu),
                OperatorDefiner_New(TOKEN_REXLANG_ASM,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asm),
                OperatorDefiner_New(TOKEN_REXLANG_ASV,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asv),
                OperatorDefiner_New(TOKEN_REXLANG_ADD,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Add),
                OperatorDefiner_New(TOKEN_REXLANG_SUB,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Sub),
                OperatorDefiner_New(TOKEN_REXLANG_MUL,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Mul),
                OperatorDefiner_New(TOKEN_REXLANG_DIV,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Div),
                OperatorDefiner_New(TOKEN_REXLANG_MOD,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Mod),
                OperatorDefiner_New(TOKEN_REXLANG_AND,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_And),
                OperatorDefiner_New(TOKEN_REXLANG_OR,(Token(*)(void*,Token*,Vector*)) RexLang_Int_Int_Handler_Or),
                OperatorDefiner_New(TOKEN_REXLANG_XOR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Xor),
                OperatorDefiner_New(TOKEN_REXLANG_LND,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Lnd),
                OperatorDefiner_New(TOKEN_REXLANG_LOR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Lor),
                OperatorDefiner_New(TOKEN_REXLANG_SHL,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shl),
                OperatorDefiner_New(TOKEN_REXLANG_SHR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shr),
                OperatorDefiner_New(TOKEN_REXLANG_SHL,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shl),
                OperatorDefiner_New(TOKEN_REXLANG_SHR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shr),
                OperatorDefiner_New(TOKEN_REXLANG_EQU,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Equ),
                OperatorDefiner_New(TOKEN_REXLANG_NEQ,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Neq),
                OperatorDefiner_New(TOKEN_REXLANG_LES,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Les),
                OperatorDefiner_New(TOKEN_REXLANG_GRT,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Grt),
                OperatorDefiner_New(TOKEN_REXLANG_LEQ,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Leq),
                OperatorDefiner_New(TOKEN_REXLANG_GRQ,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Grq),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ I8_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ASS,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Ass),
                OperatorDefiner_New(TOKEN_REXLANG_ASD,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asd),
                OperatorDefiner_New(TOKEN_REXLANG_ASU,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asu),
                OperatorDefiner_New(TOKEN_REXLANG_ASM,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asm),
                OperatorDefiner_New(TOKEN_REXLANG_ASV,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asv),
                OperatorDefiner_New(TOKEN_REXLANG_ADD,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Add),
                OperatorDefiner_New(TOKEN_REXLANG_SUB,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Sub),
                OperatorDefiner_New(TOKEN_REXLANG_MUL,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Mul),
                OperatorDefiner_New(TOKEN_REXLANG_DIV,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Div),
                OperatorDefiner_New(TOKEN_REXLANG_MOD,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Mod),
                OperatorDefiner_New(TOKEN_REXLANG_AND,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_And),
                OperatorDefiner_New(TOKEN_REXLANG_OR,(Token(*)(void*,Token*,Vector*)) RexLang_Int_Int_Handler_Or),
                OperatorDefiner_New(TOKEN_REXLANG_XOR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Xor),
                OperatorDefiner_New(TOKEN_REXLANG_LND,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Lnd),
                OperatorDefiner_New(TOKEN_REXLANG_LOR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Lor),
                OperatorDefiner_New(TOKEN_REXLANG_SHL,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shl),
                OperatorDefiner_New(TOKEN_REXLANG_SHR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shr),
                OperatorDefiner_New(TOKEN_REXLANG_SHL,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shl),
                OperatorDefiner_New(TOKEN_REXLANG_SHR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shr),
                OperatorDefiner_New(TOKEN_REXLANG_EQU,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Equ),
                OperatorDefiner_New(TOKEN_REXLANG_NEQ,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Neq),
                OperatorDefiner_New(TOKEN_REXLANG_LES,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Les),
                OperatorDefiner_New(TOKEN_REXLANG_GRT,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Grt),
                OperatorDefiner_New(TOKEN_REXLANG_LEQ,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Leq),
                OperatorDefiner_New(TOKEN_REXLANG_GRQ,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Grq),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ I16_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ASS,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Ass),
                OperatorDefiner_New(TOKEN_REXLANG_ASD,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asd),
                OperatorDefiner_New(TOKEN_REXLANG_ASU,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asu),
                OperatorDefiner_New(TOKEN_REXLANG_ASM,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asm),
                OperatorDefiner_New(TOKEN_REXLANG_ASV,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asv),
                OperatorDefiner_New(TOKEN_REXLANG_ADD,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Add),
                OperatorDefiner_New(TOKEN_REXLANG_SUB,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Sub),
                OperatorDefiner_New(TOKEN_REXLANG_MUL,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Mul),
                OperatorDefiner_New(TOKEN_REXLANG_DIV,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Div),
                OperatorDefiner_New(TOKEN_REXLANG_MOD,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Mod),
                OperatorDefiner_New(TOKEN_REXLANG_AND,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_And),
                OperatorDefiner_New(TOKEN_REXLANG_OR,(Token(*)(void*,Token*,Vector*)) RexLang_Int_Int_Handler_Or),
                OperatorDefiner_New(TOKEN_REXLANG_XOR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Xor),
                OperatorDefiner_New(TOKEN_REXLANG_LND,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Lnd),
                OperatorDefiner_New(TOKEN_REXLANG_LOR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Lor),
                OperatorDefiner_New(TOKEN_REXLANG_SHL,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shl),
                OperatorDefiner_New(TOKEN_REXLANG_SHR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shr),
                OperatorDefiner_New(TOKEN_REXLANG_SHL,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shl),
                OperatorDefiner_New(TOKEN_REXLANG_SHR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shr),
                OperatorDefiner_New(TOKEN_REXLANG_EQU,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Equ),
                OperatorDefiner_New(TOKEN_REXLANG_NEQ,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Neq),
                OperatorDefiner_New(TOKEN_REXLANG_LES,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Les),
                OperatorDefiner_New(TOKEN_REXLANG_GRT,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Grt),
                OperatorDefiner_New(TOKEN_REXLANG_LEQ,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Leq),
                OperatorDefiner_New(TOKEN_REXLANG_GRQ,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Grq),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ I32_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ASS,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Ass),
                OperatorDefiner_New(TOKEN_REXLANG_ASD,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asd),
                OperatorDefiner_New(TOKEN_REXLANG_ASU,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asu),
                OperatorDefiner_New(TOKEN_REXLANG_ASM,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asm),
                OperatorDefiner_New(TOKEN_REXLANG_ASV,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asv),
                OperatorDefiner_New(TOKEN_REXLANG_ADD,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Add),
                OperatorDefiner_New(TOKEN_REXLANG_SUB,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Sub),
                OperatorDefiner_New(TOKEN_REXLANG_MUL,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Mul),
                OperatorDefiner_New(TOKEN_REXLANG_DIV,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Div),
                OperatorDefiner_New(TOKEN_REXLANG_MOD,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Mod),
                OperatorDefiner_New(TOKEN_REXLANG_AND,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_And),
                OperatorDefiner_New(TOKEN_REXLANG_OR,(Token(*)(void*,Token*,Vector*)) RexLang_Int_Int_Handler_Or),
                OperatorDefiner_New(TOKEN_REXLANG_XOR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Xor),
                OperatorDefiner_New(TOKEN_REXLANG_LND,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Lnd),
                OperatorDefiner_New(TOKEN_REXLANG_LOR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Lor),
                OperatorDefiner_New(TOKEN_REXLANG_SHL,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shl),
                OperatorDefiner_New(TOKEN_REXLANG_SHR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shr),
                OperatorDefiner_New(TOKEN_REXLANG_SHL,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shl),
                OperatorDefiner_New(TOKEN_REXLANG_SHR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shr),
                OperatorDefiner_New(TOKEN_REXLANG_EQU,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Equ),
                OperatorDefiner_New(TOKEN_REXLANG_NEQ,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Neq),
                OperatorDefiner_New(TOKEN_REXLANG_LES,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Les),
                OperatorDefiner_New(TOKEN_REXLANG_GRT,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Grt),
                OperatorDefiner_New(TOKEN_REXLANG_LEQ,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Leq),
                OperatorDefiner_New(TOKEN_REXLANG_GRQ,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Grq),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ I64_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ASS,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Ass),
                OperatorDefiner_New(TOKEN_REXLANG_ASD,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asd),
                OperatorDefiner_New(TOKEN_REXLANG_ASU,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asu),
                OperatorDefiner_New(TOKEN_REXLANG_ASM,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asm),
                OperatorDefiner_New(TOKEN_REXLANG_ASV,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asv),
                OperatorDefiner_New(TOKEN_REXLANG_ADD,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Add),
                OperatorDefiner_New(TOKEN_REXLANG_SUB,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Sub),
                OperatorDefiner_New(TOKEN_REXLANG_MUL,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Mul),
                OperatorDefiner_New(TOKEN_REXLANG_DIV,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Div),
                OperatorDefiner_New(TOKEN_REXLANG_MOD,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Mod),
                OperatorDefiner_New(TOKEN_REXLANG_AND,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_And),
                OperatorDefiner_New(TOKEN_REXLANG_OR,(Token(*)(void*,Token*,Vector*)) RexLang_Int_Int_Handler_Or),
                OperatorDefiner_New(TOKEN_REXLANG_XOR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Xor),
                OperatorDefiner_New(TOKEN_REXLANG_LND,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Lnd),
                OperatorDefiner_New(TOKEN_REXLANG_LOR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Lor),
                OperatorDefiner_New(TOKEN_REXLANG_SHL,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shl),
                OperatorDefiner_New(TOKEN_REXLANG_SHR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shr),
                OperatorDefiner_New(TOKEN_REXLANG_SHL,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shl),
                OperatorDefiner_New(TOKEN_REXLANG_SHR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shr),
                OperatorDefiner_New(TOKEN_REXLANG_EQU,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Equ),
                OperatorDefiner_New(TOKEN_REXLANG_NEQ,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Neq),
                OperatorDefiner_New(TOKEN_REXLANG_LES,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Les),
                OperatorDefiner_New(TOKEN_REXLANG_GRT,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Grt),
                OperatorDefiner_New(TOKEN_REXLANG_LEQ,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Leq),
                OperatorDefiner_New(TOKEN_REXLANG_GRQ,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Grq),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ U8_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ASS,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Ass),
                OperatorDefiner_New(TOKEN_REXLANG_ASD,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asd),
                OperatorDefiner_New(TOKEN_REXLANG_ASU,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asu),
                OperatorDefiner_New(TOKEN_REXLANG_ASM,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asm),
                OperatorDefiner_New(TOKEN_REXLANG_ASV,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asv),
                OperatorDefiner_New(TOKEN_REXLANG_ADD,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Add),
                OperatorDefiner_New(TOKEN_REXLANG_SUB,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Sub),
                OperatorDefiner_New(TOKEN_REXLANG_MUL,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Mul),
                OperatorDefiner_New(TOKEN_REXLANG_DIV,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Div),
                OperatorDefiner_New(TOKEN_REXLANG_MOD,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Mod),
                OperatorDefiner_New(TOKEN_REXLANG_AND,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_And),
                OperatorDefiner_New(TOKEN_REXLANG_OR,(Token(*)(void*,Token*,Vector*)) RexLang_Int_Int_Handler_Or),
                OperatorDefiner_New(TOKEN_REXLANG_XOR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Xor),
                OperatorDefiner_New(TOKEN_REXLANG_LND,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Lnd),
                OperatorDefiner_New(TOKEN_REXLANG_LOR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Lor),
                OperatorDefiner_New(TOKEN_REXLANG_SHL,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shl),
                OperatorDefiner_New(TOKEN_REXLANG_SHR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shr),
                OperatorDefiner_New(TOKEN_REXLANG_SHL,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shl),
                OperatorDefiner_New(TOKEN_REXLANG_SHR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shr),
                OperatorDefiner_New(TOKEN_REXLANG_EQU,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Equ),
                OperatorDefiner_New(TOKEN_REXLANG_NEQ,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Neq),
                OperatorDefiner_New(TOKEN_REXLANG_LES,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Les),
                OperatorDefiner_New(TOKEN_REXLANG_GRT,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Grt),
                OperatorDefiner_New(TOKEN_REXLANG_LEQ,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Leq),
                OperatorDefiner_New(TOKEN_REXLANG_GRQ,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Grq),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ U16_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ASS,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Ass),
                OperatorDefiner_New(TOKEN_REXLANG_ASD,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asd),
                OperatorDefiner_New(TOKEN_REXLANG_ASU,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asu),
                OperatorDefiner_New(TOKEN_REXLANG_ASM,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asm),
                OperatorDefiner_New(TOKEN_REXLANG_ASV,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asv),
                OperatorDefiner_New(TOKEN_REXLANG_ADD,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Add),
                OperatorDefiner_New(TOKEN_REXLANG_SUB,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Sub),
                OperatorDefiner_New(TOKEN_REXLANG_MUL,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Mul),
                OperatorDefiner_New(TOKEN_REXLANG_DIV,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Div),
                OperatorDefiner_New(TOKEN_REXLANG_MOD,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Mod),
                OperatorDefiner_New(TOKEN_REXLANG_AND,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_And),
                OperatorDefiner_New(TOKEN_REXLANG_OR,(Token(*)(void*,Token*,Vector*)) RexLang_Int_Int_Handler_Or),
                OperatorDefiner_New(TOKEN_REXLANG_XOR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Xor),
                OperatorDefiner_New(TOKEN_REXLANG_LND,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Lnd),
                OperatorDefiner_New(TOKEN_REXLANG_LOR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Lor),
                OperatorDefiner_New(TOKEN_REXLANG_SHL,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shl),
                OperatorDefiner_New(TOKEN_REXLANG_SHR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shr),
                OperatorDefiner_New(TOKEN_REXLANG_SHL,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shl),
                OperatorDefiner_New(TOKEN_REXLANG_SHR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shr),
                OperatorDefiner_New(TOKEN_REXLANG_EQU,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Equ),
                OperatorDefiner_New(TOKEN_REXLANG_NEQ,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Neq),
                OperatorDefiner_New(TOKEN_REXLANG_LES,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Les),
                OperatorDefiner_New(TOKEN_REXLANG_GRT,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Grt),
                OperatorDefiner_New(TOKEN_REXLANG_LEQ,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Leq),
                OperatorDefiner_New(TOKEN_REXLANG_GRQ,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Grq),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ U32_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ASS,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Ass),
                OperatorDefiner_New(TOKEN_REXLANG_ASD,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asd),
                OperatorDefiner_New(TOKEN_REXLANG_ASU,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asu),
                OperatorDefiner_New(TOKEN_REXLANG_ASM,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asm),
                OperatorDefiner_New(TOKEN_REXLANG_ASV,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asv),
                OperatorDefiner_New(TOKEN_REXLANG_ADD,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Add),
                OperatorDefiner_New(TOKEN_REXLANG_SUB,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Sub),
                OperatorDefiner_New(TOKEN_REXLANG_MUL,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Mul),
                OperatorDefiner_New(TOKEN_REXLANG_DIV,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Div),
                OperatorDefiner_New(TOKEN_REXLANG_MOD,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Mod),
                OperatorDefiner_New(TOKEN_REXLANG_AND,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_And),
                OperatorDefiner_New(TOKEN_REXLANG_OR,(Token(*)(void*,Token*,Vector*)) RexLang_Int_Int_Handler_Or),
                OperatorDefiner_New(TOKEN_REXLANG_XOR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Xor),
                OperatorDefiner_New(TOKEN_REXLANG_LND,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Lnd),
                OperatorDefiner_New(TOKEN_REXLANG_LOR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Lor),
                OperatorDefiner_New(TOKEN_REXLANG_SHL,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shl),
                OperatorDefiner_New(TOKEN_REXLANG_SHR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shr),
                OperatorDefiner_New(TOKEN_REXLANG_SHL,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shl),
                OperatorDefiner_New(TOKEN_REXLANG_SHR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shr),
                OperatorDefiner_New(TOKEN_REXLANG_EQU,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Equ),
                OperatorDefiner_New(TOKEN_REXLANG_NEQ,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Neq),
                OperatorDefiner_New(TOKEN_REXLANG_LES,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Les),
                OperatorDefiner_New(TOKEN_REXLANG_GRT,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Grt),
                OperatorDefiner_New(TOKEN_REXLANG_LEQ,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Leq),
                OperatorDefiner_New(TOKEN_REXLANG_GRQ,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Grq),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ U64_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ASS,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Ass),
                OperatorDefiner_New(TOKEN_REXLANG_ASD,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asd),
                OperatorDefiner_New(TOKEN_REXLANG_ASU,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asu),
                OperatorDefiner_New(TOKEN_REXLANG_ASM,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asm),
                OperatorDefiner_New(TOKEN_REXLANG_ASV,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Asv),
                OperatorDefiner_New(TOKEN_REXLANG_ADD,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Add),
                OperatorDefiner_New(TOKEN_REXLANG_SUB,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Sub),
                OperatorDefiner_New(TOKEN_REXLANG_MUL,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Mul),
                OperatorDefiner_New(TOKEN_REXLANG_DIV,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Div),
                OperatorDefiner_New(TOKEN_REXLANG_MOD,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Mod),
                OperatorDefiner_New(TOKEN_REXLANG_AND,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_And),
                OperatorDefiner_New(TOKEN_REXLANG_OR,(Token(*)(void*,Token*,Vector*)) RexLang_Int_Int_Handler_Or),
                OperatorDefiner_New(TOKEN_REXLANG_XOR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Xor),
                OperatorDefiner_New(TOKEN_REXLANG_LND,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Lnd),
                OperatorDefiner_New(TOKEN_REXLANG_LOR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Lor),
                OperatorDefiner_New(TOKEN_REXLANG_SHL,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shl),
                OperatorDefiner_New(TOKEN_REXLANG_SHR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shr),
                OperatorDefiner_New(TOKEN_REXLANG_SHL,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shl),
                OperatorDefiner_New(TOKEN_REXLANG_SHR,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Shr),
                OperatorDefiner_New(TOKEN_REXLANG_EQU,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Equ),
                OperatorDefiner_New(TOKEN_REXLANG_NEQ,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Neq),
                OperatorDefiner_New(TOKEN_REXLANG_LES,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Les),
                OperatorDefiner_New(TOKEN_REXLANG_GRT,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Grt),
                OperatorDefiner_New(TOKEN_REXLANG_LEQ,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Leq),
                OperatorDefiner_New(TOKEN_REXLANG_GRQ,(Token(*)(void*,Token*,Vector*))RexLang_Int_Int_Handler_Grq),
                OPERATORDEFINER_END
            })),
            OPERATORINTERATER_END
        }),RexLang_Destroyer,RexLang_Cpyer)
    });
}

#endif //!REXLANGINT_H