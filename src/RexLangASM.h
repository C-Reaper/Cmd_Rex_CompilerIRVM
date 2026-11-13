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
    char* libpath;
    char* genpath;
    char* src;
    char* output;
    Vec_CStr return_ssas;
    unsigned int ssas;
    TT_Type logic;
    TT_Type PADD1;
    unsigned int lcmps;
    unsigned int PADD2;
} RexLang;

Boolean RexLang_StructType(RexLang* ll,CStr type){
    const int size = CStr_Size(type);
    
    if(type[size - 1] == '*')           return False;
    if(type[size - 1] == '&'){
        CStr dtype = CStr_Sub(type,0,size - 1);
        Boolean ret = RexLang_StructType(ll,dtype);
        CStr_Free(&dtype);
        return ret;
    }else if(CStr_Cmp(type,BOOL_TYPE))  return False;
    else if(CStr_Cmp(type,I8_TYPE))     return False;
    else if(CStr_Cmp(type,I16_TYPE))    return False;
    else if(CStr_Cmp(type,I32_TYPE))    return False;
    else if(CStr_Cmp(type,I64_TYPE))    return False;
    else if(CStr_Cmp(type,U8_TYPE))     return False;
    else if(CStr_Cmp(type,U16_TYPE))    return False;
    else if(CStr_Cmp(type,U32_TYPE))    return False;
    else if(CStr_Cmp(type,U64_TYPE))    return False;
    else                                return True;
}
CStr RexLang_Type(RexLang* ll,CStr type){
    const int size = CStr_Size(type);
    
    if(type[size - 1] == '*')           return IRVM_TYPE_POINTER;
    if(type[size - 1] == '&'){
        CStr dtype = CStr_Sub(type,0,size - 1);
        CStr ret = RexLang_Type(ll,dtype);
        CStr_Free(&dtype);
        return ret;
    }else if(CStr_Cmp(type,BOOL_TYPE))  return IRVM_TYPE_BOOL;
    else if(CStr_Cmp(type,I8_TYPE))     return IRVM_TYPE_I8;
    else if(CStr_Cmp(type,I16_TYPE))    return IRVM_TYPE_I16;
    else if(CStr_Cmp(type,I32_TYPE))    return IRVM_TYPE_I32;
    else if(CStr_Cmp(type,I64_TYPE))    return IRVM_TYPE_I64;
    else if(CStr_Cmp(type,U8_TYPE))     return IRVM_TYPE_U8;
    else if(CStr_Cmp(type,U16_TYPE))    return IRVM_TYPE_U16;
    else if(CStr_Cmp(type,U32_TYPE))    return IRVM_TYPE_U32;
    else if(CStr_Cmp(type,U64_TYPE))    return IRVM_TYPE_U64;
    else if(CStr_Cmp(type,VOID_TYPE))   return IRVM_TYPE_VOID;
    else{
        Compiler_ErrorHandler(&ll->ev,"type %s invalid!",type);
        return NULL;
    }
}

int RexLang_UnfoldTypeCount(RexLang* ll,CStr name) {
    if(!name) return 0;
    
    Type* const t = Scope_FindType(&ll->ev.sc,name);
    if(!t) return 0;

    if(t->related.size > 0){
        int count = 0;
        
        for(int i = 0;i<t->related.size;i++){
            Member* m = (Member*)Vector_Get(&t->related,i);
            count += RexLang_UnfoldTypeCount(ll,m->type);
        }

        return count;
    }else if(CStr_Cmp(name,VOID_TYPE)){
        return 0;
    }else{
        return 1;
    }
}
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
    }else if(CStr_Cmp(name,VOID_TYPE)){
        return Vec_CStr_New();
    }else{
        return Vec_CStr_Make((CStr[]){ CStr_Cpy(RexLang_Type(ll,name)) },1);
    }
}
Vec_CStr RexLang_UnfoldMembers(RexLang* ll,CStr name) {
    if(!name) return Vec_CStr_New();
    
    Type* const t = Scope_FindType(&ll->ev.sc,name);
    if(!t || t->related.size == 0) return Vec_CStr_New();

    Vec_CStr types = Vec_CStr_New();
        
    for(int i = 0;i<t->related.size;i++){
        Member* m = (Member*)Vector_Get(&t->related,i);
        
        Vec_CStr m_types = RexLang_UnfoldMembers(ll,m->type);
        
        if(m_types.size == 0){
            Vec_CStr_Push(&types,m->name);
        }else{
            CStr mdot = CStr_Concat(m->name,".");
            Vec_CStr_AddPre(&m_types,mdot);
            CStr_Free(&mdot);

            Vec_CStr_Add(&types,&m_types);
        }

        Vec_CStr_Free(&m_types);
    }

    return types;
}
Vec_CStr RexLang_UnfoldMemberTypes(RexLang* ll,CStr name) {
    if(!name) return Vec_CStr_New();
    
    Type* const t = Scope_FindType(&ll->ev.sc,name);
    if(!t || t->related.size == 0) return Vec_CStr_New();

    Vec_CStr types = Vec_CStr_New();
        
    for(int i = 0;i<t->related.size;i++){
        Member* m = (Member*)Vector_Get(&t->related,i);
        
        Vec_CStr m_types = RexLang_UnfoldMemberTypes(ll,m->type);
        
        if(m_types.size == 0){
            Vec_CStr_Push(&types,m->type);
        }else{
            Vec_CStr_Add(&types,&m_types);
        }

        Vec_CStr_Free(&m_types);
    }

    return types;
}
Vec_CStr RexLang_GetMembers(RexLang* ll,CStr name) {
    if(!name) return Vec_CStr_New();
    
    Type* const t = Scope_FindType(&ll->ev.sc,name);
    if(!t || t->related.size == 0) return Vec_CStr_New();

    Vec_CStr types = Vec_CStr_New();
        
    for(int i = 0;i<t->related.size;i++){
        Member* m = (Member*)Vector_Get(&t->related,i);
        Vec_CStr_Push(&types,m->name);
    }

    return types;
}
Vec_CStr RexLang_GetMemberTypes(RexLang* ll,CStr name) {
    if(!name) return Vec_CStr_New();
    
    Type* const t = Scope_FindType(&ll->ev.sc,name);
    if(!t || t->related.size == 0) return Vec_CStr_New();

    Vec_CStr types = Vec_CStr_New();
        
    for(int i = 0;i<t->related.size;i++){
        Member* m = (Member*)Vector_Get(&t->related,i);
        Vec_CStr_Push(&types,m->type);
    }

    return types;
}

