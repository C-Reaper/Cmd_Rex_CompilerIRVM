#include "/home/codeleaded/System/Static/Library/AlxCallStack.h"
#include "/home/codeleaded/System/Static/Library/AlxExternFunctions.h"

#include "../src/RexLang.h"
#include "../src/RexLangASM.h"

Token U16_U16_Handler_Ass(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteAss(ll,a,b);
}

Token U16_U16_Handler_Asd(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteAss5(ll,IRVM_OP_ADD,a,b);
}
Token U16_U16_Handler_Asu(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteAss5(ll,IRVM_OP_SUB,a,b);
}
Token U16_U16_Handler_Asm(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteAss5(ll,IRVM_OP_MUL,a,b);
}
Token U16_U16_Handler_Asv(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteAss5(ll,IRVM_OP_DIV,a,b);
}

Token U16_U16_Handler_Add(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_ADD,a,b);
}
Token U16_U16_Handler_Sub(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_SUB,a,b);
}
Token U16_U16_Handler_Mul(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_MUL,a,b);
}
Token U16_U16_Handler_Div(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_DIV,a,b);
}
Token U16_U16_Handler_Mod(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    //return RexLang_Execute5(ll,IRVM_OP_MOD,a,b);
    return Token_Null();
}

Token U16_Handler_Neg(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_Execute4(ll,IRVM_OP_NEG,a);
}
Token U16_Handler_Inc(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_Execute4(ll,IRVM_OP_INC,a);
}
Token U16_Handler_Dec(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_Execute4(ll,IRVM_OP_DEC,a);
}

Token U16_U16_Handler_And(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_AND,a,b);
}
Token U16_U16_Handler_Or(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_OR,a,b);
}
Token U16_U16_Handler_Xor(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_XOR,a,b);
}
Token U16_Handler_Not(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_Execute4(ll,IRVM_OP_NOT,a);
}
Token U16_U16_Handler_Shl(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_SHL,a,b);
}
Token U16_U16_Handler_Shr(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_SHR,a,b);
}

Token U16_U16_Handler_Equ(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_EQ,a,b);
}
Token U16_U16_Handler_Neq(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_NQ,a,b);
}
Token U16_U16_Handler_Les(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_LT,a,b);
}
Token U16_U16_Handler_Grt(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_GT,a,b);
}
Token U16_U16_Handler_Leq(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_LQ,a,b);
}
Token U16_U16_Handler_Grq(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_GQ,a,b);
}
Token U16_Handler_Adr(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_ExecuteAdr(ll,a);
}

Token U16_Null_Handler_Cast(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);

    //printf("[U16]: CAST: %s\n",a->str);

    String ret = String_New();

    if(a->tt==TOKEN_NUMBER){
        String_AppendNumber(&ret,a->v_i64);
    }else{
        Variable* v = Scope_FindVariable(&ll->ev.sc,a->str);
        if(v){
            RexLangVariable* sv = (RexLangVariable*)Variable_Data(v);
            String_Appendf(&ret,"T: %s [&:%d]",v->typename,sv->destroy);
        }else{
            Environment_ErrorHandler(&ll->ev,"Cast -> Error!");
            String_Append(&ret,"ERROR");
        }
    }

    CStr out = String_CStr(&ret);
    String_Free(&ret);
    return Token_Move(TOKEN_CONSTSTRING_DOUBLE,out);
}
Token U16_Handler_Cast(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_ExecuteCast(ll,op->str,a);
}
Token U16_Handler_Size(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    //printf("[U16]: SIZE: %s\n",a->str);
    return Token_New_I64(TOKEN_NUMBER,U16_SIZE);
}

