#ifndef REXLANGASM_H
#define REXLANGASM_H

#include "/home/codeleaded/System/Static/Container/CVector.h"
#include "/home/codeleaded/System/Static/Library/AlxScope.h"
#include "/home/codeleaded/System/Static/Library/AlxShutingYard.h"
#include "/home/codeleaded/System/Static/Library/AlxCompiler.h"
#include "/home/codeleaded/System/Static/Library/ConstParser.h"
#include "/home/codeleaded/System/Static/Library/Files.h"
#include "/home/codeleaded/System/Static/Library/String.h"
#include "/home/codeleaded/System/Static/Library/IRVM.h"

#include "RexLangDefines.h"


typedef struct LogicBlock {
    int count;
    int extend;
} LogicBlock;

typedef struct Define {
    CStr name;
    TokenMap content;
} Define;

Define Define_New(CStr name,TokenMap tm){
    Define d;
    d.name = CStr_Cpy(name);
    d.content = tm;
    return d;
}
void Define_Free(Define* d){
    CStr_Free(&d->name);
    TokenMap_Free(&d->content);
}
void Define_Print(Define* d){
    printf("%s: ",d->name);
    TokenMap_Print_S(&d->content);
}


typedef struct RexLang {
    Compiler ev;        // like inheritance
    IRVM vm;            // store data
    CVector filesstack; // call stack of files -> path
    CVector filesinc;
    CVector defines; // Vector<Define>
    Vector logicpath; // Vector<LogicBlock>
    char* dllpath;
    char* src;
    char* output;
    TT_Type logic;
    TT_Type PADD1;
    unsigned int ssas;
    unsigned int lcmps;
    unsigned int PADD2;
} RexLang;

Vec_CStr RexLang_UnfoldType(RexLang* ll,CStr name) {
    if(!name) return Vec_CStr_New();
    
    Type* const t = Scope_FindType(&ll->ev.sc,name);
    if(!t) return Vec_CStr_New();

    if(t->related.size > 0){
        Vec_CStr types = Vec_CStr_New();
        
        for(int i = 0;i<t->related.size;i++){
            Member* m = (Member*)Vector_Get(&t->related,i);
            
            Vec_CStr m_types = RexLang_UnfoldType(ll,m->type);
            Vec_CStr_Add(&types,&m_types);
            Vec_CStr_Free(&m_types);
        }

        return types;
    }else{
        return Vec_CStr_Make((CStr[]){ CStr_Cpy(name) },1);
    }
}

void RexLang_AppendT(RexLang* ll,Token m_t) {
    IRVM_Function* fn = IRVM_FuncIn(&ll->vm);

    if(ll->vm.fns.size > 0){
        IRVM_Function* const fn = (IRVM_Function*)Vector_Get(&ll->vm.fns,ll->vm.fns.size - 1);
        if(fn->bbs.size == 0) PVector_Push(&fn->bbs,(IRVM_BasicBlock[]){ IRVM_BasicBlock_New() },sizeof(IRVM_BasicBlock));
        
        IRVM_BasicBlock* const bb = (IRVM_BasicBlock*)PVector_Get(&fn->bbs,fn->bbs.size - 1);
        if(bb->tms.size == 0) Vector_Push(&bb->tms,(TokenMap[]){ TokenMap_New() });
        
        Vector_Push(&bb->tms,&m_t);
    }else{
        Compiler_ErrorHandler(&ll->ev,"no function for token!");
    }
}
void RexLang_AppendTM(RexLang* ll,TokenMap m_tm) {
    IRVM_Function* fn = IRVM_FuncIn(&ll->vm);

    if(ll->vm.fns.size > 0){
        IRVM_Function* const fn = (IRVM_Function*)Vector_Get(&ll->vm.fns,ll->vm.fns.size - 1);
        if(fn->bbs.size == 0) PVector_Push(&fn->bbs,(IRVM_BasicBlock[]){ IRVM_BasicBlock_New() },sizeof(IRVM_BasicBlock));
        
        IRVM_BasicBlock* const bb = (IRVM_BasicBlock*)PVector_Get(&fn->bbs,fn->bbs.size - 1);
        Vector_Push(&bb->tms,&m_tm);
    }else{
        Compiler_ErrorHandler(&ll->ev,"no function for token!");
    }
}
void RexLang_AppendFunc(RexLang* ll,CStr name,CStr rtype,MemberMap* params) {
    Vec_CStr rtypes = RexLang_UnfoldType(ll,rtype);
    Vec_CStr ptypes = Vec_CStr_New();

    for(int i = 0;i<params->size;i++){
        Member* m = (Member*)Vector_Get(params,i);
        Vec_CStr types = RexLang_UnfoldType(ll,m->type);
        Vec_CStr_Add(&ptypes,&types);
        Vec_CStr_Free(&types);
    }

    Vector_Push(&ll->vm.fns,(IRVM_Function[]){
        IRVM_Function_Make(
            IRVM_ACCESS_NONE,
            name,
            (CStr*)rtypes.Memory,
            rtypes.size,
            (CStr*)ptypes.Memory,
            ptypes.size
        )
    });
}


Boolean RexLang_PointerType(RexLang* ll,CStr name){
    int size = CStr_Size(name);
    for(int i = size-1;i>=0;i--){
        if(name[i]=='*') return 1;
        if(name[i]=='&') continue;
        return 0;
    }
    return 0;
}
Boolean RexLang_DrefType(RexLang* ll,CStr name){
    if(!name) return 0;
    
    int size = CStr_Size(name);
    if(name[size-1]=='&') return 1;
    return 0;
}
int RexLang_DrefTypeCount(RexLang* ll,CStr name){
    int size = CStr_Size(name);
    for(int i = size - 1;i>=0;i--)
        if(name[i]!='&') return (size - 1) - i;
    return size;
}
CStr RexLang_TypeOfDref(RexLang* ll,CStr name){
    int size = CStr_Size(name);
    for(int i = size-1;i>=0;i--){
        if(name[i]=='&') continue;
        return CStr_Cpy_From_To(name,0,i+1);
    }
    return NULL;
}
CStr RexLang_TypeOfPointer(RexLang* ll,CStr name){
    int size = CStr_Size(name);
    for(int i = size-1;i>=0;i--){
        if(name[i]=='&' || name[i]=='*') continue;
        return CStr_Cpy_From_To(name,0,i+1);
    }
    return NULL;
}
int RexLang_Size(RexLang* ll,CStr name){
    if(RexLang_DrefType(ll,name)){
        return IRVM_ARCH_BYTES;
    }else{
        Type* t = Scope_FindType(&ll->ev.sc,name);
        if(t){
            Token op = Token_By(TOKEN_REXLANG_SIZE,"sizeof");
            
            Vector othertypes = Vector_New(sizeof(CStr));
            Token (*Handler)(void*,Token*,Vector*) = Scope_HandlerOf(&ll->ev.sc,&op,name,&othertypes);
            //for(int i = 0;i<types.size;i++) CStr_Free((CStr*)Vector_Get(&types,i));
            Vector_Free(&othertypes);
        
            if(Handler){
                TokenMap args = TokenMap_Make((Token[]){ Token_By(TOKEN_STRING,t->name),Token_Null() });
                Token Item = Handler(ll,&op,&args);
                TokenMap_Free(&args);
                Token_Free(&op);
            
                Number size = Item.v_i64;
                Token_Free(&Item);
                return size;
            }
            Token_Free(&op);
        }
    }
    return 0;
}
char RexLang_CompressStackType(RexLang* ll,TokenMap* tm,int start){
    Token* type = (Token*)Vector_Get(tm,start);
    
    int pret = 0;

    for(int i = start + 1;i<tm->size;i++){
        Token* tok = (Token*)Vector_Get(tm,i);
        
        if(tok->tt == TOKEN_REXLANG_SUBS) pret++;
        if(tok->tt == TOKEN_REXLANG_SUBSR){
            pret--;

            if(pret == 0){
                type->args = (Vector*)malloc(sizeof(Vector));
                *type->args = Vector_New(sizeof(TokenMap));
                Vector_Push(type->args,(TokenMap[]){ TokenMap_Sub(tm,start + 2,i) });
                TokenMap_Remove(tm,start + 2,i + 1);
                return 1;
            }else if(pret < 0){
                return 0;
            }
        }
    }
    return 0;
}