Boolean RexLang_PointerType(CStr name){
    const int size = CStr_Size(name);
    for(int i = size - 1;i>=0;i--){
        if(name[i]=='*') return 1;
        if(name[i]=='&') continue;
        return 0;
    }
    return 0;
}
Boolean RexLang_DrefType(CStr name){
    if(!name) return 0;
    const int size = CStr_Size(name);
    if(name[size-1]=='&') return 1;
    return 0;
}
Boolean RexLang_MultiDrefType(CStr name){
    if(!name) return 0;
    const int size = CStr_Size(name);
    if(size > 2 && name[size - 1]=='&' && name[size - 2]=='&') return 1;
    return 0;
}
int RexLang_DrefTypeCount(CStr name){
    int size = CStr_Size(name);
    for(int i = size - 1;i>=0;i--)
        if(name[i]!='&') return (size - 1) - i;
    return size;
}
CStr RexLang_TypeOfDref(CStr name){
    int size = CStr_Size(name);
    for(int i = size-1;i>=0;i--){
        if(name[i]=='&') continue;
        return CStr_Cpy_From_To(name,0,i+1);
    }
    return NULL;
}
CStr RexLang_TypeOfPointer(CStr name){
    int size = CStr_Size(name);
    for(int i = size-1;i>=0;i--){
        if(name[i]=='&' || name[i]=='*') continue;
        return CStr_Cpy_From_To(name,0,i+1);
    }
    return NULL;
}
int RexLang_Size(RexLang* ll,CStr name){
    if(RexLang_DrefType(name)){
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
char RexLang_CompressStackType(TokenMap* tm,int start){
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

CStr RexLang_VariableType(RexLang* ll,Token* a){
    if(a->tt==TOKEN_STRING){
        Variable* v_a = Scope_FindVariable(&ll->ev.sc,a->str);
        if(v_a) return RexLang_TypeOfDref(v_a->typename);
    }
    if(a->tt==TOKEN_BOOL)               return CStr_Cpy(BOOL_TYPE);
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
CStr RexLang_TokenType(RexLang* ll,Token* a){
    if(a->tt==TOKEN_STRING){
        Variable* v_a = Scope_FindVariable(&ll->ev.sc,a->str);
        if(v_a) return v_a->typename;
    }
    if(a->tt==TOKEN_BOOL)               return BOOL_TYPE;
    if(a->tt==TOKEN_NUMBER)             return I64_TYPE;
    if(a->tt==TOKEN_CONSTSTRING_DOUBLE) return STR_TYPE;
    if(a->tt==TOKEN_CONSTSTRING_SINGLE) return CHAR_TYPE;
    if(a->tt==TOKEN_FLOAT)              return F64_TYPE;
    if(a->tt==TOKEN_FUNCTIONPOINTER)    return NULL_TYPE;
    if(a->tt==TOKEN_REXLANG_NULL)       return NULL_TYPE;
    return NULL;
}
CStr RexLang_GetPtrType(RexLang* ll,Token* a){
    CStr type = RexLang_TokenType(ll,a);
    if(type) return CStr_Concat(type,"*");
    return NULL;
}
CStr RexLang_GetDrefType(RexLang* ll,CStr type){
    const int size = CStr_Size(type);
    
    if(type[size - 1] == '*' || type[size - 1] == '&'){
        return CStr_Sub(type,0,size - 1);
    }else{
        Compiler_ErrorHandler(&ll->ev,"type %s not a pointer!",type);
        return NULL;
    }
}
CStr RexLang_GetDrefOfType(RexLang* ll,CStr type){
    const int size = CStr_Size(type);
    
    if(type[size - 1] == '*'){
        CStr dtype = CStr_Cpy(type);
        dtype[size - 1] = '&';
        return dtype;
    }else if(type[size - 1] == '&' || type[size - 1] == '*'){
        for(int i = size - 1;i>=0;i--){
            if(type[i]=='&')
                continue;
            else if(type[i] = '*'){
                CStr dtype = CStr_Sub(type,0,i + 1);
                dtype[i] = '&';
                return dtype;
            }else{
                Compiler_ErrorHandler(&ll->ev,"type %s not a pointer!",type);
                return NULL;
            }
        }
        return CStr_Cpy(type);
    }else{
        Compiler_ErrorHandler(&ll->ev,"type %s not a pointer!",type);
        return NULL;
    }
}

int RexLang_TypeRealSize(RexLang* ll,Token* a){
    CStr type = RexLang_VariableType(ll,a);
    if(!type) return 0;

    if(RexLang_DrefType(type)){
        CStr ntype = RexLang_TypeOfDref(type);
        CStr_Set(&type,ntype);
        CStr_Free(&ntype);
    }

    int size = RexLang_Size(ll,type);
    CStr_Free(&type);
    return size;
}
CStr RexLang_FunctionName(CStr name){
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

    CStr fname = RexLang_FunctionName(name);
    Vector_Push(&ll->vm.fns,(IRVM_Function[]){
        IRVM_Function_Make(
            IRVM_ACCESS_NONE,
            fname,
            (CStr*)rtypes.Memory,
            rtypes.size,
            (CStr*)ptypes.Memory,
            ptypes.size
        )
    });
    CStr_Free(&fname);
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
CStr RexLang_BuildGlobal(RexLang* ll,CStr name,CStr type,int size){
    //CStr g_name = CStr_Format("g.%s",name);
    Vector_Push(&ll->vm.info_statics,(IRVM_DataInfo[]){ IRVM_DataInfo_New_Resv(name,type,size) });
    return CStr_Cpy(name);
}

RexLang_Storage RexLang_RealStorage(RexLang* ll,Range r,CStr type,RexLang_Storage storage){
    if(storage == REXLANG_STORAGE_AUTO && r == 0)                               storage = REXLANG_STORAGE_GLOBAL;
    if(storage == REXLANG_STORAGE_STACK && RexLang_StructType(ll,type))         storage = REXLANG_STORAGE_STACK_STRUCT;
    if(storage == REXLANG_STORAGE_GLOBAL && RexLang_StructType(ll,type))        storage = REXLANG_STORAGE_GLOBAL_STRUCT;
    if(storage == REXLANG_STORAGE_STACK_BLOCK && RexLang_StructType(ll,type))   storage = REXLANG_STORAGE_STACK_STRUCT_BLOCK;
    if(storage == REXLANG_STORAGE_GLOBAL_BLOCK && RexLang_StructType(ll,type))  storage = REXLANG_STORAGE_GLOBAL_STRUCT_BLOCK;
    return storage;
}
void RexLang_Variable_Decl(RexLang* ll,CStr name,CStr type,RexLang_Storage storage,uint64_t size){
    if(storage == REXLANG_STORAGE_STACK){
        RexLang_AppendTM(ll,TokenMap_Make((Token[]){
            Token_By(TOKEN_IRVM_ALLOCA,IRVM_OP_ALLOCA),
            Token_By(TOKEN_TYPE,RexLang_Type(ll,type)),
            Token_By(TOKEN_IRVM_VAR,name),
            Token_Null()
        }));
    }else if(storage == REXLANG_STORAGE_STACK_BLOCK){
        RexLang_AppendTM(ll,TokenMap_Make((Token[]){
            Token_By(TOKEN_IRVM_ALLOCS,IRVM_OP_ALLOCS),
            Token_New_I64(TOKEN_NUMBER,RexLang_Size(ll,type) * size),
            Token_By(TOKEN_IRVM_VAR,name),
            Token_Null()
        }));
    }else if(storage == REXLANG_STORAGE_STACK_STRUCT){
        RexLang_AppendTM(ll,TokenMap_Make((Token[]){
            Token_By(TOKEN_IRVM_ALLOCS,IRVM_OP_ALLOCS),
            Token_New_I64(TOKEN_NUMBER,RexLang_Size(ll,type)),
            Token_By(TOKEN_IRVM_VAR,name),
            Token_Null()
        }));
    }else if(storage == REXLANG_STORAGE_STACK_STRUCT_BLOCK){
        RexLang_AppendTM(ll,TokenMap_Make((Token[]){
            Token_By(TOKEN_IRVM_ALLOCS,IRVM_OP_ALLOCS),
            Token_New_I64(TOKEN_NUMBER,RexLang_Size(ll,type) * size),
            Token_By(TOKEN_IRVM_VAR,name),
            Token_Null()
        }));
    }else if(storage == REXLANG_STORAGE_GLOBAL){
                
    }else if(storage == REXLANG_STORAGE_GLOBAL_BLOCK){
        
    }else if(storage == REXLANG_STORAGE_GLOBAL_STRUCT){
                
    }else if(storage == REXLANG_STORAGE_GLOBAL_STRUCT_BLOCK){
        
    }
}
void RexLang_Variable_BuildRange(RexLang* ll,CStr name,CStr type,Range r,RexLang_Storage storage,RexLang_Destroy destroy){
    if(r == 0){
        const RexLang_Storage real_storage = RexLang_RealStorage(ll,r,type,storage);
        CStr g_value = RexLang_BuildGlobal(ll,name,type,1);
        
        RexLangVariable sv = RexLangVariable_New(ll,real_storage,destroy);
        sv.global = g_value;

        Scope_BuildInitVariableRange(&ll->ev.sc,name,type,r,&sv);
    }else{
        const RexLang_Storage real_storage = RexLang_RealStorage(ll,r,type,storage);
        RexLang_Variable_Decl(ll,name,type,real_storage,0U);
        
        Scope_BuildInitVariableRange(&ll->ev.sc,name,type,r,(RexLangVariable[]){
            RexLangVariable_New(
                ll,
                real_storage,
                destroy
            )
        });
    }
}
void RexLang_Variable_BuildRange_Block(RexLang* ll,CStr name,CStr type,uint32_t size,Range r,RexLang_Storage storage,RexLang_Destroy destroy){
    CStr btype = RexLang_GetDrefType(ll,type);
    
    if(r == 0){
        const RexLang_Storage real_storage = RexLang_RealStorage(ll,r,btype,storage);
        CStr g_value = RexLang_BuildGlobal(ll,name,btype,size);
        
        RexLangVariable sv = RexLangVariable_New(ll,real_storage,destroy);
        sv.global = g_value;

        Scope_BuildInitVariableRange(&ll->ev.sc,name,type,r,&sv);
    }else{
        const RexLang_Storage real_storage = RexLang_RealStorage(ll,r,btype,storage);
        RexLang_Variable_Decl(ll,name,btype,real_storage,size);

        RexLangVariable sv = RexLangVariable_New(ll,real_storage,destroy);
        sv.size = size;
        
        Scope_BuildInitVariableRange(&ll->ev.sc,name,type,r,&sv);
    }

    CStr_Free(&btype);
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
    sv->id = i;
}
void RexLang_Variable_Build_SSA(RexLang* ll,CStr name,CStr type,int ssa){
    RexLang_Variable_BuildRange(ll,name,type,ll->ev.sc.range,REXLANG_STORAGE_SSA,REXLANG_DESTROY_FULL);

    Variable* v = Scope_FindVariable(&ll->ev.sc,name);
    RexLangVariable* sv = (RexLangVariable*)Variable_Data(v);
    sv->id = ssa;
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
                Token_By(TOKEN_IRVM_DESTROY,IRVM_OP_DESTROY),
                Token_By(TOKEN_IRVM_VAR,v->name),
                Token_Null()
            }));
        }else if(v_sv->storage == REXLANG_STORAGE_STACK_BLOCK){
            RexLang_AppendTM(v_sv->parent,TokenMap_Make((Token[]){
                Token_By(TOKEN_IRVM_DESTROY,IRVM_OP_DESTROY),
                Token_By(TOKEN_IRVM_VAR,v->name),
                Token_Null()
            }));
        }else if(v_sv->storage == REXLANG_STORAGE_STACK_STRUCT){
            RexLang_AppendTM(v_sv->parent,TokenMap_Make((Token[]){
                Token_By(TOKEN_IRVM_DESTROY,IRVM_OP_DESTROY),
                Token_By(TOKEN_IRVM_VAR,v->name),
                Token_Null()
            }));
        }else if(v_sv->storage == REXLANG_STORAGE_STACK_STRUCT_BLOCK){
            RexLang_AppendTM(v_sv->parent,TokenMap_Make((Token[]){
                Token_By(TOKEN_IRVM_DESTROY,IRVM_OP_DESTROY),
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

        if(v->range == 1){
            if(CStr_CmpLike(v->name,COMPILER_RETURN"*",'*')){
                v_sv->storage = REXLANG_STORAGE_RETURN;
                v_sv->id = ll->ssas;

                Vec_CStr types = RexLang_UnfoldMemberTypes(ll,v->typename);
                if(types.size == 0) ll->ssas++;
                else                ll->ssas += types.size;
                Vec_CStr_Free(&types);
            }else{
                v_sv->storage = REXLANG_STORAGE_PARAM;
                v_sv->id = ll->ssas;

                Vec_CStr types = RexLang_UnfoldMemberTypes(ll,v->typename);
                if(types.size == 0) ll->ssas++;
                else                ll->ssas += types.size;
                Vec_CStr_Free(&types);
            }
        }else{
            if(CStr_CmpLike(v->name,COMPILER_RETURN"*",'*')){
                v_sv->storage = REXLANG_STORAGE_SSA;
                v_sv->id = ll->ssas;

                Vec_CStr types = RexLang_UnfoldMemberTypes(ll,v->typename);
                if(types.size == 0) ll->ssas++;
                else                ll->ssas += types.size;
                Vec_CStr_Free(&types);
            }else if(v->name[0] == '.'){
                v_sv->storage = REXLANG_STORAGE_SSA;
                v_sv->id = ll->ssas;

                Vec_CStr types = RexLang_UnfoldMemberTypes(ll,v->typename);
                if(types.size == 0) ll->ssas++;
                else                ll->ssas += types.size;
                Vec_CStr_Free(&types);
            }
        }

        if(v_sv->destroy == REXLANG_DESTROY_DESTRUCT || v_sv->destroy == REXLANG_DESTROY_FULL){
            RexLang_Variable_Decl(ll,v->name,v->typename,v_sv->storage,0U);
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
Token RexLang_GetConstStr(RexLang* ll,Token* conststr){
    Vector values = Vector_New(sizeof(uint64_t));
    IRVM_Static_Str(&values,conststr->str);

    CStr name = CStr_Format("cstr.%d",(Number)ll->vm.info_statics.size);
    Vector_Push(&ll->vm.info_statics,(IRVM_DataInfo[]){ IRVM_DataInfo_New_Data(name,IRVM_TYPE_I8,values) });
    return Token_Move(TOKEN_IRVM_STATIC,name);
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
    if(a->tt==TOKEN_BOOL){
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

Token RexLang_GetOpIRVMToken(RexLang* ll,Token* a,Token* t_a){
    CStr type_dst = RexLang_TokenType(ll,a);
    
    return ((t_a->tt == TOKEN_IRVM_PARAM || t_a->tt == TOKEN_IRVM_SSA) && !RexLang_DrefType(type_dst) ?
        Token_By(TOKEN_IRVM_ASS,IRVM_OP_ASS) :
        Token_By(TOKEN_IRVM_STORE,IRVM_OP_STORE)
    );
}
Token RexLang_GetLdIRVMToken(RexLang* ll,Token* a,Token* t_a){
    CStr type_dst = RexLang_TokenType(ll,a);
    
    return ((t_a->tt == TOKEN_IRVM_PARAM || t_a->tt == TOKEN_IRVM_SSA) && !RexLang_DrefType(type_dst) ?
        Token_By(TOKEN_IRVM_ASS,IRVM_OP_ASS) :
        Token_By(TOKEN_IRVM_LOAD,IRVM_OP_LOAD)
    );
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
        case TOKEN_REXLANG_NULL:{
            return Token_New_I64(TOKEN_NUMBER,0);
        }
        case TOKEN_STRING:{
            Variable* v = Scope_FindVariable(&ll->ev.sc,a->str);
            RexLangVariable* v_sv = (RexLangVariable*)Variable_Data(v);
            
            if(v_sv->storage == REXLANG_STORAGE_STACK)
                return Token_By(TOKEN_IRVM_VAR,a->str);
            else if(v_sv->storage == REXLANG_STORAGE_STACK_BLOCK)
                return Token_By(TOKEN_IRVM_VAR,a->str);
            else if(v_sv->storage == REXLANG_STORAGE_STACK_STRUCT)
                return Token_By(TOKEN_IRVM_VAR,a->str);
            else if(v_sv->storage == REXLANG_STORAGE_STACK_STRUCT_BLOCK)
                return Token_By(TOKEN_IRVM_VAR,a->str);
            else if(v_sv->storage == REXLANG_STORAGE_GLOBAL)
                return Token_By(TOKEN_IRVM_STATIC,a->str);
            else if(v_sv->storage == REXLANG_STORAGE_GLOBAL_BLOCK)
                return Token_By(TOKEN_IRVM_STATIC,a->str);
            else if(v_sv->storage == REXLANG_STORAGE_GLOBAL_STRUCT)
                return Token_By(TOKEN_IRVM_STATIC,a->str);
            else if(v_sv->storage == REXLANG_STORAGE_GLOBAL_STRUCT_BLOCK)
                return Token_By(TOKEN_IRVM_STATIC,a->str);
            else if(v_sv->storage == REXLANG_STORAGE_SSA)
                return Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(v_sv->id));
            else if(v_sv->storage == REXLANG_STORAGE_RETURN){
                Token ssa = Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ll->ssas++));
                Vec_CStr_Push(&ll->return_ssas,CStr_Cpy(ssa.str));
                return ssa;
            }
            else if(v_sv->storage == REXLANG_STORAGE_PARAM){
                return Token_Move(TOKEN_IRVM_PARAM,(CStr)Number_Get(v_sv->id));
            }else
                return Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(v_sv->id));
        }
        case TOKEN_FUNCTION:{
            return Token_By(TOKEN_IRVM_FUNCTION,a->str);
        }
        case TOKEN_CONSTSTRING_SINGLE:{
            return Token_New_I64(TOKEN_NUMBER,a->str[0]);
        }
        case TOKEN_CONSTSTRING_DOUBLE:{
            return RexLang_GetConstStr(ll,a);
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
Token RexLang_GetIRVMTokenTarget(RexLang* ll,Token* a){
    if(a->tt == TOKEN_STRING){
        Variable* v = Scope_FindVariable(&ll->ev.sc,a->str);
        RexLangVariable* v_sv = (RexLangVariable*)Variable_Data(v);
        CStr type = RexLang_TokenType(ll,a);
        
        if(RexLang_MultiDrefType(type)){
            Token dst = Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(v_sv->id));
            CStr dtype = CStr_Cpy(RexLang_TokenType(ll,a));
            
            while(RexLang_MultiDrefType(dtype)){
                const Number ssa_id = ll->ssas++;
                Token dref = Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ssa_id));
                
                RexLang_AppendTM(ll,TokenMap_Make((Token[]){
                    Token_By(TOKEN_IRVM_LOAD,IRVM_OP_LOAD),
                    Token_By(TOKEN_TYPE,IRVM_TYPE_POINTER),
                    dref,
                    dst,
                    Token_Null()
                }));

                dst = RexLang_GetIRVMToken(ll,&dref);

                CStr swap = RexLang_GetDrefType(ll,dtype);
                CStr_Free(&dtype);
                dtype = swap;
            }

            CStr_Free(&dtype);
            return dst;
        }
    }

    return RexLang_GetIRVMToken(ll,a);
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
            RexLangVariable* v_sv = (RexLangVariable*)Variable_Data(v);
            CStr type = RexLang_TokenType(ll,a);
            
            if(v_sv->storage == REXLANG_STORAGE_STACK){
                Token t_out = Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ll->ssas++));
                
                RexLang_AppendTM(ll,TokenMap_Make((Token[]){
                    Token_By(TOKEN_IRVM_LOAD,IRVM_OP_LOAD),
                    Token_By(TOKEN_TYPE,RexLang_Type(ll,type)),
                    Token_Cpy(&t_out),
                    Token_By(TOKEN_IRVM_VAR,a->str),
                    Token_Null()
                }));

                return t_out;
            }else if(v_sv->storage == REXLANG_STORAGE_STACK_BLOCK){
                Token t_out = Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ll->ssas++));
                
                RexLang_AppendTM(ll,TokenMap_Make((Token[]){
                    Token_By(TOKEN_IRVM_ASS,IRVM_OP_ASS),
                    Token_By(TOKEN_TYPE,RexLang_Type(ll,type)),
                    Token_Cpy(&t_out),
                    Token_By(TOKEN_IRVM_VAR,a->str),
                    Token_Null()
                }));

                return t_out;
            }else if(v_sv->storage == REXLANG_STORAGE_STACK_STRUCT){
                Compiler_ErrorHandler(&ll->ev,"struct %s can not be moved in to single ssa!",a->str);
                return Token_Null();
            }else if(v_sv->storage == REXLANG_STORAGE_STACK_STRUCT_BLOCK){
                Token t_out = Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ll->ssas++));
                
                RexLang_AppendTM(ll,TokenMap_Make((Token[]){
                    Token_By(TOKEN_IRVM_ASS,IRVM_OP_ASS),
                    Token_By(TOKEN_TYPE,RexLang_Type(ll,type)),
                    Token_Cpy(&t_out),
                    Token_By(TOKEN_IRVM_VAR,a->str),
                    Token_Null()
                }));

                return t_out;
            }else if(v_sv->storage == REXLANG_STORAGE_GLOBAL){
                Token t_out = Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ll->ssas++));
                
                RexLang_AppendTM(ll,TokenMap_Make((Token[]){
                    Token_By(TOKEN_IRVM_LOAD,IRVM_OP_LOAD),
                    Token_By(TOKEN_TYPE,RexLang_Type(ll,type)),
                    Token_Cpy(&t_out),
                    Token_By(TOKEN_IRVM_STATIC,a->str),
                    Token_Null()
                }));

                return t_out;
            }else if(v_sv->storage == REXLANG_STORAGE_GLOBAL_BLOCK){
                Token t_out = Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ll->ssas++));
                
                RexLang_AppendTM(ll,TokenMap_Make((Token[]){
                    Token_By(TOKEN_IRVM_ASS,IRVM_OP_ASS),
                    Token_By(TOKEN_TYPE,RexLang_Type(ll,type)),
                    Token_Cpy(&t_out),
                    Token_By(TOKEN_IRVM_STATIC,a->str),
                    Token_Null()
                }));

                return t_out;
            }else if(v_sv->storage == REXLANG_STORAGE_GLOBAL_STRUCT){
                Compiler_ErrorHandler(&ll->ev,"global struct %s can not be moved in to single ssa!",a->str);
                return Token_Null();
            }else if(v_sv->storage == REXLANG_STORAGE_GLOBAL_STRUCT_BLOCK){
                Token t_out = Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ll->ssas++));
                
                RexLang_AppendTM(ll,TokenMap_Make((Token[]){
                    Token_By(TOKEN_IRVM_ASS,IRVM_OP_ASS),
                    Token_By(TOKEN_TYPE,RexLang_Type(ll,type)),
                    Token_Cpy(&t_out),
                    Token_By(TOKEN_IRVM_STATIC,a->str),
                    Token_Null()
                }));

                return t_out;
            }else if(v_sv->storage == REXLANG_STORAGE_SSA){
                return Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(v_sv->id));
            }else if(v_sv->storage == REXLANG_STORAGE_RETURN){
                Token ssa = Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ll->ssas++));
                Vec_CStr_Push(&ll->return_ssas,CStr_Cpy(ssa.str));
                return ssa;
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
            return RexLang_GetConstStr(ll,a);
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
Token RexLang_GetIRVMTokenTargetSSA(RexLang* ll,Token* a){
    if(a->tt == TOKEN_STRING){
        Variable* v = Scope_FindVariable(&ll->ev.sc,a->str);
        RexLangVariable* v_sv = (RexLangVariable*)Variable_Data(v);
        CStr type = RexLang_TokenType(ll,a);
        
        if(RexLang_DrefType(type)){
            Token dst = Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(v_sv->id));
            CStr dtype = CStr_Cpy(RexLang_TokenType(ll,a));
            
            while(RexLang_DrefType(dtype)){
                const Number ssa_id = ll->ssas++;
                Token dref = Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ssa_id));
                
                RexLang_AppendTM(ll,TokenMap_Make((Token[]){
                    Token_By(TOKEN_IRVM_LOAD,IRVM_OP_LOAD),
                    Token_By(TOKEN_TYPE,RexLang_Type(ll,type)),
                    dref,
                    dst,
                    Token_Null()
                }));

                dst = RexLang_GetIRVMTokenSSA(ll,&dref);
                
                CStr swap = RexLang_GetDrefType(ll,dtype);
                CStr_Free(&dtype);
                dtype = swap;
            }

            CStr_Free(&dtype);
            return dst;
        }
    }
    return RexLang_GetIRVMTokenSSA(ll,a);
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

    Token t_dst = RexLang_GetIRVMTokenTarget(ll,dst);
    if(t_dst.tt == TOKEN_IRVM_PARAM && dst->str[0] != '.'){
        Compiler_ErrorHandler(&ll->ev,"assignment not allowed to parameters!");
        Token_Free(&t_dst);
        return Token_Null();
    }
    
    RexLang_AppendTM(ll,TokenMap_Make((Token[]){
        RexLang_GetOpIRVMToken(ll,dst,&t_dst),
        Token_By(TOKEN_TYPE,RexLang_Type(ll,type->name)),
        t_dst,
        RexLang_GetIRVMTokenTargetSSA(ll,a),
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
        }else if(a->tt==TOKEN_BOOL){
            String_AppendBoolean(&ret,a->v_b1);
        }else if(a->tt==TOKEN_FLOAT){
            String_AppendDouble(&ret,a->v_f64);
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

    const Token ssa_a = RexLang_GetIRVMTokenTargetSSA(ll,a);
    const Number ssa_id = ll->ssas++;
    
    CStr stack_name = CStr_Format("%%%d",ssa_id);
    RexLang_Variable_Build_SSA(ll,stack_name,tname,ssa_id);
    
    RexLang_AppendTM(ll,TokenMap_Make((Token[]){
        Token_By(TOKEN_IRVM_ASS,IRVM_OP_ASS),
        Token_By(TOKEN_TYPE,RexLang_Type(ll,tname)),
        Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ssa_id)),
        ssa_a,
        Token_Null()
    }));

    return Token_Move(TOKEN_STRING,stack_name);
}
Token RexLang_ExecuteAdr(RexLang* ll,Token* a){
    if(RexLang_ErrorsInArg(ll,a))   return Token_Null();

    Type* type = Scope_TypeOf(&ll->ev.sc,a);
    if(!type){
        Compiler_ErrorHandler(&ll->ev,"type of adr invalid!");
        return Token_Null();
    }

    const Number ssa_id = ll->ssas++;
    const Token t_a = RexLang_GetIRVMTokenTarget(ll,a);
    if(t_a.tt == TOKEN_IRVM_PARAM)
        Compiler_ErrorHandler(&ll->ev,"adr of parameter invalid!");
    
    CStr typename_a = RexLang_GetPtrType(ll,a);
    CStr stack_name = CStr_Format("%%%d",ssa_id);
    RexLang_Variable_Build_SSA(ll,stack_name,typename_a,ssa_id);
    CStr_Free(&typename_a);
    
    RexLang_AppendTM(ll,TokenMap_Make((Token[]){
        t_a.tt == TOKEN_IRVM_VAR ? Token_By(TOKEN_IRVM_ASS,IRVM_OP_ASS) : Token_By(TOKEN_IRVM_ADR,IRVM_OP_ADR),
        Token_By(TOKEN_TYPE,IRVM_TYPE_POINTER),
        Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ssa_id)),
        t_a,
        Token_Null()
    }));

    return Token_Move(TOKEN_STRING,stack_name);
}
Token RexLang_ExecuteDrf(RexLang* ll,Token* a){
    if(RexLang_ErrorsInArg(ll,a))   return Token_Null();

    Type* type = Scope_TypeOf(&ll->ev.sc,a);
    if(a->tt != TOKEN_STRING){
        Compiler_ErrorHandler(&ll->ev,"drf on not a variable invalid!");
        return Token_Null();
    }
    if(!type){
        Compiler_ErrorHandler(&ll->ev,"type of %s invalid!",a->str);
        return Token_Null();
    }
    if(!RexLang_PointerType(type->name)){
        Compiler_ErrorHandler(&ll->ev,"type %s of drf not a pointer!",type->name);
        return Token_Null();
    }

    const Variable* const var = Scope_FindVariable(&ll->ev.sc,a->str);
    Token t_dst = RexLang_GetIRVMTokenTarget(ll,a);
    
    const Number ssa_id = ll->ssas++;
    CStr typename_a = RexLang_GetDrefOfType(ll,var->typename);
    CStr stack_name = CStr_Format("%%%d",ssa_id);
    RexLang_Variable_Build_SSA(ll,stack_name,typename_a,ssa_id);
    CStr_Free(&typename_a);
    
    RexLang_AppendTM(ll,TokenMap_Make((Token[]){
        RexLang_GetLdIRVMToken(ll,a,&t_dst),
        Token_By(TOKEN_TYPE,RexLang_Type(ll,type->name)),
        Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ssa_id)),
        t_dst,
        Token_Null()
    }));

    Token ret = Token_Move(TOKEN_STRING,stack_name);
    if(CStr_Cmp(var->name,REXLANG_SELF)) ret.ac = TOKEN_ACCESS_FULL;
    return ret;
}
Token RexLang_Execute4(RexLang* ll,CStr inst,Token* a){
    if(RexLang_ErrorsInArg(ll,a))   return Token_Null();

    Type* type = Scope_TypeOf(&ll->ev.sc,a);
    if(!type){
        Compiler_ErrorHandler(&ll->ev,"type of tokens invalid!");
        return Token_Null();
    }

    const Token ssa_a = RexLang_GetIRVMTokenTargetSSA(ll,a);
    const Number ssa_id = ll->ssas++;

    CStr typename_a = RexLang_VariableType(ll,a);
    CStr stack_name = CStr_Format("%%%d",ssa_id);
    RexLang_Variable_Build_SSA(ll,stack_name,typename_a,ssa_id);
    CStr_Free(&typename_a);
    
    RexLang_AppendTM(ll,TokenMap_Make((Token[]){
        IRVM_GetInst(&ll->vm,inst),
        Token_By(TOKEN_TYPE,RexLang_Type(ll,type->name)),
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

    const Token ssa_a = RexLang_GetIRVMTokenTargetSSA(ll,a);
    const Token ssa_b = RexLang_GetIRVMTokenTargetSSA(ll,b);

    const Number ssa_id = ll->ssas++;
    CStr typename_a = RexLang_VariableType(ll,a);
    CStr stack_name = CStr_Format("%%%d",ssa_id);
    RexLang_Variable_Build_SSA(ll,stack_name,typename_a,ssa_id);
    CStr_Free(&typename_a);
    
    RexLang_AppendTM(ll,TokenMap_Make((Token[]){
        IRVM_GetInst(&ll->vm,inst),
        Token_By(TOKEN_TYPE,RexLang_Type(ll,type->name)),
        Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ssa_id)),
        ssa_a,
        ssa_b,
        Token_Null()
    }));

    return Token_Move(TOKEN_STRING,stack_name);
}
Token RexLang_ExecuteType5(RexLang* ll,CStr rettype,CStr inst,Token* a,Token* b){
    if(RexLang_ErrorsInArg(ll,a))   return Token_Null();
    if(RexLang_ErrorsInArg(ll,b))   return Token_Null();

    Type* type = Scope_TypeOf(&ll->ev.sc,a);
    if(!type) type = Scope_TypeOf(&ll->ev.sc,b);

    if(!type){
        Compiler_ErrorHandler(&ll->ev,"type of tokens invalid!");
        return Token_Null();
    }

    const Token ssa_a = RexLang_GetIRVMTokenTargetSSA(ll,a);
    const Token ssa_b = RexLang_GetIRVMTokenTargetSSA(ll,b);

    const Number ssa_id = ll->ssas++;
    CStr stack_name = CStr_Format("%%%d",ssa_id);
    RexLang_Variable_Build_SSA(ll,stack_name,rettype,ssa_id);
    
    RexLang_AppendTM(ll,TokenMap_Make((Token[]){
        IRVM_GetInst(&ll->vm,inst),
        Token_By(TOKEN_TYPE,RexLang_Type(ll,type->name)),
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

    const Token ssa_a = RexLang_GetIRVMTokenTargetSSA(ll,a);
    const Number ssa_id = ll->ssas++;

    CStr typename_a = RexLang_VariableType(ll,a);
    CStr stack_name = CStr_Format("%%%d",ssa_id);
    RexLang_Variable_Build_SSA(ll,stack_name,typename_a,ssa_id);
    CStr_Free(&typename_a);
    
    RexLang_AppendTM(ll,TokenMap_Make((Token[]){
        Token_By(TOKEN_IRVM_NOT,IRVM_OP_NOT),
        Token_By(TOKEN_TYPE,RexLang_Type(ll,type->name)),
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

    const Token ssa_a = RexLang_GetIRVMTokenTargetSSA(ll,a);
    const Token ssa_b = RexLang_GetIRVMTokenTargetSSA(ll,b);

    const Number ssa_id = ll->ssas++;
    CStr typename_a = RexLang_VariableType(ll,a);
    CStr stack_name = CStr_Format("%%%d",ssa_id);
    RexLang_Variable_Build_SSA(ll,stack_name,typename_a,ssa_id);
    CStr_Free(&typename_a);
    
    RexLang_AppendTM(ll,TokenMap_Make((Token[]){
        Token_By(TOKEN_IRVM_AND,IRVM_OP_AND),
        Token_By(TOKEN_TYPE,RexLang_Type(ll,type->name)),
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

    const Token ssa_a = RexLang_GetIRVMTokenTargetSSA(ll,a);
    const Token ssa_b = RexLang_GetIRVMTokenTargetSSA(ll,b);

    const Number ssa_id = ll->ssas++;
    CStr typename_a = RexLang_VariableType(ll,a);
    CStr stack_name = CStr_Format("%%%d",ssa_id);
    RexLang_Variable_Build_SSA(ll,stack_name,typename_a,ssa_id);
    CStr_Free(&typename_a);
    
    RexLang_AppendTM(ll,TokenMap_Make((Token[]){
        Token_By(TOKEN_IRVM_OR,IRVM_OP_OR),
        Token_By(TOKEN_TYPE,RexLang_Type(ll,type->name)),
        Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ssa_id)),
        ssa_a,
        ssa_b,
        Token_Null()
    }));

    return Token_Move(TOKEN_STRING,stack_name);
}

Token RexLang_StructPtrSSA(RexLang* ll,Token* a){
    if(a->tt == TOKEN_STRING){
        Variable* v = Scope_FindVariable(&ll->ev.sc,a->str);
        RexLangVariable* v_sv = (RexLangVariable*)Variable_Data(v);
        CStr type = RexLang_TokenType(ll,a);
        
        if(RexLang_DrefType(type)){
            return RexLang_GetIRVMTokenTarget(ll,a);
        }else{
            return RexLang_ExecuteAdr(ll,a);
        }
    }else{
        Compiler_ErrorHandler(&ll->ev,"argument not a variable!");
        return Token_Null();
    }
}
Token RexLang_ExecuteAcs(RexLang* ll,Token* a,CStr member){
    if(RexLang_ErrorsInArg(ll,a))   return Token_Null();

    Type* type = Scope_TypeOf(&ll->ev.sc,a);
    if(a->tt != TOKEN_STRING){
        Compiler_ErrorHandler(&ll->ev,"drf on not a variable invalid!");
        return Token_Null();
    }
    if(!type){
        Compiler_ErrorHandler(&ll->ev,"type of %s invalid!",a->str);
        return Token_Null();
    }
    if(!RexLang_StructType(ll,type->name)){
        Compiler_ErrorHandler(&ll->ev,"type %s of acs not a struct!",type->name);
        return Token_Null();
    }

    Member* mem = NULL;
    int index = 0;
    int unfold = 0;
    int offset = 0;

    for(;index<type->related.size;index++){
        mem = (Member*)Vector_Get(&type->related,index);

        if(CStr_Cmp(member,mem->name)){
            if(!mem->access && !CStr_Cmp(a->str,REXLANG_SELF) && a->ac != TOKEN_ACCESS_FULL){
                Compiler_ErrorHandler(&ll->ev,"member not accesible %s!",member);
                return Token_Null();
            }
            break;
        }
        
        offset += RexLang_Size(ll,mem->type);
        unfold += RexLang_UnfoldTypeCount(ll,mem->type);

        if(index + 1 == type->related.size){
            Compiler_ErrorHandler(&ll->ev,"member not found %s!",member);
            return Token_Null();
        }
    }

    Variable* va = Scope_FindVariable(&ll->ev.sc,a->str);
    RexLangVariable* va_sv = (RexLangVariable*)Variable_Data(va);
    
    if(va_sv->storage == REXLANG_STORAGE_RETURN || va_sv->storage == REXLANG_STORAGE_PARAM){
        const Number ssa_id = ll->ssas++;

        CStr typename_a = CStr_Cpy(mem->type);
        CStr stack_name = CStr_Format("%%%d",ssa_id);
        RexLang_Variable_Build_Param(ll,stack_name,typename_a,va_sv->id + unfold);
        CStr_Free(&typename_a);
        
        return Token_Move(TOKEN_STRING,stack_name);
    }else{
        Token ssa_a = RexLang_GetIRVMTokenTarget(ll,a);
        const Number ssa_id = ll->ssas++;

        CStr typename_a = CStr_Concat(mem->type,"&");
        CStr stack_name = CStr_Format("%%%d",ssa_id);
        RexLang_Variable_Build_SSA(ll,stack_name,typename_a,ssa_id);
        CStr_Free(&typename_a);
        
        RexLang_AppendTM(ll,TokenMap_Make((Token[]){
            Token_By(TOKEN_IRVM_ADD,IRVM_OP_ADD),
            Token_By(TOKEN_TYPE,IRVM_TYPE_POINTER),
            Token_Move(TOKEN_IRVM_SSA,(CStr)Number_Get(ssa_id)),
            ssa_a,
            Token_New_I64(TOKEN_NUMBER,offset),
            Token_Null()
        }));

        return Token_Move(TOKEN_STRING,stack_name);
    }
}

#endif //!REXLANGASM_H