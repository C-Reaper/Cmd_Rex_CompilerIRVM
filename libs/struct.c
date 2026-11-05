#include "/home/codeleaded/System/Static/Library/AlxCallStack.h"
#include "/home/codeleaded/System/Static/Library/AlxExternFunctions.h"

#include "../src/RexLang.h"
#include "../src/RexLangASM.h"

Token Struct_Struct_Ass_PtrSSA(RexLang* ll,CStr type,Token* a,Token* b){   
    // Var ptr, SSA ref
    Token ssa_a = RexLang_StructPtrSSA(ll,a);
    Token ssa_b = RexLang_StructPtrSSA(ll,b);
    Vec_CStr types = RexLang_UnfoldMemberTypes(ll,type);
    int offset = 0;

    for(int i = 0;i<types.size;i++){
        CStr t = *(CStr*)Vector_Get(&types,i);
        
        const int t_size = RexLang_Size(ll,t);
        const Number ssa_id_0 = ll->ssas++;
        const Number ssa_id_1 = ll->ssas++;
        const Number ssa_id_2 = ll->ssas++;

        RexLang_AppendTM(ll,TokenMap_Make((Token[]){
            Token_By(TOKEN_IRVM_ADD,IRVM_OP_ADD),
            Token_By(TOKEN_TYPE,RexLang_Type(ll,t)),
            Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ssa_id_0)),
            Token_Cpy(&ssa_a),
            Token_New_I64(TOKEN_NUMBER,offset),
            Token_Null()
        }));
        RexLang_AppendTM(ll,TokenMap_Make((Token[]){
            Token_By(TOKEN_IRVM_ADD,IRVM_OP_ADD),
            Token_By(TOKEN_TYPE,RexLang_Type(ll,t)),
            Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ssa_id_1)),
            Token_Cpy(&ssa_b),
            Token_New_I64(TOKEN_NUMBER,offset),
            Token_Null()
        }));
        RexLang_AppendTM(ll,TokenMap_Make((Token[]){
            Token_By(TOKEN_IRVM_LOAD,IRVM_OP_LOAD),
            Token_By(TOKEN_TYPE,RexLang_Type(ll,t)),
            Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ssa_id_2)),
            Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ssa_id_1)),
            Token_Null()
        }));
        RexLang_AppendTM(ll,TokenMap_Make((Token[]){
            Token_By(TOKEN_IRVM_STORE,IRVM_OP_STORE),
            Token_By(TOKEN_TYPE,RexLang_Type(ll,t)),
            Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ssa_id_0)),
            Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ssa_id_2)),
            Token_Null()
        }));

        offset += t_size;
    }

    Vec_CStr_Free(&types);
    Token_Free(&ssa_a);
    Token_Free(&ssa_b);
    return Token_Cpy(a);
}
Token Struct_Struct_Ass_LSplitSSA(RexLang* ll,CStr type,Token* a,Token* b){
    // (SSA Split Return Value) = (Var ptr | SSA ref)
    Variable* va = Scope_FindVariable(&ll->ev.sc,a->str);
    RexLangVariable* va_sv = (RexLangVariable*)Variable_Data(va);
    const TT_Type tt = RexLangVariable_Type(va_sv);
    
    Token ssa_b = RexLang_StructPtrSSA(ll,b);
    Vec_CStr types = RexLang_UnfoldMemberTypes(ll,type);
    int offset = 0;

    for(int i = 0;i<types.size;i++){
        CStr t = *(CStr*)Vector_Get(&types,i);
        
        const int t_size = RexLang_Size(ll,t);
        const Number ssa_id_0 = ll->ssas++;

        RexLang_AppendTM(ll,TokenMap_Make((Token[]){
            Token_By(TOKEN_IRVM_ADD,IRVM_OP_ADD),
            Token_By(TOKEN_TYPE,RexLang_Type(ll,t)),
            Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ssa_id_0)),
            Token_Cpy(&ssa_b),
            Token_New_I64(TOKEN_NUMBER,offset),
            Token_Null()
        }));

        if(va_sv->storage == REXLANG_STORAGE_RETURN){
            Token ssa = Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ll->ssas++));
            Vec_CStr_Push(&ll->return_ssas,CStr_Cpy(ssa.str));
            
            RexLang_AppendTM(ll,TokenMap_Make((Token[]){
                Token_By(TOKEN_IRVM_LOAD,IRVM_OP_LOAD),
                Token_By(TOKEN_TYPE,RexLang_Type(ll,t)),
                ssa,
                Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ssa_id_0)),
                Token_Null()
            }));
        }else{
            RexLang_AppendTM(ll,TokenMap_Make((Token[]){
                Token_By(TOKEN_IRVM_LOAD,IRVM_OP_LOAD),
                Token_By(TOKEN_TYPE,RexLang_Type(ll,t)),
                Token_Move(tt,(CStr)Number_Get(va_sv->id + i)),
                Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ssa_id_0)),
                Token_Null()
            }));
        }
        
        offset += t_size;
    }

    Vec_CStr_Free(&types);
    return Token_Cpy(a);
}
Token Struct_Struct_Ass_RSplitSSA(RexLang* ll,CStr type,Token* a,Token* b){
    // (Var ptr | SSA ref) = (SSA Split Return Value)
    Variable* vb = Scope_FindVariable(&ll->ev.sc,b->str);
    RexLangVariable* vb_sv = (RexLangVariable*)Variable_Data(vb);
    const TT_Type tt = RexLangVariable_Type(vb_sv);
    
    Token ssa_a = RexLang_StructPtrSSA(ll,a);
    Vec_CStr types = RexLang_UnfoldMemberTypes(ll,type);
    int offset = 0;

    for(int i = 0;i<types.size;i++){
        CStr t = *(CStr*)Vector_Get(&types,i);
        
        const int t_size = RexLang_Size(ll,t);
        const Number ssa_id_0 = ll->ssas++;

        RexLang_AppendTM(ll,TokenMap_Make((Token[]){
            Token_By(TOKEN_IRVM_ADD,IRVM_OP_ADD),
            Token_By(TOKEN_TYPE,RexLang_Type(ll,t)),
            Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ssa_id_0)),
            Token_Cpy(&ssa_a),
            Token_New_I64(TOKEN_NUMBER,offset),
            Token_Null()
        }));
        RexLang_AppendTM(ll,TokenMap_Make((Token[]){
            Token_By(TOKEN_IRVM_STORE,IRVM_OP_STORE),
            Token_By(TOKEN_TYPE,RexLang_Type(ll,t)),
            Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ssa_id_0)),
            Token_Move(tt,(CStr)Number_Get(vb_sv->id + i)),
            Token_Null()
        }));

        offset += t_size;
    }

    Vec_CStr_Free(&types);
    return Token_Cpy(a);
}
Token Struct_Struct_Ass_LRSplitSSA(RexLang* ll,CStr type,Token* a,Token* b){
    // (SSA Split Return Value) = (SSA Split Return Value)
    Variable* va = Scope_FindVariable(&ll->ev.sc,a->str);
    RexLangVariable* va_sv = (RexLangVariable*)Variable_Data(va);
    const TT_Type tt_a = RexLangVariable_Type(va_sv);
    
    Variable* vb = Scope_FindVariable(&ll->ev.sc,b->str);
    RexLangVariable* vb_sv = (RexLangVariable*)Variable_Data(vb);
    const TT_Type tt_b = RexLangVariable_Type(vb_sv);
    
    Vec_CStr types = RexLang_UnfoldMemberTypes(ll,type);
    int offset = 0;

    for(int i = 0;i<types.size;i++){
        CStr t = *(CStr*)Vector_Get(&types,i);
        const int t_size = RexLang_Size(ll,t);

        if(va_sv->storage == REXLANG_STORAGE_RETURN){
            Token ssa = Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ll->ssas++));
            Vec_CStr_Push(&ll->return_ssas,CStr_Cpy(ssa.str));
            
            RexLang_AppendTM(ll,TokenMap_Make((Token[]){
                Token_By(TOKEN_IRVM_ASS,IRVM_OP_ASS),
                Token_By(TOKEN_TYPE,RexLang_Type(ll,t)),
                ssa,
                Token_Move(tt_b,(CStr)Number_Get(vb_sv->id + i)),
                Token_Null()
            }));
        }else{
            RexLang_AppendTM(ll,TokenMap_Make((Token[]){
                Token_By(TOKEN_IRVM_STORE,IRVM_OP_STORE),
                Token_By(TOKEN_TYPE,RexLang_Type(ll,t)),
                Token_Move(tt_a,(CStr)Number_Get(va_sv->id + i)),
                Token_Move(tt_b,(CStr)Number_Get(vb_sv->id + i)),
                Token_Null()
            }));
        }

        offset += t_size;
    }

    Vec_CStr_Free(&types);
    return Token_Cpy(a);
}