Number RexLang_Variablename_Id(RexLang* ll,CStr name,int offset){
    CStr build = CStr_Concat(name,"*");

    Number stack_name_count = Scope_FindVariablenameLastLike(&ll->ev.sc,build,'*',offset);
    if(stack_name_count==NUMBER_PARSE_ERROR)    stack_name_count = 0;

    CStr_Free(&build);
    return stack_name_count;
}
CStr RexLang_Variablename_Next(RexLang* ll,CStr name,int offset){
    CStr build = CStr_Concat(name,"*");

    Number stack_name_count = Scope_FindVariablenameLastLike(&ll->ev.sc,build,'*',offset);
    if(stack_name_count==NUMBER_PARSE_ERROR)    stack_name_count = 0;
    else                                        stack_name_count++;

    CStr_Free(&build);
    CStr stack_name = CStr_Format("%s%d",name,stack_name_count);
    return stack_name;
}
CStr RexLang_ConstStr(RexLang* ll,CStr name){
    int size = CStr_Size(name);
    String value = String_New();
    String buffer = String_New();
    
    for(int i = 0;i<size;i++){
        if(name[i]>=32 && name[i]<127){
            String_AppendChar(&buffer,name[i]);
        }else{
            if(buffer.size>0){
                String_AppendChar(&value,'\"');
                String_AppendString(&value,&buffer);
                String_AppendChar(&value,'\"');
                String_AppendChar(&value,',');
                String_Clear(&buffer);
            }
            String_AppendNumber(&value,name[i]);
            String_AppendChar(&value,',');
        }

        if(i == size - 1){
            if(buffer.size>0){
                String_AppendChar(&value,'\"');
                String_AppendString(&value,&buffer);
                String_AppendChar(&value,'\"');
                String_AppendChar(&value,',');
            }
        }
    }
    String_AppendNumber(&value,0);
    
    CStr cstr_value = String_CStr(&value);
    String_Free(&value);
    String_Free(&buffer);
    return cstr_value;
}
CStr RexLang_BuildConstStr(RexLang* ll,CStr cstr){
    return NULL;
}
CStr RexLang_BuildGlobal(RexLang* ll,CStr name,int size){
    return NULL;
}

void RexLang_Variable_BuildRange(RexLang* ll,CStr name,CStr type,Range r,RexLang_Storage storage,RexLang_Destroy destroy){
    if(ll->ev.sc.range == 0){
        CStr g_value = RexLang_BuildGlobal(ll,name,RexLang_Size(ll,type));
        Variable* v = Scope_FindVariable(&ll->ev.sc,name);
        
        RexLangVariable sv = RexLangVariable_New(ll,storage == REXLANG_STORAGE_AUTO ? REXLANG_STORAGE_GLOBAL : storage,destroy);
        sv.global = g_value;

        Scope_BuildInitVariableRange(&ll->ev.sc,name,type,r,&sv);
    }else{
        const RexLang_Storage real_storage = storage == REXLANG_STORAGE_AUTO ? REXLANG_STORAGE_STACK : storage;

        if(real_storage == REXLANG_STORAGE_STACK){
            RexLang_AppendTM(ll,TokenMap_Make((Token[]){
                Token_By(TOKEN_IRVM_ALLOCA,"alloca"),
                Token_By(TOKEN_TYPE,type),
                Token_By(TOKEN_IRVM_VAR,name),
                Token_Null()
            }));
        }else if(real_storage == REXLANG_STORAGE_STACK_BLOCK){
            RexLang_AppendTM(ll,TokenMap_Make((Token[]){
                Token_By(TOKEN_IRVM_ALLOCA,"alloca"),
                Token_By(TOKEN_TYPE,type),
                Token_By(TOKEN_IRVM_VAR,name),
                Token_Null()
            }));
        }

        Scope_BuildInitVariableRange(&ll->ev.sc,name,type,r,(RexLangVariable[]){
            RexLangVariable_New(
                ll,
                real_storage,
                destroy
            )
        });
    }
}
void RexLang_Variable_Build(RexLang* ll,CStr name,CStr type,RexLang_Destroy destroy){
    RexLang_Variable_BuildRange(ll,name,type,ll->ev.sc.range,REXLANG_STORAGE_AUTO,destroy);
}
void RexLang_Variable_Build_Return(RexLang* ll,CStr name,CStr type){
    RexLang_Variable_BuildRange(ll,name,type,ll->ev.sc.range,REXLANG_STORAGE_RETURN,REXLANG_DESTROY_FULL);
}
void RexLang_Variable_Build_Param(RexLang* ll,CStr name,CStr type,int i){
    RexLang_Variable_BuildRange(ll,name,type,ll->ev.sc.range,REXLANG_STORAGE_PARAM,REXLANG_DESTROY_FULL);

    Variable* v = Scope_FindVariable(&ll->ev.sc,name);
    RexLangVariable* sv = (RexLangVariable*)Variable_Data(v);
    sv->id = i + 1;
}
void RexLang_Variable_Build_SSA(RexLang* ll,CStr name,CStr type,int ssa){
    RexLang_Variable_BuildRange(ll,name,type,ll->ev.sc.range,REXLANG_STORAGE_SSA,REXLANG_DESTROY_FULL);

    Variable* v = Scope_FindVariable(&ll->ev.sc,name);
    RexLangVariable* sv = (RexLangVariable*)Variable_Data(v);
    sv->id = ssa;
}

