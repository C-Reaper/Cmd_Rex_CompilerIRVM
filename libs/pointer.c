#include "/home/codeleaded/System/Static/Library/AlxCallStack.h"
#include "/home/codeleaded/System/Static/Library/AlxExternFunctions.h"

#include "../src/RexLang.h"
#include "../src/RexLangASM.h"

Token Pointer_Pointer_Handler_Ass(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteAss(ll,a,b);
}

Token Pointer_Pointer_Handler_Asd(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteAss5(ll,IRVM_OP_ADD,a,b);
}
Token Pointer_Pointer_Handler_Asu(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteAss5(ll,IRVM_OP_SUB,a,b);
}

Token Pointer_Pointer_Handler_Add(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_ADD,a,b);
}
Token Pointer_Pointer_Handler_Sub(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_SUB,a,b);
}
Token Pointer_Handler_Inc(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_Execute4(ll,IRVM_OP_INC,a);
}
Token Pointer_Handler_Dec(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_Execute4(ll,IRVM_OP_DEC,a);
}

Token Pointer_Pointer_Handler_Equ(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_EQ,a,b);
}
Token Pointer_Pointer_Handler_Neq(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_NQ,a,b);
}
Token Pointer_Pointer_Handler_Les(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_LT,a,b);
}
Token Pointer_Pointer_Handler_Grt(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_GT,a,b);
}
Token Pointer_Pointer_Handler_Leq(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_LQ,a,b);
}
Token Pointer_Pointer_Handler_Grq(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_GQ,a,b);
}
Token Pointer_Handler_Adr(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_ExecuteAdr(ll,a);
}

Token Pointer_Null_Handler_Cast(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);

    //printf("[Pointer]: CAST: %s\n",a->str);

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
Token Pointer_Handler_Cast(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_ExecuteCast(ll,op->str,a);
}
Token Pointer_Handler_Size(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    //printf("[Pointer]: SIZE: %s\n",a->str);
    return Token_New_I64(TOKEN_NUMBER,POINTER_SIZE);
}

Token Pointer_Handler_Drf(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_ExecuteDrf(ll,a);
}
Token Pointer_Handler_Arw(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    //printf("[Pointer]: ARW: %s->%s\n",a->str,b->str);
    
    /*
    if(a->tt==TOKEN_STRING){
        Variable* v = Scope_FindVariable(&ll->ev.sc,a->str);
        
        if(v){
            CStr structtype = CStr_PopOff(v->typename);
            Type* t = Scope_FindType(&ll->ev.sc,structtype);
            CStr_Free(&structtype);
            if(t){
                Member* member = MemberMap_Find(&t->related,b->str);
                if(member){
                    if(member->access || CStr_Cmp(a->str,RexLang_SELF)){
                        int offset = 0;
                        for(int i = 0;i<t->related.size;i++){
                            Member* m = (Member*)Vector_Get(&t->related,i);
                            if(CStr_Cmp(b->str,m->name)) break;
                            offset += RexLang_Size(ll,m->type);
                        }

                        CStr stack_name = RexLang_Variablename_Next(ll,".STACK",6);

                        if(!RexLang_DrefType(ll,v->typename)){
                            CStr type = CStr_Concat(member->type,"&");
                            RexLang_Variable_Build_Decl(ll,stack_name,type);
                            CStr_Free(&type);

                            Token stack_t = Token_Move(TOKEN_STRING,stack_name);
                            CStr location_a = RexLang_Location(ll,a->str);
                            CStr location_s = RexLang_Location(ll,stack_name);

                            RexLang_Indentation_Appendf(ll,&ll->text,"mov %s,%s",RexLang_REG_A_64,location_a);
                            RexLang_Indentation_Appendf(ll,&ll->text,"mov %s,%s",location_s,RexLang_REG_A_64);
                            RexLang_Indentation_Appendf(ll,&ll->text,"add %s,%d",location_s,offset);

                            CStr_Free(&location_a);
                            CStr_Free(&location_s);
                        }else{
                            CStr type = CStr_Concat(member->type,"&");
                            RexLang_Variable_Build_Decl(ll,stack_name,type);
                            CStr_Free(&type);

                            Token stack_t = Token_Move(TOKEN_STRING,stack_name);
                            CStr location_a = RexLang_Location(ll,a->str);
                            CStr location_s = RexLang_Location(ll,stack_name);

                            RexLang_Indentation_Appendf(ll,&ll->text,"mov %s,%s",RexLang_REG_A_64,location_a);
                            RexLang_Indentation_Appendf(ll,&ll->text,"mov %s,%s[%s]",RexLang_REG_A_64,RexLang_DREF_64,RexLang_REG_A_64);
                            RexLang_Indentation_Appendf(ll,&ll->text,"mov %s,%s",location_s,RexLang_REG_A_64);
                            RexLang_Indentation_Appendf(ll,&ll->text,"add %s,%d",location_s,offset);

                            CStr_Free(&location_a);
                            CStr_Free(&location_s);
                        }
                        return Token_Move(TOKEN_STRING,stack_name);
                    }else{
                        Environment_ErrorHandler(&ll->ev,"Arw: Member %s isn't pub or non self %s tries to access!",b->str,a->str);
                        return Token_Null();
                    }
                }else{
                    Environment_ErrorHandler(&ll->ev,"Arw: Member %s not found!",b->str);
                    return Token_Null();
                }
            }else{
                Environment_ErrorHandler(&ll->ev,"Arw: Type %s not found!",v->typename);
                return Token_Null();
            }
        }else{
            Environment_ErrorHandler(&ll->ev,"Arw: Variable %s not found!",a->str);
            return Token_Null();
        }
    }else{
        Environment_ErrorHandler(&ll->ev,"Arw: Error -> %s has no dref op!",a->str);
        return Token_Null();
    }
    */
    return Token_Null();
}
Token Pointer_I64_Handler_Subs(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    //printf("[Pointer]: SUBS: %s[%s]\n",a->str,b->str);
    
    Token add = Token_By(TOKEN_REXLANG_ADD,"+");
    Token drf = Token_By(TOKEN_REXLANG_DRF,"*");

    Token out = Pointer_Pointer_Handler_Add(ll,&add,args);
    TokenMap newargs = TokenMap_Make((Token[]){ out,Token_Null() });
    Token ret = Pointer_Handler_Drf(ll,&drf,&newargs);
    TokenMap_Free(&newargs);

    Token_Free(&drf);
    Token_Free(&add);
    return ret;
}