Token Struct_Struct_Handler_Ass(RexLang* ll,Token* op,Vector* args){   
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    //printf("[Struct]: ASS: %s = %s\n",a->str,b->str);

    if(a->tt==TOKEN_STRING && b->tt==TOKEN_STRING){
        Variable* va = Scope_FindVariable(&ll->ev.sc,a->str);
        Variable* vb = Scope_FindVariable(&ll->ev.sc,b->str);

        Type* t_a = Scope_FindType(&ll->ev.sc,va->typename);
        Type* t_b = Scope_FindType(&ll->ev.sc,vb->typename);
        
        if(t_a != t_b){
            Environment_ErrorHandler(&ll->ev,"Ass: Error -> %s and %s have different types!",a->str,b->str);
            return Token_Null();
        }

        const char split_a = CStr_CmpLike(a->str,COMPILER_RETURN"*",'*') || va->range == 1 || a->str[0] == '.';
        const char split_b = CStr_CmpLike(b->str,COMPILER_RETURN"*",'*') || vb->range == 1 || b->str[0] == '.';

        if(split_a && split_b){
            return Struct_Struct_Ass_LRSplitSSA(ll,va->typename,a,b);
        }else if(split_a){
            return Struct_Struct_Ass_LSplitSSA(ll,va->typename,a,b);
        }else if(split_b){
            return Struct_Struct_Ass_RSplitSSA(ll,va->typename,a,b);
        }else{
            return Struct_Struct_Ass_PtrSSA(ll,va->typename,a,b);
        }
    }else{
        Environment_ErrorHandler(&ll->ev,"Ass: Error -> %s is no struct type!",b->str);
        return Token_Null();
    }
}
Token Struct_Handler_Adr(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_ExecuteAdr(ll,a);
}
Token Struct_Handler_Acs(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteAcs(ll,a,b->str);
}