CStr RexLang_VariableType(RexLang* ll,Token* a){
    if(a->tt==TOKEN_STRING){
        Variable* v_a = Scope_FindVariable(&ll->ev.sc,a->str);
        if(v_a) return RexLang_TypeOfDref(ll,v_a->typename);
    }
    if(a->tt==TOKEN_REXLANG_BOOLEAN)    return CStr_Cpy(BOOL_TYPE);
    if(a->tt==TOKEN_NUMBER)             return CStr_Cpy(I64_TYPE);
    if(a->tt==TOKEN_CONSTSTRING_DOUBLE) return CStr_Cpy(STR_TYPE);
    if(a->tt==TOKEN_CONSTSTRING_SINGLE) return CStr_Cpy(CHAR_TYPE);
    if(a->tt==TOKEN_FLOAT)              return CStr_Cpy(F64_TYPE);
    if(a->tt==TOKEN_FUNCTIONPOINTER)    return CStr_Cpy(NULL_TYPE);
    if(a->tt==TOKEN_REXLANG_NULL)       return CStr_Cpy(NULL_TYPE);
    return NULL;
}
CStr RexLang_VariablesType(RexLang* ll,Token* a,Token* b){
    CStr ret = RexLang_VariableType(ll,a);
    if(ret) return ret;
    return RexLang_VariableType(ll,b);
}

int RexLang_TypeRealSize(RexLang* ll,Token* a){
    CStr type = RexLang_VariableType(ll,a);
    if(!type) return 0;

    if(RexLang_DrefType(ll,type)){
        CStr ntype = RexLang_TypeOfDref(ll,type);
        CStr_Set(&type,ntype);
        CStr_Free(&ntype);
    }

    int size = RexLang_Size(ll,type);
    CStr_Free(&type);
    return size;
}
CStr RexLang_FunctionName(RexLang* ll,CStr name){
    String builder = String_Format("f.%s",name);
    String_Replace(&builder,"::",".");
    
    CStr functionname = String_CStr(&builder);
    String_Free(&builder);
    return functionname;
}
CStr RexLang_SpaceName(RexLang* ll,CStr name){
    String builder = String_New();

    for(int i = 0;i<ll->ev.cs.size;i++){
        CallPosition* cp = (CallPosition*)Vector_Get(&ll->ev.cs,i);
        if(cp->type==TOKEN_REXLANG_NAMESPACE){
            String_Append(&builder,cp->fname);
            String_Append(&builder,"::");
        }
    }
    String_Append(&builder,name);

    CStr cstr = String_CStr(&builder);
    String_Free(&builder);
    return cstr;
}
CStr RexLang_FuncSpaceName(RexLang* ll,CStr name){
    String builder = String_New();

    for(int i = 0;i<ll->ev.cs.size;i++){
        CallPosition* cp = (CallPosition*)Vector_Get(&ll->ev.cs,i);
        if(cp->type==TOKEN_REXLANG_IMPL){
            String_Append(&builder,cp->fname);
            String_Append(&builder,"::");
            break;
        }
    }

    if(builder.size == 0){
        for(int i = 0;i<ll->ev.cs.size;i++){
            CallPosition* cp = (CallPosition*)Vector_Get(&ll->ev.cs,i);
            if(cp->type==TOKEN_REXLANG_NAMESPACE){
                String_Append(&builder,cp->fname);
                String_Append(&builder,"::");
            }
        }
    }

    String_Append(&builder,name);

    CStr cstr = String_CStr(&builder);
    String_Free(&builder);
    return cstr;
}
Function* RexLang_FunctionIn(RexLang* ll) {
    for(int i = ll->ev.cs.size - 1;i>=0;i--){
        CallPosition* cp = Vector_Get(&ll->ev.cs,i);
        
        if(cp && cp->type==TOKEN_FUNCTIONDECL){
            TT_Iter it = FunctionMap_Find(&ll->ev.fs,cp->fname);
            Function* f = (Function*)Vector_Get(&ll->ev.fs,it);
            return f;
        }
    }
    return NULL;
}

