#include "/home/codeleaded/System/Static/Library/AlxCallStack.h"
#include "/home/codeleaded/System/Static/Library/AlxExternFunctions.h"

#include "../src/RexLang.h"
#include "../src/RexLangASM.h"

Token Dref_Null_Handler_Cast(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);

    //printf("[Dref]: CAST: %s\n",a->str);

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
Token Dref_Handler_Cast(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_ExecuteCast(ll,op->str,a);
}
Token Dref_Handler_Size(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    //printf("[Dref]: SIZE: %s\n",a->str);
    return Token_New_I64(TOKEN_NUMBER,POINTER_SIZE);
}

void Ex_Packer(ExternFunctionMap* Extern_Functions,Vector* funcs,Scope* s){//Vector<CStr>
    TypeMap_PushContained(&s->types,funcs,
        Type_Make(DREF_TYPE,sizeof(RexLangVariable),OperatorInterationMap_Make((OperatorInterater[]){
            OperatorInterater_Make((CStr[]){ NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_SIZE,(Token(*)(void*,Token*,Vector*))Dref_Handler_Size),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ DREF_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OPERATORDEFINER_END
            })),
            OPERATORINTERATER_END
        }),RexLang_Destroyer,RexLang_Cpyer)
    );
}