Token Struct_Null_Handler_Cast(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
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
Token Struct_Handler_Cast(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_ExecuteCast(ll,op->str,a);
}
Token Struct_Handler_Size(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    //printf("[Struct]: SIZE: %s\n",a->str);

    Type* t = Scope_FindType(&ll->ev.sc,a->str);
    if(t){
        int size = 0;
        for(int i = 0;i<t->related.size;i++){
            Member* m = (Member*)Vector_Get(&t->related,i);
            size += RexLang_Size(ll,m->type);
        }
        return Token_New_I64(TOKEN_NUMBER,size);
    }
    return Token_New_I64(TOKEN_NUMBER,0);
}

void Ex_Packer(ExternFunctionMap* Extern_Functions,Vector* funcs,Scope* s){//Vector<CStr>
    TypeMap_PushContained(&s->types,funcs,
        Type_Make(STRUCT_TYPE,sizeof(RexLangVariable),OperatorInterationMap_Make((OperatorInterater[]){
            OperatorInterater_Make((CStr[]){ NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ADR,(Token(*)(void*,Token*,Vector*))Struct_Handler_Adr),
                OperatorDefiner_New(TOKEN_CAST,(Token(*)(void*,Token*,Vector*))Struct_Handler_Cast),
                OperatorDefiner_New(TOKEN_INIT,(Token(*)(void*,Token*,Vector*))RexLang_Init),
                OperatorDefiner_New(TOKEN_REXLANG_SIZE,(Token(*)(void*,Token*,Vector*))Struct_Handler_Size),
                //OperatorDefiner_New(TOKEN_DESTROY,NULL),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ STRUCT_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ASS,(Token(*)(void*,Token*,Vector*))Struct_Struct_Handler_Ass),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ OPERATORINTERATER_DONTCARE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ACS,(Token(*)(void*,Token*,Vector*))Struct_Handler_Acs),
                OPERATORDEFINER_END
            })),
            OPERATORINTERATER_END
        }),RexLang_Destroyer,RexLang_Cpyer)
    );
}