void RexLang_LogicAddPath(RexLang* ll){
    if(ll->ev.sc.range>=ll->logicpath.size){
        while(ll->ev.sc.range>=ll->logicpath.size)
            Vector_Push(&ll->logicpath,(LogicBlock[]){{ 0,0 }});
    }else{
        LogicBlock* r = (LogicBlock*)Vector_Get(&ll->logicpath,ll->ev.sc.range);
        r->count++;
    }
}
void RexLang_LogicAddExtend(RexLang* ll){
    if(ll->ev.sc.range>=ll->logicpath.size){
        while(ll->ev.sc.range>=ll->logicpath.size) Vector_Push(&ll->logicpath,(LogicBlock[]){{ 0,0 }});
    }else{
        LogicBlock* r = (LogicBlock*)Vector_Get(&ll->logicpath,ll->ev.sc.range);
        r->extend++;
    }
}
int RexLang_GetLogicPath(RexLang* ll,Range r){
    if(r<0){
        return -1;
    }
    if(r>=ll->logicpath.size){
        while(ll->ev.sc.range>=ll->logicpath.size) Vector_Push(&ll->logicpath,(LogicBlock[]){{ 0,0 }});
        return 0;
    }
    return ((LogicBlock*)Vector_Get(&ll->logicpath,r))->count;
}
int RexLang_GetLogicExtend(RexLang* ll,Range r){
    if(r<0){
        return -1;
    }
    if(r>=ll->logicpath.size){
        while(ll->ev.sc.range>=ll->logicpath.size) Vector_Push(&ll->logicpath,(LogicBlock[]){{ 0,0 }});
        return 0;
    }
    return ((LogicBlock*)Vector_Get(&ll->logicpath,r))->extend;
}
CStr RexLang_Logic(RexLang* ll,char se,CStr type,Range r){
    int lp = RexLang_GetLogicPath(ll,ll->ev.sc.range);
    if(CStr_Cmp(type,"ELIF")){
        int ex = RexLang_GetLogicExtend(ll,ll->ev.sc.range);
        return CStr_Format("l.%d_%s%d_%d_%d",se,type,r,lp,ex);
    }
    return CStr_Format("l.%d_%s%d_%d",se,type,r,lp);
}
void RexLang_LogicCorrection(RexLang* ll,TokenMap* tm){
    Token* t = (Token*)Vector_Get(tm,0);
    
    if(ll->logic==TOKEN_REXLANG_IF){
        //int lp = RexLang_GetLogicPath(ll,ll->ev.sc.range);
        if(t->tt==TOKEN_REXLANG_ELIF || t->tt==TOKEN_REXLANG_ELSE){
            CStr log_label = RexLang_Logic(ll,1,REXLANG_LOG,ll->ev.sc.range);

            RexLang_AppendTM(ll,TokenMap_Make((Token[]){
                Token_By(TOKEN_IRVM_JMP,"jmp"),
                Token_By(TOKEN_STRING,log_label),
                Token_Null()
            }));

            CStr_Free(&log_label);
        }else{
            CStr log_label = RexLang_Logic(ll,1,REXLANG_LOG,ll->ev.sc.range);
            
            RexLang_AppendTM(ll,TokenMap_Make((Token[]){
                Token_By(TOKEN_IRVM_TAR,"tar"),
                Token_By(TOKEN_STRING,log_label),
                Token_Null()
            }));

            CStr_Free(&log_label);
        }

        CStr if_label = RexLang_Logic(ll,1,REXLANG_IF,ll->ev.sc.range);

        RexLang_AppendTM(ll,TokenMap_Make((Token[]){
            Token_By(TOKEN_IRVM_TAR,"tar"),
            Token_By(TOKEN_STRING,if_label),
            Token_Null()
        }));

        CStr_Free(&if_label);
    }else if(ll->logic==TOKEN_REXLANG_ELIF){
        //int lp = RexLang_GetLogicPath(ll,ll->ev.sc.range);
        if(t->tt==TOKEN_REXLANG_ELIF || t->tt==TOKEN_REXLANG_ELSE){
            CStr log_label = RexLang_Logic(ll,1,REXLANG_LOG,ll->ev.sc.range);
            
            RexLang_AppendTM(ll,TokenMap_Make((Token[]){
                Token_By(TOKEN_IRVM_JMP,"jmp"),
                Token_By(TOKEN_STRING,log_label),
                Token_Null()
            }));

            CStr_Free(&log_label);
        }else{
            CStr log_label = RexLang_Logic(ll,1,REXLANG_LOG,ll->ev.sc.range);
            
            RexLang_AppendTM(ll,TokenMap_Make((Token[]){
                Token_By(TOKEN_IRVM_TAR,"tar"),
                Token_By(TOKEN_STRING,log_label),
                Token_Null()
            }));

            CStr_Free(&log_label);
        }

        CStr if_label = RexLang_Logic(ll,1,REXLANG_ELIF,ll->ev.sc.range);

        RexLang_AppendTM(ll,TokenMap_Make((Token[]){
            Token_By(TOKEN_IRVM_TAR,"tar"),
            Token_By(TOKEN_STRING,if_label),
            Token_Null()
        }));

        CStr_Free(&if_label);
    }else if(ll->logic==TOKEN_REXLANG_ELSE){
        //int lp = RexLang_GetLogicPath(ll,ll->ev.sc.range);
        CStr log_label = RexLang_Logic(ll,1,REXLANG_LOG,ll->ev.sc.range);
         
        RexLang_AppendTM(ll,TokenMap_Make((Token[]){
            Token_By(TOKEN_IRVM_TAR,"tar"),
            Token_By(TOKEN_STRING,log_label),
            Token_Null()
        }));

        CStr_Free(&log_label);
    }else if(ll->logic==TOKEN_REXLANG_WHILE){
        //int lp = RexLang_GetLogicPath(ll,ll->ev.sc.range);
        CStr start_label = RexLang_Logic(ll,0,REXLANG_WHILE,ll->ev.sc.range);
        CStr end_label = RexLang_Logic(ll,1,REXLANG_WHILE,ll->ev.sc.range);

        RexLang_AppendTM(ll,TokenMap_Make((Token[]){
            Token_By(TOKEN_IRVM_JMP,"jmp"),
            Token_By(TOKEN_STRING,start_label),
            Token_Null()
        }));

        RexLang_AppendTM(ll,TokenMap_Make((Token[]){
            Token_By(TOKEN_IRVM_TAR,"tar"),
            Token_By(TOKEN_STRING,end_label),
            Token_Null()
        }));

        CStr_Free(&end_label);
        CStr_Free(&start_label);
    }else if(ll->logic==TOKEN_REXLANG_FOR){
        //int lp = RexLang_GetLogicPath(ll,ll->ev.sc.range);
        CStr start_label = RexLang_Logic(ll,0,REXLANG_FOR,ll->ev.sc.range);
        CStr end_label = RexLang_Logic(ll,1,REXLANG_FOR,ll->ev.sc.range);
        
        RexLang_AppendTM(ll,TokenMap_Make((Token[]){
            Token_By(TOKEN_IRVM_JMP,"jmp"),
            Token_By(TOKEN_STRING,start_label),
            Token_Null()
        }));

        RexLang_AppendTM(ll,TokenMap_Make((Token[]){
            Token_By(TOKEN_IRVM_TAR,"tar"),
            Token_By(TOKEN_STRING,end_label),
            Token_Null()
        }));

        CStr_Free(&end_label);
        CStr_Free(&start_label);
        Scope_Pop(&ll->ev.sc);// first possible decl in for ... , 
    }
    ll->logic = TOKEN_NONE;
}
CStr RexLang_LogicCmp(RexLang* ll,CStr type){
    return CStr_Format("lcmp.%d_%s",ll->lcmps++,type);
}

Boolean RexLang_TypeInt(CStr typename){
    return (
        CStr_Cmp(typename,I8_TYPE) || CStr_Cmp(typename,I16_TYPE) ||
        CStr_Cmp(typename,I32_TYPE) || CStr_Cmp(typename,I64_TYPE) ||
        CStr_Cmp(typename,U8_TYPE) || CStr_Cmp(typename,U16_TYPE) ||
        CStr_Cmp(typename,U32_TYPE) || CStr_Cmp(typename,U64_TYPE)
    );
}
Boolean RexLang_TypeFloat(CStr typename){
    return CStr_Cmp(typename,F32_TYPE) || CStr_Cmp(typename,F64_TYPE);
}