void Ex_Packer(ExternFunctionMap* Extern_Functions,Vector* funcs,Scope* s){//Vector<CStr>
    TypeMap_PushContained(&s->types,funcs,
        Type_Make(POINTER_TYPE,sizeof(RexLangVariable),OperatorInterationMap_Make((OperatorInterater[]){
            OperatorInterater_Make((CStr[]){ NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_INC,(Token(*)(void*,Token*,Vector*))Pointer_Handler_Inc),
                OperatorDefiner_New(TOKEN_REXLANG_DEC,(Token(*)(void*,Token*,Vector*))Pointer_Handler_Dec),
                OperatorDefiner_New(TOKEN_REXLANG_ADR,(Token(*)(void*,Token*,Vector*))Pointer_Handler_Adr),
                OperatorDefiner_New(TOKEN_REXLANG_DRF,(Token(*)(void*,Token*,Vector*))Pointer_Handler_Drf),
                OperatorDefiner_New(TOKEN_CAST,(Token(*)(void*,Token*,Vector*))Pointer_Handler_Cast),
                OperatorDefiner_New(TOKEN_INIT,(Token(*)(void*,Token*,Vector*))RexLang_Init),
                OperatorDefiner_New(TOKEN_REXLANG_SIZE,(Token(*)(void*,Token*,Vector*))Pointer_Handler_Size),
                //OperatorDefiner_New(TOKEN_DESTROY,NULL),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ POINTER_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ASS,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Ass),
                OperatorDefiner_New(TOKEN_REXLANG_EQU,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Equ),
                OperatorDefiner_New(TOKEN_REXLANG_NEQ,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Neq),
                OperatorDefiner_New(TOKEN_REXLANG_LES,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Les),
                OperatorDefiner_New(TOKEN_REXLANG_GRT,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Grt),
                OperatorDefiner_New(TOKEN_REXLANG_LEQ,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Leq),
                OperatorDefiner_New(TOKEN_REXLANG_GRQ,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Grq),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ I32_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ASS,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Ass),
                OperatorDefiner_New(TOKEN_REXLANG_ASD,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Asd),
                OperatorDefiner_New(TOKEN_REXLANG_ASU,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Asu),
                OperatorDefiner_New(TOKEN_REXLANG_ADD,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Add),
                OperatorDefiner_New(TOKEN_REXLANG_SUB,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Sub),
                OperatorDefiner_New(TOKEN_REXLANG_EQU,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Equ),
                OperatorDefiner_New(TOKEN_REXLANG_NEQ,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Neq),
                OperatorDefiner_New(TOKEN_REXLANG_LES,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Les),
                OperatorDefiner_New(TOKEN_REXLANG_GRT,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Grt),
                OperatorDefiner_New(TOKEN_REXLANG_LEQ,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Leq),
                OperatorDefiner_New(TOKEN_REXLANG_GRQ,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Grq),
                OperatorDefiner_New(TOKEN_REXLANG_SUBS,(Token(*)(void*,Token*,Vector*))Pointer_I64_Handler_Subs),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ I64_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ASS,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Ass),
                OperatorDefiner_New(TOKEN_REXLANG_ASD,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Asd),
                OperatorDefiner_New(TOKEN_REXLANG_ASU,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Asu),
                OperatorDefiner_New(TOKEN_REXLANG_ADD,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Add),
                OperatorDefiner_New(TOKEN_REXLANG_SUB,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Sub),
                OperatorDefiner_New(TOKEN_REXLANG_EQU,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Equ),
                OperatorDefiner_New(TOKEN_REXLANG_NEQ,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Neq),
                OperatorDefiner_New(TOKEN_REXLANG_LES,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Les),
                OperatorDefiner_New(TOKEN_REXLANG_GRT,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Grt),
                OperatorDefiner_New(TOKEN_REXLANG_LEQ,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Leq),
                OperatorDefiner_New(TOKEN_REXLANG_GRQ,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Grq),
                OperatorDefiner_New(TOKEN_REXLANG_SUBS,(Token(*)(void*,Token*,Vector*))Pointer_I64_Handler_Subs),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ U32_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ASS,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Ass),
                OperatorDefiner_New(TOKEN_REXLANG_ASD,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Asd),
                OperatorDefiner_New(TOKEN_REXLANG_ASU,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Asu),
                OperatorDefiner_New(TOKEN_REXLANG_ADD,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Add),
                OperatorDefiner_New(TOKEN_REXLANG_SUB,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Sub),
                OperatorDefiner_New(TOKEN_REXLANG_EQU,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Equ),
                OperatorDefiner_New(TOKEN_REXLANG_NEQ,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Neq),
                OperatorDefiner_New(TOKEN_REXLANG_LES,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Les),
                OperatorDefiner_New(TOKEN_REXLANG_GRT,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Grt),
                OperatorDefiner_New(TOKEN_REXLANG_LEQ,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Leq),
                OperatorDefiner_New(TOKEN_REXLANG_GRQ,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Grq),
                OperatorDefiner_New(TOKEN_REXLANG_SUBS,(Token(*)(void*,Token*,Vector*))Pointer_I64_Handler_Subs),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ U64_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ASS,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Ass),
                OperatorDefiner_New(TOKEN_REXLANG_ASD,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Asd),
                OperatorDefiner_New(TOKEN_REXLANG_ASU,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Asu),
                OperatorDefiner_New(TOKEN_REXLANG_ADD,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Add),
                OperatorDefiner_New(TOKEN_REXLANG_SUB,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Sub),
                OperatorDefiner_New(TOKEN_REXLANG_EQU,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Equ),
                OperatorDefiner_New(TOKEN_REXLANG_NEQ,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Neq),
                OperatorDefiner_New(TOKEN_REXLANG_LES,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Les),
                OperatorDefiner_New(TOKEN_REXLANG_GRT,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Grt),
                OperatorDefiner_New(TOKEN_REXLANG_LEQ,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Leq),
                OperatorDefiner_New(TOKEN_REXLANG_GRQ,(Token(*)(void*,Token*,Vector*))Pointer_Pointer_Handler_Grq),
                OperatorDefiner_New(TOKEN_REXLANG_SUBS,(Token(*)(void*,Token*,Vector*))Pointer_I64_Handler_Subs),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ OPERATORINTERATER_DONTCARE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ARW,(Token(*)(void*,Token*,Vector*))Pointer_Handler_Arw),
                OPERATORDEFINER_END
            })),
            OPERATORINTERATER_END
        }),RexLang_Destroyer,RexLang_Cpyer)
    );
}
