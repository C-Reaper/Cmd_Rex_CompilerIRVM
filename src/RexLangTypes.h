#ifndef REXLANGTYPES_H
#define REXLANGTYPES_H

#include "RexLangDefines.h"
#include "RexLangASM.h"

Token RexLang_Void_Null_Handler_Cast(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);

    //printf("[Void]: CAST: %s\n",a->str);

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
Token RexLang_Void_Handler_Cast(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_ExecuteCast(ll,op->str,a);
}
Token RexLang_Void_Handler_Size(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    //printf("[Void]: SIZE: %s\n",a->str);
    return Token_New_I64(TOKEN_NUMBER,VOID_SIZE);
}


Token RexLang_Pointer_Pointer_Handler_Ass(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteAss(ll,a,b);
}
Token RexLang_Pointer_Pointer_Handler_Asd(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteAss5(ll,IRVM_OP_ADD,a,b);
}
Token RexLang_Pointer_Pointer_Handler_Asu(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteAss5(ll,IRVM_OP_SUB,a,b);
}

Token RexLang_Pointer_Pointer_Handler_Add(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_ADD,a,b);
}
Token RexLang_Pointer_Pointer_Handler_Sub(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_Execute5(ll,IRVM_OP_SUB,a,b);
}
Token RexLang_Pointer_Handler_Inc(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_Execute4(ll,IRVM_OP_INC,a);
}
Token RexLang_Pointer_Handler_Dec(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_Execute4(ll,IRVM_OP_DEC,a);
}

Token RexLang_Pointer_Pointer_Handler_Equ(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteType5(ll,IRVM_TYPE_BOOL,IRVM_OP_EQ,a,b);
}
Token RexLang_Pointer_Pointer_Handler_Neq(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteType5(ll,IRVM_TYPE_BOOL,IRVM_OP_NQ,a,b);
}
Token RexLang_Pointer_Pointer_Handler_Les(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteType5(ll,IRVM_TYPE_BOOL,IRVM_OP_LT,a,b);
}
Token RexLang_Pointer_Pointer_Handler_Grt(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteType5(ll,IRVM_TYPE_BOOL,IRVM_OP_GT,a,b);
}
Token RexLang_Pointer_Pointer_Handler_Leq(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteType5(ll,IRVM_TYPE_BOOL,IRVM_OP_LQ,a,b);
}
Token RexLang_Pointer_Pointer_Handler_Grq(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteType5(ll,IRVM_TYPE_BOOL,IRVM_OP_GQ,a,b);
}
Token RexLang_Pointer_Handler_Adr(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_ExecuteAdr(ll,a);
}

Token RexLang_Pointer_Handler_Drf(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_ExecuteDrf(ll,a);
}
Token RexLang_Pointer_Handler_Arw(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    //printf("[Pointer]: ARW: %s->%s\n",a->str,b->str);
    
    Token dref = RexLang_ExecuteDrf(ll,a);
    Token acs = RexLang_ExecuteAcs(ll,&dref,b->str);
    Token_Free(&dref);
    return acs;
}
Token RexLang_Pointer_I64_Handler_Subs(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    //printf("[Pointer]: SUBS: %s[%s]\n",a->str,b->str);
    
    Token add = Token_By(TOKEN_REXLANG_ADD,"+");
    Token drf = Token_By(TOKEN_REXLANG_DRF,"*");

    Token out = RexLang_Pointer_Pointer_Handler_Add(ll,&add,args);
    TokenMap newargs = TokenMap_Make((Token[]){ out,Token_Null() });
    Token ret = RexLang_Pointer_Handler_Drf(ll,&drf,&newargs);
    TokenMap_Free(&newargs);

    Token_Free(&drf);
    Token_Free(&add);
    return ret;
}