void RexLang_Destroyer(Variable* v){
    //printf("[RexLang]: Destroyer: %s -> ",v->typename);
    RexLangVariable* v_sv = (RexLangVariable*)Variable_Data(v);
    
    if(v_sv->destroy == REXLANG_DESTROY_DESTRUCT || v_sv->destroy == REXLANG_DESTROY_FULL){
        if(v_sv->storage == REXLANG_STORAGE_STACK){
            RexLang_AppendTM(v_sv->parent,TokenMap_Make((Token[]){
                Token_By(TOKEN_IRVM_DESTROY,"destroy"),
                Token_By(TOKEN_IRVM_VAR,v->name),
                Token_Null()
            }));
        }else if(v_sv->storage == REXLANG_STORAGE_STACK_BLOCK){
            RexLang_AppendTM(v_sv->parent,TokenMap_Make((Token[]){
                Token_By(TOKEN_IRVM_DESTROY,"destroyblock"),
                Token_Move(TOKEN_NUMBER,(CStr)Number_Get(v_sv->size)),
                Token_By(TOKEN_IRVM_VAR,v->name),
                Token_Null()
            }));
        }
    }

    RexLangVariable_Free(v_sv);
}
void RexLang_Cpyer(Variable* src,Variable* dst){
    //printf("[RexLang]: Cpyer!\n");
    //RexLangVariable* src_str = (RexLangVariable*)Variable_Data(src);
    //RexLangVariable* dst_str = (RexLangVariable*)Variable_Data(dst);
    //*dst_str = VariableMap_Cpy(src_str);
}
Token RexLang_Init(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Variable* v = Scope_FindVariable(&ll->ev.sc,a->str);

    //printf("[RexLang]: INIT: %s\n",a->str);
    
    if(v){
        RexLangVariable* v_sv = (RexLangVariable*)Variable_Data(v);
        *v_sv = RexLangVariable_New(ll,REXLANG_STORAGE_NONE,REXLANG_DESTROY_FULL);

        if(CStr_CmpLike(v->name,COMPILER_RETURN"*",'*')){
            v_sv->storage = REXLANG_STORAGE_SSA;
            v_sv->id = ll->ssas++;
        }else if(v->name[0] == '.'){
            v_sv->storage = REXLANG_STORAGE_SSA;
            v_sv->id = ll->ssas++;
        }

        if(v_sv->destroy == REXLANG_DESTROY_DESTRUCT || v_sv->destroy == REXLANG_DESTROY_FULL){
            if(v_sv->storage == REXLANG_STORAGE_STACK){
                RexLang_AppendTM(v_sv->parent,TokenMap_Make((Token[]){
                    Token_By(TOKEN_IRVM_ALLOCA,"alloca"),
                    Token_By(TOKEN_TYPE,v->typename),
                    Token_By(TOKEN_IRVM_VAR,v->name),
                    Token_Null()
                }));
            }else if(v_sv->storage == REXLANG_STORAGE_STACK_BLOCK){
                RexLang_AppendTM(v_sv->parent,TokenMap_Make((Token[]){
                    Token_By(TOKEN_IRVM_ALLOCA,"allocablock"),
                    Token_By(TOKEN_TYPE,v->typename),
                    Token_Move(TOKEN_NUMBER,(CStr)Number_Get(v_sv->size)),
                    Token_By(TOKEN_IRVM_VAR,v->name),
                    Token_Null()
                }));
            }else if(v_sv->storage == REXLANG_STORAGE_GLOBAL){
                
            }else if(v_sv->storage == REXLANG_STORAGE_GLOBAL_BLOCK){
                
            }
        }
    }
    return Token_Cpy(a);
}
void RexLang_FromTo_DestroyOnly(RexLang* ll,Range start,Range end) {
    for(int i = ll->ev.sc.vars.size-1;i>=0;i--){
        Variable* v = (Variable*)PVector_Get(&ll->ev.sc.vars,i);
        
        if(v->range > start && v->range<=end){
            RexLangVariable* v_sv = (RexLangVariable*)Variable_Data(v);
            
            const char destroy = v_sv->destroy;
            v_sv->destroy = REXLANG_DESTROY_DESTRUCT;
            Variable_Destroyer(v);
            v_sv->destroy = destroy;
        }
    }
}

CStr RexLang_GetFloat(RexLang* ll,Double d){
    CStr cstr = (CStr)Number_Get(*((Number*)&d));
    return cstr;
}
CStr RexLang_GetFloatStr(RexLang* ll,CStr fstr){
    Double d = Double_Parse((i8*)fstr,0);
    CStr cstr = (CStr)Number_Get(*((Number*)&d));
    return cstr;
}
Token RexLang_GetFloatToken(RexLang* ll,CStr fstr){
    return Token_Move(TOKEN_FLOAT,RexLang_GetFloatStr(ll,fstr));
}

CStr RexLang_FInstOf(RexLang* ll,Token* a,CStr inst){
    if(a->tt==TOKEN_STRING){
        Variable* v = Scope_FindVariable(&ll->ev.sc,a->str);
        if(v){
            if(RexLang_TypeFloat(v->typename)){
                return CStr_Format("f%s",inst);
            }else if(RexLang_TypeInt(v->typename)){
                return CStr_Format("fi%s",inst);
            }
        }
    }else if(a->tt==TOKEN_FLOAT){
        return CStr_Format("f%s",inst);
    }else if(a->tt==TOKEN_NUMBER){
        return CStr_Format("fi%s",inst);
    }
    return NULL;
}

Boolean RexLang_Extract(RexLang* ll,Token* a,Number* n){
    if(a->tt==TOKEN_STRING){
        return False;
    }else if(a->tt==TOKEN_NUMBER){
        *n = a->v_i64;
        return True;
    }else{
        Compiler_ErrorHandler(&ll->ev,"Number -> 1. Arg: %s is not a int type!",a->str);
        return False;
    }
}
Boolean RexLang_ExtractBool(RexLang* ll,Token* a,Boolean* b){
    if(a->tt==TOKEN_STRING){
        return False;
    }else if(a->tt==TOKEN_REXLANG_BOOLEAN){
        *b = a->v_b1;
        return True;
    }else{
        Compiler_ErrorHandler(&ll->ev,"1. Arg: %s is not a bool type!",a->str);
        return False;
    }
}
Boolean RexLang_ErrorsInArg(RexLang* ll,Token* a){
    if(a->tt==TOKEN_NONE){
        Compiler_ErrorHandler(&ll->ev,"Errors -> Tokentype of %s is NONE!",a->str);
        return 1;
    }
    if(a->tt==TOKEN_CONSTSTRING_DOUBLE){
        if(a->str == NULL){
            Compiler_ErrorHandler(&ll->ev,"Errors -> const str %s is null!",a->str);
            return 1; 
        }
    }
    if(a->tt==TOKEN_CONSTSTRING_SINGLE){
        if(a->str == NULL){
            Compiler_ErrorHandler(&ll->ev,"Errors -> const char %s is null!",a->str);
            return 1;
        }
    }
    if(a->tt==TOKEN_NUMBER){
        if(a->v_i64 == NUMBER_PARSE_ERROR){
            Compiler_ErrorHandler(&ll->ev,"Errors -> number %d is invalid!",a->v_i64);
            return 1;
        }
    }
    if(a->tt==TOKEN_FLOAT){
        if(a->v_f64 == DOUBLE_PARSE_ERROR){
            Compiler_ErrorHandler(&ll->ev,"Errors -> float %lf is invalid!",a->v_f64);
            return 1;
        }
    }
    if(a->tt==TOKEN_REXLANG_BOOLEAN){
        if(a->v_b1 == BOOL_PARSE_ERROR){
            Compiler_ErrorHandler(&ll->ev,"Errors -> bool %d is invalid!",a->v_b1);
            return 1;
        }
    }
    if(a->tt==TOKEN_STRING){
        if(Scope_FindVariable(&ll->ev.sc,a->str) == NULL){
            Compiler_ErrorHandler(&ll->ev,"Errors -> variable %s doesn't exist!",a->str);
            return 1;
        }
    }
    if(a->tt==TOKEN_FUNCTIONPOINTER){
        if(Scope_FindVariable(&ll->ev.sc,a->str) == NULL){
            Compiler_ErrorHandler(&ll->ev,"Errors -> function %s is null!",a->str);
            return 1;
        }
    }

    const int size = RexLang_TypeRealSize(ll,a);
    if(size <= 0){
        Compiler_ErrorHandler(&ll->ev,"Errors -> size of %s is %d!",a->str,size);
        return 1;
    }

    return 0;
}