void Ex_Packer(ExternFunctionMap* Extern_Functions,Vector* funcs,Scope* s){//Vector<CStr>
    TypeMap_PushContained(&s->types,funcs,
        Type_Make(U16_TYPE,sizeof(RexLangVariable),OperatorInterationMap_Make((OperatorInterater[]){
            OperatorInterater_Make((CStr[]){ NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_NEG,(Token(*)(void*,Token*,Vector*))U16_Handler_Neg),
                OperatorDefiner_New(TOKEN_REXLANG_INC,(Token(*)(void*,Token*,Vector*))U16_Handler_Inc),
                OperatorDefiner_New(TOKEN_REXLANG_DEC,(Token(*)(void*,Token*,Vector*))U16_Handler_Dec),
                OperatorDefiner_New(TOKEN_REXLANG_NOT,(Token(*)(void*,Token*,Vector*))U16_Handler_Not),
                OperatorDefiner_New(TOKEN_REXLANG_ADR,(Token(*)(void*,Token*,Vector*))U16_Handler_Adr),
                OperatorDefiner_New(TOKEN_CAST,(Token(*)(void*,Token*,Vector*))U16_Handler_Cast),
                OperatorDefiner_New(TOKEN_REXLANG_SIZE,(Token(*)(void*,Token*,Vector*))U16_Handler_Size),
                //OperatorDefiner_New(TOKEN_DESTROY,NULL),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ U8_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ASS,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Ass),
                OperatorDefiner_New(TOKEN_REXLANG_ASD,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Asd),
                OperatorDefiner_New(TOKEN_REXLANG_ASU,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Asu),
                OperatorDefiner_New(TOKEN_REXLANG_ASM,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Asm),
                OperatorDefiner_New(TOKEN_REXLANG_ASV,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Asv),
                OperatorDefiner_New(TOKEN_REXLANG_ADD,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Add),
                OperatorDefiner_New(TOKEN_REXLANG_SUB,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Sub),
                OperatorDefiner_New(TOKEN_REXLANG_MUL,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Mul),
                OperatorDefiner_New(TOKEN_REXLANG_DIV,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Div),
                OperatorDefiner_New(TOKEN_REXLANG_MOD,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Mod),
                OperatorDefiner_New(TOKEN_REXLANG_AND,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_And),
                OperatorDefiner_New(TOKEN_REXLANG_OR,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Or),
                OperatorDefiner_New(TOKEN_REXLANG_XOR,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Xor),
                OperatorDefiner_New(TOKEN_REXLANG_SHL,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Shl),
                OperatorDefiner_New(TOKEN_REXLANG_SHR,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Shr),
                OperatorDefiner_New(TOKEN_REXLANG_EQU,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Equ),
                OperatorDefiner_New(TOKEN_REXLANG_NEQ,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Neq),
                OperatorDefiner_New(TOKEN_REXLANG_LES,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Les),
                OperatorDefiner_New(TOKEN_REXLANG_GRT,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Grt),
                OperatorDefiner_New(TOKEN_REXLANG_LEQ,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Leq),
                OperatorDefiner_New(TOKEN_REXLANG_GRQ,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Grq),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ U16_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ASS,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Ass),
                OperatorDefiner_New(TOKEN_REXLANG_ASD,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Asd),
                OperatorDefiner_New(TOKEN_REXLANG_ASU,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Asu),
                OperatorDefiner_New(TOKEN_REXLANG_ASM,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Asm),
                OperatorDefiner_New(TOKEN_REXLANG_ASV,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Asv),
                OperatorDefiner_New(TOKEN_REXLANG_ADD,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Add),
                OperatorDefiner_New(TOKEN_REXLANG_SUB,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Sub),
                OperatorDefiner_New(TOKEN_REXLANG_MUL,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Mul),
                OperatorDefiner_New(TOKEN_REXLANG_DIV,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Div),
                OperatorDefiner_New(TOKEN_REXLANG_MOD,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Mod),
                OperatorDefiner_New(TOKEN_REXLANG_AND,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_And),
                OperatorDefiner_New(TOKEN_REXLANG_OR,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Or),
                OperatorDefiner_New(TOKEN_REXLANG_XOR,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Xor),
                OperatorDefiner_New(TOKEN_REXLANG_SHL,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Shl),
                OperatorDefiner_New(TOKEN_REXLANG_SHR,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Shr),
                OperatorDefiner_New(TOKEN_REXLANG_EQU,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Equ),
                OperatorDefiner_New(TOKEN_REXLANG_NEQ,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Neq),
                OperatorDefiner_New(TOKEN_REXLANG_LES,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Les),
                OperatorDefiner_New(TOKEN_REXLANG_GRT,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Grt),
                OperatorDefiner_New(TOKEN_REXLANG_LEQ,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Leq),
                OperatorDefiner_New(TOKEN_REXLANG_GRQ,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Grq),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ U32_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ASS,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Ass),
                OperatorDefiner_New(TOKEN_REXLANG_ASD,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Asd),
                OperatorDefiner_New(TOKEN_REXLANG_ASU,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Asu),
                OperatorDefiner_New(TOKEN_REXLANG_ASM,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Asm),
                OperatorDefiner_New(TOKEN_REXLANG_ASV,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Asv),
                OperatorDefiner_New(TOKEN_REXLANG_ADD,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Add),
                OperatorDefiner_New(TOKEN_REXLANG_SUB,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Sub),
                OperatorDefiner_New(TOKEN_REXLANG_MUL,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Mul),
                OperatorDefiner_New(TOKEN_REXLANG_DIV,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Div),
                OperatorDefiner_New(TOKEN_REXLANG_MOD,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Mod),
                OperatorDefiner_New(TOKEN_REXLANG_AND,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_And),
                OperatorDefiner_New(TOKEN_REXLANG_OR,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Or),
                OperatorDefiner_New(TOKEN_REXLANG_XOR,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Xor),
                OperatorDefiner_New(TOKEN_REXLANG_SHL,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Shl),
                OperatorDefiner_New(TOKEN_REXLANG_SHR,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Shr),
                OperatorDefiner_New(TOKEN_REXLANG_EQU,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Equ),
                OperatorDefiner_New(TOKEN_REXLANG_NEQ,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Neq),
                OperatorDefiner_New(TOKEN_REXLANG_LES,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Les),
                OperatorDefiner_New(TOKEN_REXLANG_GRT,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Grt),
                OperatorDefiner_New(TOKEN_REXLANG_LEQ,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Leq),
                OperatorDefiner_New(TOKEN_REXLANG_GRQ,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Grq),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ U64_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ASS,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Ass),
                OperatorDefiner_New(TOKEN_REXLANG_ASD,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Asd),
                OperatorDefiner_New(TOKEN_REXLANG_ASU,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Asu),
                OperatorDefiner_New(TOKEN_REXLANG_ASM,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Asm),
                OperatorDefiner_New(TOKEN_REXLANG_ASV,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Asv),
                OperatorDefiner_New(TOKEN_REXLANG_ADD,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Add),
                OperatorDefiner_New(TOKEN_REXLANG_SUB,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Sub),
                OperatorDefiner_New(TOKEN_REXLANG_MUL,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Mul),
                OperatorDefiner_New(TOKEN_REXLANG_DIV,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Div),
                OperatorDefiner_New(TOKEN_REXLANG_MOD,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Mod),
                OperatorDefiner_New(TOKEN_REXLANG_AND,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_And),
                OperatorDefiner_New(TOKEN_REXLANG_OR,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Or),
                OperatorDefiner_New(TOKEN_REXLANG_XOR,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Xor),
                OperatorDefiner_New(TOKEN_REXLANG_SHL,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Shl),
                OperatorDefiner_New(TOKEN_REXLANG_SHR,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Shr),
                OperatorDefiner_New(TOKEN_REXLANG_EQU,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Equ),
                OperatorDefiner_New(TOKEN_REXLANG_NEQ,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Neq),
                OperatorDefiner_New(TOKEN_REXLANG_LES,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Les),
                OperatorDefiner_New(TOKEN_REXLANG_GRT,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Grt),
                OperatorDefiner_New(TOKEN_REXLANG_LEQ,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Leq),
                OperatorDefiner_New(TOKEN_REXLANG_GRQ,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Grq),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ I8_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ASS,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Ass),
                OperatorDefiner_New(TOKEN_REXLANG_ASD,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Asd),
                OperatorDefiner_New(TOKEN_REXLANG_ASU,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Asu),
                OperatorDefiner_New(TOKEN_REXLANG_ASM,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Asm),
                OperatorDefiner_New(TOKEN_REXLANG_ASV,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Asv),
                OperatorDefiner_New(TOKEN_REXLANG_ADD,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Add),
                OperatorDefiner_New(TOKEN_REXLANG_SUB,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Sub),
                OperatorDefiner_New(TOKEN_REXLANG_MUL,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Mul),
                OperatorDefiner_New(TOKEN_REXLANG_DIV,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Div),
                OperatorDefiner_New(TOKEN_REXLANG_MOD,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Mod),
                OperatorDefiner_New(TOKEN_REXLANG_AND,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_And),
                OperatorDefiner_New(TOKEN_REXLANG_OR,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Or),
                OperatorDefiner_New(TOKEN_REXLANG_XOR,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Xor),
                OperatorDefiner_New(TOKEN_REXLANG_SHL,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Shl),
                OperatorDefiner_New(TOKEN_REXLANG_SHR,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Shr),
                OperatorDefiner_New(TOKEN_REXLANG_EQU,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Equ),
                OperatorDefiner_New(TOKEN_REXLANG_NEQ,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Neq),
                OperatorDefiner_New(TOKEN_REXLANG_LES,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Les),
                OperatorDefiner_New(TOKEN_REXLANG_GRT,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Grt),
                OperatorDefiner_New(TOKEN_REXLANG_LEQ,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Leq),
                OperatorDefiner_New(TOKEN_REXLANG_GRQ,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Grq),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ I16_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ASS,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Ass),
                OperatorDefiner_New(TOKEN_REXLANG_ASD,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Asd),
                OperatorDefiner_New(TOKEN_REXLANG_ASU,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Asu),
                OperatorDefiner_New(TOKEN_REXLANG_ASM,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Asm),
                OperatorDefiner_New(TOKEN_REXLANG_ASV,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Asv),
                OperatorDefiner_New(TOKEN_REXLANG_ADD,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Add),
                OperatorDefiner_New(TOKEN_REXLANG_SUB,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Sub),
                OperatorDefiner_New(TOKEN_REXLANG_MUL,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Mul),
                OperatorDefiner_New(TOKEN_REXLANG_DIV,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Div),
                OperatorDefiner_New(TOKEN_REXLANG_MOD,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Mod),
                OperatorDefiner_New(TOKEN_REXLANG_AND,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_And),
                OperatorDefiner_New(TOKEN_REXLANG_OR,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Or),
                OperatorDefiner_New(TOKEN_REXLANG_XOR,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Xor),
                OperatorDefiner_New(TOKEN_REXLANG_SHL,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Shl),
                OperatorDefiner_New(TOKEN_REXLANG_SHR,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Shr),
                OperatorDefiner_New(TOKEN_REXLANG_EQU,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Equ),
                OperatorDefiner_New(TOKEN_REXLANG_NEQ,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Neq),
                OperatorDefiner_New(TOKEN_REXLANG_LES,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Les),
                OperatorDefiner_New(TOKEN_REXLANG_GRT,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Grt),
                OperatorDefiner_New(TOKEN_REXLANG_LEQ,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Leq),
                OperatorDefiner_New(TOKEN_REXLANG_GRQ,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Grq),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ I32_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ASS,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Ass),
                OperatorDefiner_New(TOKEN_REXLANG_ASD,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Asd),
                OperatorDefiner_New(TOKEN_REXLANG_ASU,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Asu),
                OperatorDefiner_New(TOKEN_REXLANG_ASM,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Asm),
                OperatorDefiner_New(TOKEN_REXLANG_ASV,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Asv),
                OperatorDefiner_New(TOKEN_REXLANG_ADD,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Add),
                OperatorDefiner_New(TOKEN_REXLANG_SUB,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Sub),
                OperatorDefiner_New(TOKEN_REXLANG_MUL,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Mul),
                OperatorDefiner_New(TOKEN_REXLANG_DIV,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Div),
                OperatorDefiner_New(TOKEN_REXLANG_MOD,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Mod),
                OperatorDefiner_New(TOKEN_REXLANG_AND,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_And),
                OperatorDefiner_New(TOKEN_REXLANG_OR,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Or),
                OperatorDefiner_New(TOKEN_REXLANG_XOR,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Xor),
                OperatorDefiner_New(TOKEN_REXLANG_SHL,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Shl),
                OperatorDefiner_New(TOKEN_REXLANG_SHR,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Shr),
                OperatorDefiner_New(TOKEN_REXLANG_EQU,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Equ),
                OperatorDefiner_New(TOKEN_REXLANG_NEQ,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Neq),
                OperatorDefiner_New(TOKEN_REXLANG_LES,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Les),
                OperatorDefiner_New(TOKEN_REXLANG_GRT,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Grt),
                OperatorDefiner_New(TOKEN_REXLANG_LEQ,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Leq),
                OperatorDefiner_New(TOKEN_REXLANG_GRQ,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Grq),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ I64_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ASS,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Ass),
                OperatorDefiner_New(TOKEN_REXLANG_ASD,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Asd),
                OperatorDefiner_New(TOKEN_REXLANG_ASU,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Asu),
                OperatorDefiner_New(TOKEN_REXLANG_ASM,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Asm),
                OperatorDefiner_New(TOKEN_REXLANG_ASV,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Asv),
                OperatorDefiner_New(TOKEN_REXLANG_ADD,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Add),
                OperatorDefiner_New(TOKEN_REXLANG_SUB,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Sub),
                OperatorDefiner_New(TOKEN_REXLANG_MUL,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Mul),
                OperatorDefiner_New(TOKEN_REXLANG_DIV,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Div),
                OperatorDefiner_New(TOKEN_REXLANG_MOD,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Mod),
                OperatorDefiner_New(TOKEN_REXLANG_AND,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_And),
                OperatorDefiner_New(TOKEN_REXLANG_OR,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Or),
                OperatorDefiner_New(TOKEN_REXLANG_XOR,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Xor),
                OperatorDefiner_New(TOKEN_REXLANG_SHL,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Shl),
                OperatorDefiner_New(TOKEN_REXLANG_SHR,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Shr),
                OperatorDefiner_New(TOKEN_REXLANG_EQU,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Equ),
                OperatorDefiner_New(TOKEN_REXLANG_NEQ,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Neq),
                OperatorDefiner_New(TOKEN_REXLANG_LES,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Les),
                OperatorDefiner_New(TOKEN_REXLANG_GRT,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Grt),
                OperatorDefiner_New(TOKEN_REXLANG_LEQ,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Leq),
                OperatorDefiner_New(TOKEN_REXLANG_GRQ,(Token(*)(void*,Token*,Vector*))U16_U16_Handler_Grq),
                OPERATORDEFINER_END
            })),
            OPERATORINTERATER_END
        }),RexLang_Destroyer,RexLang_Cpyer)
    );
}