Token RexLang_Pointer_Null_Handler_Cast(RexLang* ll,Token* op,Vector* args){
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
Token RexLang_Pointer_Handler_Cast(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_ExecuteCast(ll,op->str,a);
}
Token RexLang_Pointer_Handler_Size(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    //printf("[Pointer]: SIZE: %s\n",a->str);
    return Token_New_I64(TOKEN_NUMBER,POINTER_SIZE);
}


Token RexLang_Dref_Null_Handler_Cast(RexLang* ll,Token* op,Vector* args){
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
Token RexLang_Dref_Handler_Cast(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_ExecuteCast(ll,op->str,a);
}
Token RexLang_Dref_Handler_Size(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    //printf("[Dref]: SIZE: %s\n",a->str);
    return Token_New_I64(TOKEN_NUMBER,POINTER_SIZE);
}


Token RexLang_Str_Null_Handler_Cast(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);

    //printf("[Str]: CAST: %s\n",a->str);

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
Token RexLang_Str_Handler_Cast(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);

    if(op->str==NULL) return RexLang_Str_Null_Handler_Cast(ll,op,args);
    return Token_Null();
}
Token RexLang_Str_Handler_Size(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    //printf("[Str]: SIZE: %s\n",a->str);
    return Token_New_I64(TOKEN_NUMBER,(STR_SIZE));
}


Token RexLang_Struct_Struct_Ass_PtrSSA(RexLang* ll,CStr type,Token* a,Token* b){   
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
Token RexLang_Struct_Struct_Ass_LSplitSSA(RexLang* ll,CStr type,Token* a,Token* b){
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
            Token_By(TOKEN_TYPE,IRVM_TYPE_POINTER),
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
Token RexLang_Struct_Struct_Ass_RSplitSSA(RexLang* ll,CStr type,Token* a,Token* b){
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
            Token_By(TOKEN_TYPE,IRVM_TYPE_POINTER),
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
Token RexLang_Struct_Struct_Ass_LRSplitSSA(RexLang* ll,CStr type,Token* a,Token* b){
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

Token RexLang_Struct_Struct_Handler_Ass(RexLang* ll,Token* op,Vector* args){   
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
            return RexLang_Struct_Struct_Ass_LRSplitSSA(ll,va->typename,a,b);
        }else if(split_a){
            return RexLang_Struct_Struct_Ass_LSplitSSA(ll,va->typename,a,b);
        }else if(split_b){
            return RexLang_Struct_Struct_Ass_RSplitSSA(ll,va->typename,a,b);
        }else{
            return RexLang_Struct_Struct_Ass_PtrSSA(ll,va->typename,a,b);
        }
    }else{
        Environment_ErrorHandler(&ll->ev,"Ass: Error -> %s is no struct type!",b->str);
        return Token_Null();
    }
}
Token RexLang_Struct_Handler_Adr(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_ExecuteAdr(ll,a);
}
Token RexLang_Struct_Handler_Acs(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return RexLang_ExecuteAcs(ll,a,b->str);
}

Token RexLang_Struct_Null_Handler_Cast(RexLang* ll,Token* op,Vector* args){
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
Token RexLang_Struct_Handler_Cast(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    return RexLang_ExecuteCast(ll,op->str,a);
}
Token RexLang_Struct_Handler_Size(RexLang* ll,Token* op,Vector* args){
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


void RexLang_LoadVoidType(RexLang* ll){
    TypeMap_Push(&ll->ev.sc.types,(Type[]){
        Type_Make(VOID_TYPE,sizeof(RexLangVariable),OperatorInterationMap_Make((OperatorInterater[]){
            OperatorInterater_Make((CStr[]){ NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_CAST,(Token(*)(void*,Token*,Vector*))RexLang_Void_Handler_Cast),
                OperatorDefiner_New(TOKEN_INIT,(Token(*)(void*,Token*,Vector*))RexLang_Init),
                OperatorDefiner_New(TOKEN_REXLANG_SIZE,(Token(*)(void*,Token*,Vector*))RexLang_Void_Handler_Size),
                //OperatorDefiner_New(TOKEN_DESTROY,NULL),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ VOID_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                //OperatorDefiner_New(TOKEN_REXLANG_ASS,(Token(*)(void*,Token*,Vector*))Void_Void_Handler_Ass),
                OPERATORDEFINER_END
            })),
            OPERATORINTERATER_END
        }),RexLang_Destroyer,RexLang_Cpyer)
    });
}
void RexLang_LoadPointerType(RexLang* ll){
    TypeMap_Push(&ll->ev.sc.types,(Type[]){
        Type_Make(POINTER_TYPE,sizeof(RexLangVariable),OperatorInterationMap_Make((OperatorInterater[]){
            OperatorInterater_Make((CStr[]){ NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_INC,(Token(*)(void*,Token*,Vector*))RexLang_Pointer_Handler_Inc),
                OperatorDefiner_New(TOKEN_REXLANG_DEC,(Token(*)(void*,Token*,Vector*))RexLang_Pointer_Handler_Dec),
                OperatorDefiner_New(TOKEN_REXLANG_ADR,(Token(*)(void*,Token*,Vector*))RexLang_Pointer_Handler_Adr),
                OperatorDefiner_New(TOKEN_REXLANG_DRF,(Token(*)(void*,Token*,Vector*))RexLang_Pointer_Handler_Drf),
                OperatorDefiner_New(TOKEN_CAST,(Token(*)(void*,Token*,Vector*))RexLang_Pointer_Handler_Cast),
                OperatorDefiner_New(TOKEN_INIT,(Token(*)(void*,Token*,Vector*))RexLang_Init),
                OperatorDefiner_New(TOKEN_REXLANG_SIZE,(Token(*)(void*,Token*,Vector*))RexLang_Pointer_Handler_Size),
                //OperatorDefiner_New(TOKEN_DESTROY,NULL),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ POINTER_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ASS,(Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Ass),
                OperatorDefiner_New(TOKEN_REXLANG_EQU,(Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Equ),
                OperatorDefiner_New(TOKEN_REXLANG_NEQ,(Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Neq),
                OperatorDefiner_New(TOKEN_REXLANG_LES,(Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Les),
                OperatorDefiner_New(TOKEN_REXLANG_GRT,(Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Grt),
                OperatorDefiner_New(TOKEN_REXLANG_LEQ,(Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Leq),
                OperatorDefiner_New(TOKEN_REXLANG_GRQ,(Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Grq),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ I32_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ASS, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Ass),
                OperatorDefiner_New(TOKEN_REXLANG_ASD, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Asd),
                OperatorDefiner_New(TOKEN_REXLANG_ASU, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Asu),
                OperatorDefiner_New(TOKEN_REXLANG_ADD, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Add),
                OperatorDefiner_New(TOKEN_REXLANG_SUB, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Sub),
                OperatorDefiner_New(TOKEN_REXLANG_EQU, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Equ),
                OperatorDefiner_New(TOKEN_REXLANG_NEQ, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Neq),
                OperatorDefiner_New(TOKEN_REXLANG_LES, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Les),
                OperatorDefiner_New(TOKEN_REXLANG_GRT, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Grt),
                OperatorDefiner_New(TOKEN_REXLANG_LEQ, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Leq),
                OperatorDefiner_New(TOKEN_REXLANG_GRQ, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Grq),
                OperatorDefiner_New(TOKEN_REXLANG_SUBS,(Token(*)(void*,Token*,Vector*))RexLang_Pointer_I64_Handler_Subs),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ I64_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ASS, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Ass),
                OperatorDefiner_New(TOKEN_REXLANG_ASD, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Asd),
                OperatorDefiner_New(TOKEN_REXLANG_ASU, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Asu),
                OperatorDefiner_New(TOKEN_REXLANG_ADD, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Add),
                OperatorDefiner_New(TOKEN_REXLANG_SUB, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Sub),
                OperatorDefiner_New(TOKEN_REXLANG_EQU, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Equ),
                OperatorDefiner_New(TOKEN_REXLANG_NEQ, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Neq),
                OperatorDefiner_New(TOKEN_REXLANG_LES, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Les),
                OperatorDefiner_New(TOKEN_REXLANG_GRT, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Grt),
                OperatorDefiner_New(TOKEN_REXLANG_LEQ, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Leq),
                OperatorDefiner_New(TOKEN_REXLANG_GRQ, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Grq),
                OperatorDefiner_New(TOKEN_REXLANG_SUBS,(Token(*)(void*,Token*,Vector*))RexLang_Pointer_I64_Handler_Subs),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ U32_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ASS, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Ass),
                OperatorDefiner_New(TOKEN_REXLANG_ASD, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Asd),
                OperatorDefiner_New(TOKEN_REXLANG_ASU, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Asu),
                OperatorDefiner_New(TOKEN_REXLANG_ADD, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Add),
                OperatorDefiner_New(TOKEN_REXLANG_SUB, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Sub),
                OperatorDefiner_New(TOKEN_REXLANG_EQU, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Equ),
                OperatorDefiner_New(TOKEN_REXLANG_NEQ, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Neq),
                OperatorDefiner_New(TOKEN_REXLANG_LES, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Les),
                OperatorDefiner_New(TOKEN_REXLANG_GRT, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Grt),
                OperatorDefiner_New(TOKEN_REXLANG_LEQ, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Leq),
                OperatorDefiner_New(TOKEN_REXLANG_GRQ, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Grq),
                OperatorDefiner_New(TOKEN_REXLANG_SUBS,(Token(*)(void*,Token*,Vector*))RexLang_Pointer_I64_Handler_Subs),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ U64_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ASS, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Ass),
                OperatorDefiner_New(TOKEN_REXLANG_ASD, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Asd),
                OperatorDefiner_New(TOKEN_REXLANG_ASU, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Asu),
                OperatorDefiner_New(TOKEN_REXLANG_ADD, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Add),
                OperatorDefiner_New(TOKEN_REXLANG_SUB, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Sub),
                OperatorDefiner_New(TOKEN_REXLANG_EQU, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Equ),
                OperatorDefiner_New(TOKEN_REXLANG_NEQ, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Neq),
                OperatorDefiner_New(TOKEN_REXLANG_LES, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Les),
                OperatorDefiner_New(TOKEN_REXLANG_GRT, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Grt),
                OperatorDefiner_New(TOKEN_REXLANG_LEQ, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Leq),
                OperatorDefiner_New(TOKEN_REXLANG_GRQ, (Token(*)(void*,Token*,Vector*))RexLang_Pointer_Pointer_Handler_Grq),
                OperatorDefiner_New(TOKEN_REXLANG_SUBS,(Token(*)(void*,Token*,Vector*))RexLang_Pointer_I64_Handler_Subs),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ OPERATORINTERATER_DONTCARE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ARW,(Token(*)(void*,Token*,Vector*))RexLang_Pointer_Handler_Arw),
                OPERATORDEFINER_END
            })),
            OPERATORINTERATER_END
        }),RexLang_Destroyer,RexLang_Cpyer)
    });
}
void RexLang_LoadDrefType(RexLang* ll){
    TypeMap_Push(&ll->ev.sc.types,(Type[]){
        Type_Make(DREF_TYPE,sizeof(RexLangVariable),OperatorInterationMap_Make((OperatorInterater[]){
            OperatorInterater_Make((CStr[]){ NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_SIZE,(Token(*)(void*,Token*,Vector*))RexLang_Dref_Handler_Size),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ DREF_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OPERATORDEFINER_END
            })),
            OPERATORINTERATER_END
        }),RexLang_Destroyer,RexLang_Cpyer)
    });
}
void RexLang_LoadStrType(RexLang* ll){
    TypeMap_Push(&ll->ev.sc.types,(Type[]){
        Type_Make(STR_TYPE,sizeof(RexLangVariable),OperatorInterationMap_Make((OperatorInterater[]){
            OperatorInterater_Make((CStr[]){ NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_CAST,(Token(*)(void*,Token*,Vector*))RexLang_Str_Handler_Cast),
                OperatorDefiner_New(TOKEN_INIT,(Token(*)(void*,Token*,Vector*))RexLang_Init),
                OperatorDefiner_New(TOKEN_REXLANG_SIZE,(Token(*)(void*,Token*,Vector*))RexLang_Str_Handler_Size),
                //OperatorDefiner_New(TOKEN_DESTROY,NULL),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ VOID_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                //OperatorDefiner_New(TOKEN_REXLANG_ASS,(Token(*)(void*,Token*,Vector*))Str_Str_Handler_Ass),
                OPERATORDEFINER_END
            })),
            OPERATORINTERATER_END
        }),RexLang_Destroyer,RexLang_Cpyer)
    });
}
void RexLang_LoadStructType(RexLang* ll){
    TypeMap_Push(&ll->ev.sc.types,(Type[]){
        Type_Make(STRUCT_TYPE,sizeof(RexLangVariable),OperatorInterationMap_Make((OperatorInterater[]){
            OperatorInterater_Make((CStr[]){ NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ADR,(Token(*)(void*,Token*,Vector*))RexLang_Struct_Handler_Adr),
                OperatorDefiner_New(TOKEN_CAST,(Token(*)(void*,Token*,Vector*))RexLang_Struct_Handler_Cast),
                OperatorDefiner_New(TOKEN_INIT,(Token(*)(void*,Token*,Vector*))RexLang_Init),
                OperatorDefiner_New(TOKEN_REXLANG_SIZE,(Token(*)(void*,Token*,Vector*))RexLang_Struct_Handler_Size),
                //OperatorDefiner_New(TOKEN_DESTROY,NULL),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ STRUCT_TYPE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ASS,(Token(*)(void*,Token*,Vector*))RexLang_Struct_Struct_Handler_Ass),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ OPERATORINTERATER_DONTCARE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_REXLANG_ACS,(Token(*)(void*,Token*,Vector*))RexLang_Struct_Handler_Acs),
                OPERATORDEFINER_END
            })),
            OPERATORINTERATER_END
        }),RexLang_Destroyer,RexLang_Cpyer)
    });
}

#endif //!REXLANGTYPES_H