Token RexLang_GetIRVMToken(RexLang* ll,Token* a){
    switch (a->tt){
        case TOKEN_NUMBER:{
            return Token_Cpy(a);
        }
        case TOKEN_BOOL:{
            return Token_Cpy(a);
        }
        case TOKEN_FLOAT:{
            return Token_Cpy(a);
        }
        case TOKEN_STRING:{
            Variable* v = Scope_FindVariable(&ll->ev.sc,a->str);
            RexLangVariable* v_sv = (RexLangVariable*)Variable_Data(v);
            
            /*
            if(RexLang_DrefType(ll,v->typename)){
                CStr location = RexLang_Location(ll,a->str);
                RexLang_Indentation_Appendf(ll,&ll->text,"mov %s,%s",reg,location);
                CStr typename = CStr_Cpy(v->typename);
                int drefs = RexLang_DrefTypeCount(ll,typename);
                
                for(int i = 0;i<drefs-1;i++){
                    CStr typeselector = NULL;
                    if(typename[CStr_Size(typename) - 1]=='&')  typeselector = CStr_Cpy(RexLang_DREF_64);
                    else                                        typeselector = RexLang_TypeSelector_T(ll,typename);

                    RexLang_Indentation_Appendf(ll,&ll->text,"mov %s,%s[%s]",reg,typeselector,reg);
                    CStr_Free(&typeselector);

                    CStr newtypename = CStr_PopOff(typename);
                    CStr_Set(&typename,newtypename);
                    CStr_Free(&newtypename);
                }

                CStr_Free(&location);
            }else{
            }
            */
            
            if(v->range == 0)
                return Token_By(TOKEN_IRVM_STATIC,a->str);
            else if(v->range == 1)
                return Token_Move(TOKEN_IRVM_PARAM,(CStr)Number_Get(v_sv->id));
            else{
                if(v_sv->storage == REXLANG_STORAGE_STACK)              return Token_By(TOKEN_IRVM_VAR,a->str);
                else if(v_sv->storage == REXLANG_STORAGE_STACK_BLOCK)   return Token_By(TOKEN_IRVM_VAR,a->str);
                else if(v_sv->storage == REXLANG_STORAGE_GLOBAL)        return Token_By(TOKEN_IRVM_STATIC,a->str);
                else if(v_sv->storage == REXLANG_STORAGE_GLOBAL_BLOCK)  return Token_By(TOKEN_IRVM_STATIC,a->str);
                else if(v_sv->storage == REXLANG_STORAGE_SSA)           return Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(v_sv->id));
                else if(v_sv->storage == REXLANG_STORAGE_RETURN)        return Token_Move(TOKEN_IRVM_PARAM,(CStr)Number_Get(v_sv->id));
                else if(v_sv->storage == REXLANG_STORAGE_PARAM)         return Token_Move(TOKEN_IRVM_PARAM,(CStr)Number_Get(v_sv->id));
                else                                                    return Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(v_sv->id));
            }
        }
        case TOKEN_FUNCTION:{
            return Token_By(TOKEN_IRVM_FUNCTION,a->str);
        }
        case TOKEN_CONSTSTRING_SINGLE:{
            return Token_New_I64(TOKEN_NUMBER,a->str[0]);
        }
        case TOKEN_CONSTSTRING_DOUBLE:{
            return Token_Null();
        }
        case TOKEN_IRVM_FUNCTION:  return Token_Cpy(a);
        case TOKEN_IRVM_PARAM:     return Token_Cpy(a);
        case TOKEN_IRVM_SSA:       return Token_Cpy(a);
        case TOKEN_IRVM_VAR:       return Token_Cpy(a);
        case TOKEN_IRVM_STATIC:    return Token_Cpy(a);
    }

    Compiler_ErrorHandler(&ll->ev,"token %s invalid!",a->str);
    return Token_Null();
}
Token RexLang_GetIRVMTokenSSA(RexLang* ll,Token* a){
    switch (a->tt){
        case TOKEN_NUMBER:{
            return Token_Cpy(a);
        }
        case TOKEN_BOOL:{
            return Token_Cpy(a);
        }
        case TOKEN_FLOAT:{
            return Token_Cpy(a);
        }
        case TOKEN_REXLANG_NULL:{
            return Token_New_I64(TOKEN_NUMBER,0);
        }
        case TOKEN_STRING:{
            Variable* v = Scope_FindVariable(&ll->ev.sc,a->str);
            
            /*
            if(RexLang_DrefType(ll,v->typename)){
                CStr location = RexLang_Location(ll,a->str);
                RexLang_Indentation_Appendf(ll,&ll->text,"mov %s,%s",reg,location);
                CStr typename = CStr_Cpy(v->typename);
                int drefs = RexLang_DrefTypeCount(ll,typename);
                
                for(int i = 0;i<drefs-1;i++){
                    CStr typeselector = NULL;
                    if(typename[CStr_Size(typename) - 1]=='&')  typeselector = CStr_Cpy(RexLang_DREF_64);
                    else                                        typeselector = RexLang_TypeSelector_T(ll,typename);

                    RexLang_Indentation_Appendf(ll,&ll->text,"mov %s,%s[%s]",reg,typeselector,reg);
                    CStr_Free(&typeselector);

                    CStr newtypename = CStr_PopOff(typename);
                    CStr_Set(&typename,newtypename);
                    CStr_Free(&newtypename);
                }

                CStr_Free(&location);
            }else{
            }
            */
            
            Type* type = Scope_TypeOf(&ll->ev.sc,a);
            if(!type){
                Compiler_ErrorHandler(&ll->ev,"type of %s invalid!",a->str);
                return Token_Null();
            }
            RexLangVariable* v_sv = (RexLangVariable*)Variable_Data(v);
            
            if(v_sv->storage == REXLANG_STORAGE_STACK){
                Token t_out = Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ll->ssas++));
                
                RexLang_AppendTM(ll,TokenMap_Make((Token[]){
                    Token_By(TOKEN_IRVM_LOAD,IRVM_OP_LOAD),
                    Token_By(TOKEN_TYPE,type->name),
                    Token_Cpy(&t_out),
                    Token_By(TOKEN_IRVM_VAR,a->str),
                    Token_Null()
                }));

                return t_out;
            }else if(v_sv->storage == REXLANG_STORAGE_STACK_BLOCK){
                Token t_out = Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ll->ssas++));
                
                RexLang_AppendTM(ll,TokenMap_Make((Token[]){
                    Token_By(TOKEN_IRVM_LOAD,IRVM_OP_LOAD),
                    Token_By(TOKEN_TYPE,type->name),
                    Token_Cpy(&t_out),
                    Token_By(TOKEN_IRVM_VAR,a->str),
                    Token_Null()
                }));

                return t_out;
            }else if(v_sv->storage == REXLANG_STORAGE_GLOBAL){
                Token t_out = Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ll->ssas++));
                
                RexLang_AppendTM(ll,TokenMap_Make((Token[]){
                    Token_By(TOKEN_IRVM_LOAD,IRVM_OP_LOAD),
                    Token_By(TOKEN_TYPE,type->name),
                    Token_Cpy(&t_out),
                    Token_By(TOKEN_IRVM_STATIC,a->str),
                    Token_Null()
                }));

                return t_out;
            }else if(v_sv->storage == REXLANG_STORAGE_GLOBAL_BLOCK){
                Token t_out = Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ll->ssas++));
                
                RexLang_AppendTM(ll,TokenMap_Make((Token[]){
                    Token_By(TOKEN_IRVM_LOAD,IRVM_OP_LOAD),
                    Token_By(TOKEN_TYPE,type->name),
                    Token_Cpy(&t_out),
                    Token_By(TOKEN_IRVM_STATIC,a->str),
                    Token_Null()
                }));

                return t_out;
            }else if(v_sv->storage == REXLANG_STORAGE_SSA){
                return Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(v_sv->id));
            }else if(v_sv->storage == REXLANG_STORAGE_RETURN){
                return Token_Move(TOKEN_IRVM_PARAM,(CStr)Number_Get(v_sv->id));
            }else if(v_sv->storage == REXLANG_STORAGE_PARAM){
                return Token_Move(TOKEN_IRVM_PARAM,(CStr)Number_Get(v_sv->id));
            }else{
                return Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(v_sv->id));
            }
        }
        case TOKEN_FUNCTION:{
            return Token_By(TOKEN_IRVM_FUNCTION,a->str);
        }
        case TOKEN_CONSTSTRING_SINGLE:{
            return Token_New_I64(TOKEN_NUMBER,a->str[0]);
        }
        case TOKEN_CONSTSTRING_DOUBLE:{
            return Token_Null();
        }
        case TOKEN_IRVM_FUNCTION:  return Token_Cpy(a);
        case TOKEN_IRVM_PARAM:     return Token_Cpy(a);
        case TOKEN_IRVM_SSA:       return Token_Cpy(a);
        case TOKEN_IRVM_VAR:       return Token_Cpy(a);
        case TOKEN_IRVM_STATIC:    return Token_Cpy(a);
    }

    Compiler_ErrorHandler(&ll->ev,"token %s invalid!",a->str);
    return Token_Null();
}

Token RexLang_ExecuteAss(RexLang* ll,Token* dst,Token* a){
    if(RexLang_ErrorsInArg(ll,dst)) return Token_Null();
    if(RexLang_ErrorsInArg(ll,a))   return Token_Null();

    Type* type = Scope_TypeOf(&ll->ev.sc,dst);
    if(!type) type = Scope_TypeOf(&ll->ev.sc,a);
    if(!type){
        Compiler_ErrorHandler(&ll->ev,"type of tokens invalid!");
        return Token_Null();
    }

    if(dst->tt != TOKEN_STRING){
        Compiler_ErrorHandler(&ll->ev,"assignment only to variables!");
        return Token_Null();
    }

    Token t_dst = RexLang_GetIRVMToken(ll,dst);
    if(t_dst.tt == TOKEN_IRVM_PARAM && dst->str[0] != '.'){
        Compiler_ErrorHandler(&ll->ev,"assignment not allowed to parameters!");
        Token_Free(&t_dst);
        return Token_Null();
    }

    Token ssa = RexLang_GetIRVMTokenSSA(ll,a);

    RexLang_AppendTM(ll,TokenMap_Make((Token[]){
        (t_dst.tt == TOKEN_IRVM_PARAM || t_dst.tt == TOKEN_IRVM_SSA) ? Token_By(TOKEN_IRVM_ASS,IRVM_OP_ASS) : Token_By(TOKEN_IRVM_STORE,IRVM_OP_STORE),
        Token_By(TOKEN_TYPE,type->name),
        t_dst,
        Token_Cpy(&ssa),
        Token_Null()
    }));

    return Token_By(TOKEN_STRING,t_dst.str);
}
Token RexLang_ExecuteCast(RexLang* ll,CStr tname,Token* a){
    if(RexLang_ErrorsInArg(ll,a))   return Token_Null();

    if(!tname){
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
    
        String_AppendChar(&ret,'\0');
        return Token_Move(TOKEN_CONSTSTRING_DOUBLE,(CStr)ret.Memory);
    }

    Type* type = Scope_FindType(&ll->ev.sc,tname);
    if(!type){
        Compiler_ErrorHandler(&ll->ev,"type of cast %s invalid!",tname);
        return Token_Null();
    }

    const Token ssa_a = RexLang_GetIRVMTokenSSA(ll,a);
    const Number ssa_id = ll->ssas++;
    
    CStr typename_a = RexLang_VariableType(ll,a);
    CStr stack_name = CStr_Format("%%%d",ssa_id);
    RexLang_Variable_Build_SSA(ll,stack_name,typename_a,ssa_id);
    CStr_Free(&typename_a);
    
    RexLang_AppendTM(ll,TokenMap_Make((Token[]){
        Token_By(TOKEN_IRVM_ASS,IRVM_OP_ASS),
        Token_By(TOKEN_TYPE,tname),
        Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ssa_id)),
        ssa_a,
        Token_Null()
    }));

    return Token_Move(TOKEN_STRING,stack_name);
}
Token RexLang_ExecuteAdr(RexLang* ll,Token* a){
    if(RexLang_ErrorsInArg(ll,a))   return Token_Null();

    Type* type = Scope_FindType(&ll->ev.sc,a->str);
    if(!type){
        Compiler_ErrorHandler(&ll->ev,"type of adr invalid!");
        return Token_Null();
    }

    const Number ssa_id = ll->ssas++;

    CStr typename_a = RexLang_VariableType(ll,a);
    CStr stack_name = CStr_Format("%%%d",ssa_id);
    RexLang_Variable_Build_SSA(ll,stack_name,typename_a,ssa_id);
    CStr_Free(&typename_a);
    
    RexLang_AppendTM(ll,TokenMap_Make((Token[]){
        Token_By(TOKEN_IRVM_ADR,IRVM_OP_ADR),
        Token_By(TOKEN_TYPE,type->name),
        Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ssa_id)),
        RexLang_GetIRVMToken(ll,a),
        Token_Null()
    }));

    return Token_Move(TOKEN_STRING,stack_name);
}
Token RexLang_ExecuteDrf(RexLang* ll,Token* a){
    if(RexLang_ErrorsInArg(ll,a))   return Token_Null();

    Type* type = Scope_FindType(&ll->ev.sc,a->str);
    if(!type){
        Compiler_ErrorHandler(&ll->ev,"type of adr invalid!");
        return Token_Null();
    }
    
    const Number ssa_id = ll->ssas++;
    CStr typename_a = RexLang_VariableType(ll,a);
    CStr stack_name = CStr_Format("%%%d",ssa_id);
    RexLang_Variable_Build_SSA(ll,stack_name,typename_a,ssa_id);
    CStr_Free(&typename_a);
    
    RexLang_AppendTM(ll,TokenMap_Make((Token[]){
        Token_By(TOKEN_IRVM_ASS,IRVM_OP_ASS),
        Token_By(TOKEN_TYPE,type->name),
        Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ssa_id)),
        RexLang_GetIRVMToken(ll,a),
        Token_Null()
    }));

    return Token_Move(TOKEN_STRING,stack_name);
}
Token RexLang_Execute4(RexLang* ll,CStr inst,Token* a){
    if(RexLang_ErrorsInArg(ll,a))   return Token_Null();

    Type* type = Scope_TypeOf(&ll->ev.sc,a);
    if(!type){
        Compiler_ErrorHandler(&ll->ev,"type of tokens invalid!");
        return Token_Null();
    }

    const Token ssa_a = RexLang_GetIRVMTokenSSA(ll,a);
    const Number ssa_id = ll->ssas++;

    CStr typename_a = RexLang_VariableType(ll,a);
    CStr stack_name = CStr_Format("%%%d",ssa_id);
    RexLang_Variable_Build_SSA(ll,stack_name,typename_a,ssa_id);
    CStr_Free(&typename_a);
    
    RexLang_AppendTM(ll,TokenMap_Make((Token[]){
        IRVM_GetInst(&ll->vm,inst),
        Token_By(TOKEN_TYPE,type->name),
        Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ssa_id)),
        ssa_a,
        Token_Null()
    }));

    return Token_Move(TOKEN_STRING,stack_name);
}
Token RexLang_Execute5(RexLang* ll,CStr inst,Token* a,Token* b){
    if(RexLang_ErrorsInArg(ll,a))   return Token_Null();
    if(RexLang_ErrorsInArg(ll,b))   return Token_Null();

    Type* type = Scope_TypeOf(&ll->ev.sc,a);
    if(!type) type = Scope_TypeOf(&ll->ev.sc,b);

    if(!type){
        Compiler_ErrorHandler(&ll->ev,"type of tokens invalid!");
        return Token_Null();
    }

    const Token ssa_a = RexLang_GetIRVMTokenSSA(ll,a);
    const Token ssa_b = RexLang_GetIRVMTokenSSA(ll,b);

    const Number ssa_id = ll->ssas++;
    CStr typename_a = RexLang_VariableType(ll,a);
    CStr stack_name = CStr_Format("%%%d",ssa_id);
    RexLang_Variable_Build_SSA(ll,stack_name,typename_a,ssa_id);
    CStr_Free(&typename_a);
    
    RexLang_AppendTM(ll,TokenMap_Make((Token[]){
        IRVM_GetInst(&ll->vm,inst),
        Token_By(TOKEN_TYPE,type->name),
        Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ssa_id)),
        ssa_a,
        ssa_b,
        Token_Null()
    }));

    return Token_Move(TOKEN_STRING,stack_name);
}
Token RexLang_ExecuteAss4(RexLang* ll,CStr inst,Token* a){
    Token ssa = RexLang_Execute4(ll,inst,a);
    const Token ret = RexLang_ExecuteAss(ll,a,&ssa);
    Token_Free(&ssa);
    return ret;
}
Token RexLang_ExecuteAss5(RexLang* ll,CStr inst,Token* a,Token* b){
    Token ssa = RexLang_Execute5(ll,inst,a,b);
    const Token ret = RexLang_ExecuteAss(ll,a,&ssa);
    Token_Free(&ssa);
    return ret;
}

Token RexLang_ExecuteLot(RexLang* ll,Token* a){
    if(RexLang_ErrorsInArg(ll,a))   return Token_Null();

    Type* type = Scope_TypeOf(&ll->ev.sc,a);
    if(!type){
        Compiler_ErrorHandler(&ll->ev,"type of tokens invalid!");
        return Token_Null();
    }

    const Token ssa_a = RexLang_GetIRVMTokenSSA(ll,a);
    const Number ssa_id = ll->ssas++;

    CStr typename_a = RexLang_VariableType(ll,a);
    CStr stack_name = CStr_Format("%%%d",ssa_id);
    RexLang_Variable_Build_SSA(ll,stack_name,typename_a,ssa_id);
    CStr_Free(&typename_a);
    
    RexLang_AppendTM(ll,TokenMap_Make((Token[]){
        Token_By(TOKEN_IRVM_NOT,IRVM_OP_NOT),
        Token_By(TOKEN_TYPE,type->name),
        Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ssa_id)),
        ssa_a,
        Token_Null()
    }));

    return Token_Move(TOKEN_STRING,stack_name);
}
Token RexLang_ExecuteLnd(RexLang* ll,Token* a,Token* b){
    if(RexLang_ErrorsInArg(ll,a))   return Token_Null();
    if(RexLang_ErrorsInArg(ll,b))   return Token_Null();

    Type* type = Scope_TypeOf(&ll->ev.sc,a);
    if(!type) type = Scope_TypeOf(&ll->ev.sc,b);

    if(!type){
        Compiler_ErrorHandler(&ll->ev,"type of tokens invalid!");
        return Token_Null();
    }

    const Token ssa_a = RexLang_GetIRVMTokenSSA(ll,a);
    const Token ssa_b = RexLang_GetIRVMTokenSSA(ll,b);

    const Number ssa_id = ll->ssas++;
    CStr typename_a = RexLang_VariableType(ll,a);
    CStr stack_name = CStr_Format("%%%d",ssa_id);
    RexLang_Variable_Build_SSA(ll,stack_name,typename_a,ssa_id);
    CStr_Free(&typename_a);
    
    RexLang_AppendTM(ll,TokenMap_Make((Token[]){
        Token_By(TOKEN_IRVM_AND,IRVM_OP_AND),
        Token_By(TOKEN_TYPE,type->name),
        Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ssa_id)),
        ssa_a,
        ssa_b,
        Token_Null()
    }));

    return Token_Move(TOKEN_STRING,stack_name);
}
Token RexLang_ExecuteLor(RexLang* ll,Token* a,Token* b){
    if(RexLang_ErrorsInArg(ll,a))   return Token_Null();
    if(RexLang_ErrorsInArg(ll,b))   return Token_Null();

    Type* type = Scope_TypeOf(&ll->ev.sc,a);
    if(!type) type = Scope_TypeOf(&ll->ev.sc,b);

    if(!type){
        Compiler_ErrorHandler(&ll->ev,"type of tokens invalid!");
        return Token_Null();
    }

    const Token ssa_a = RexLang_GetIRVMTokenSSA(ll,a);
    const Token ssa_b = RexLang_GetIRVMTokenSSA(ll,b);

    const Number ssa_id = ll->ssas++;
    CStr typename_a = RexLang_VariableType(ll,a);
    CStr stack_name = CStr_Format("%%%d",ssa_id);
    RexLang_Variable_Build_SSA(ll,stack_name,typename_a,ssa_id);
    CStr_Free(&typename_a);
    
    RexLang_AppendTM(ll,TokenMap_Make((Token[]){
        Token_By(TOKEN_IRVM_OR,IRVM_OP_OR),
        Token_By(TOKEN_TYPE,type->name),
        Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ssa_id)),
        ssa_a,
        ssa_b,
        Token_Null()
    }));

    return Token_Move(TOKEN_STRING,stack_name);
}

#endif //!REXLANGASM_H