#ifndef RexLangASMF
#define RexLangASMF

#include "RexLangASM.h"


Double RexLang_Function_FAdd(Double a,Double b){
    return a + b;
}
Double RexLang_Function_FSub(Double a,Double b){
    return a - b;
}
Double RexLang_Function_FMul(Double a,Double b){
    return a * b;
}
Double RexLang_Function_FDiv(Double a,Double b){
    return a / b;
}
Double RexLang_Function_FNeg(Double a){
    return -a;
}
Double RexLang_Function_FInc(Double a){
    return ++a;
}
Double RexLang_Function_FDec(Double a){
    return --a;
}
Boolean RexLang_Function_FEqu(Double a,Double b){
    return a == b;
}
Boolean RexLang_Function_FNeq(Double a,Double b){
    return a != b;
}
Boolean RexLang_Function_FLes(Double a,Double b){
    return a < b;
}
Boolean RexLang_Function_FGrt(Double a,Double b){
    return a > b;
}
Boolean RexLang_Function_FLeq(Double a,Double b){
    return a <= b;
}
Boolean RexLang_Function_FGrq(Double a,Double b){
    return a >= b;
}

void RexLang_AtFPU(RexLang* ll,Token* a){
    if(a->tt==TOKEN_STRING){
        Variable* v = Scope_FindVariable(&ll->ev.sc,a->str);
        if(v){
            if(RexLang_TypeFloat(v->typename)){
                if(RexLang_DrefType(ll,v->typename)){
                    RexLang_DrefIntoReg(ll,a,RexLang_REG_D_64);
                    
                    CStr typename = RexLang_TypeOfDref(ll,v->typename);
                    CStr typeselector = RexLang_TypeSelector_T(ll,typename);
                    RexLang_Indentation_Appendf(ll,&ll->text,"fld %s[%s]",typeselector,RexLang_REG_D_64);
                    CStr_Free(&typename);
                    CStr_Free(&typeselector);
                }else{
                    CStr location = RexLang_Location(ll,a->str);
                    RexLang_Indentation_Appendf(ll,&ll->text,"fld %s",location);
                    CStr_Free(&location);
                }
            }else if(RexLang_TypeInt(v->typename)){
                if(RexLang_DrefType(ll,v->typename)){
                    RexLang_DrefIntoReg(ll,a,RexLang_REG_D_64);
                    
                    CStr typename = RexLang_TypeOfDref(ll,v->typename);
                    CStr typeselector = RexLang_TypeSelector_T(ll,typename);
                    RexLang_Indentation_Appendf(ll,&ll->text,"fild %s[%s]",typeselector,RexLang_REG_D_64);
                    CStr_Free(&typename);
                    CStr_Free(&typeselector);
                }else{
                    CStr location = RexLang_Location(ll,a->str);
                    RexLang_Indentation_Appendf(ll,&ll->text,"fild %s",location);
                    CStr_Free(&location);
                }
            }else{
                Compiler_ErrorHandler(&ll->ev,"AtFPU -> Error: %s is not a int of float!",a->str);
            }
        }else{
            Compiler_ErrorHandler(&ll->ev,"AtFPU -> Error: %s is not a var!",a->str);
        }
    }else if(a->tt==TOKEN_FLOAT){
        CStr fstr = RexLang_GetFloat(ll,a->v_f64);
        RexLang_Indentation_Appendf(ll,&ll->text,"mov %s,%s",RexLang_REG_A_64,fstr);
        RexLang_Indentation_Appendf(ll,&ll->text,"push %s",RexLang_REG_A_64);
        RexLang_Indentation_Appendf(ll,&ll->text,"fld %s[%s]",RexLang_DREF_64,RexLang_REG_SP_64);
        RexLang_Indentation_Appendf(ll,&ll->text,"add %s,8",RexLang_REG_SP_64);
        CStr_Free(&fstr);
    }else if(a->tt==TOKEN_NUMBER){
        RexLang_Indentation_Appendf(ll,&ll->text,"mov %s,%d",RexLang_REG_A_64,a->v_i64);
        RexLang_Indentation_Appendf(ll,&ll->text,"push %s",RexLang_REG_A_64);
        RexLang_Indentation_Appendf(ll,&ll->text,"fild %s[%s]",RexLang_DREF_64,RexLang_REG_SP_64);
        RexLang_Indentation_Appendf(ll,&ll->text,"add %s,8",RexLang_REG_SP_64);
    }else{
        Compiler_ErrorHandler(&ll->ev,"AtFPU -> Error: %s is not a var or const!",a->str);
    }
}
void RexLang_DoFPU(RexLang* ll,Token* a,CStr inst){
    CStr rinst = RexLang_FInstOf(ll,a,inst);
    
    if(a->tt==TOKEN_STRING){
        Variable* v = Scope_FindVariable(&ll->ev.sc,a->str);
        if(v){
            if(RexLang_TypeFloat(v->typename)){
                if(RexLang_DrefType(ll,v->typename)){
                    RexLang_DrefIntoReg(ll,a,RexLang_REG_D_64);
                    
                    CStr typename = RexLang_TypeOfDref(ll,v->typename);
                    CStr typeselector = RexLang_TypeSelector_T(ll,typename);
                    RexLang_Indentation_Appendf(ll,&ll->text,"%s %s[%s]",rinst,typeselector,RexLang_REG_D_64);
                    CStr_Free(&typename);
                    CStr_Free(&typeselector);
                }else{
                    CStr location = RexLang_Location(ll,a->str);
                    RexLang_Indentation_Appendf(ll,&ll->text,"%s %s",rinst,location);
                    CStr_Free(&location);
                }
            }else if(RexLang_TypeInt(v->typename)){
                if(RexLang_DrefType(ll,v->typename)){
                    RexLang_DrefIntoReg(ll,a,RexLang_REG_D_64);
                    
                    CStr typename = RexLang_TypeOfDref(ll,v->typename);
                    CStr typeselector = RexLang_TypeSelector_T(ll,typename);
                    RexLang_Indentation_Appendf(ll,&ll->text,"%s %s[%s]",rinst,typeselector,RexLang_REG_D_64);
                    CStr_Free(&typename);
                    CStr_Free(&typeselector);
                }else{
                    CStr location = RexLang_Location(ll,a->str);
                    RexLang_Indentation_Appendf(ll,&ll->text,"%s %s",rinst,location);
                    CStr_Free(&location);
                }
            }else{
                Compiler_ErrorHandler(&ll->ev,"DoFPU -> Error: %s is not a int of float!",a->str);
            }
        }else{
            Compiler_ErrorHandler(&ll->ev,"DoFPU -> Error: %s is not a var!",a->str);
        }
    }else if(a->tt==TOKEN_FLOAT){
        CStr fstr = RexLang_GetFloat(ll,a->v_f64);
        RexLang_Indentation_Appendf(ll,&ll->text,"sub %s,8",RexLang_REG_SP_64);
        RexLang_Indentation_Appendf(ll,&ll->text,"mov %s[%s],%s",RexLang_DREF_64,RexLang_REG_SP_64,fstr);
        RexLang_Indentation_Appendf(ll,&ll->text,"%s %s[%s]",rinst,RexLang_DREF_64,RexLang_REG_SP_64);
        RexLang_Indentation_Appendf(ll,&ll->text,"add %s,8",RexLang_REG_SP_64);
        CStr_Free(&fstr);
    }else if(a->tt==TOKEN_NUMBER){
        RexLang_Indentation_Appendf(ll,&ll->text,"sub %s,8",RexLang_REG_SP_64);
        RexLang_Indentation_Appendf(ll,&ll->text,"mov %s[%s],%d",RexLang_DREF_64,RexLang_REG_SP_64,a->v_i64);
        RexLang_Indentation_Appendf(ll,&ll->text,"%s %s[%s]",rinst,RexLang_DREF_64,RexLang_REG_SP_64);
        RexLang_Indentation_Appendf(ll,&ll->text,"add %s,8",RexLang_REG_SP_64);
    }else{
        Compiler_ErrorHandler(&ll->ev,"DoFPU -> Error: %s is not a var or const!",a->str);
    }
    
    CStr_Free(&rinst);
}
void RexLang_DoFPUInst(RexLang* ll,CStr inst){
    RexLang_Indentation_Appendf(ll,&ll->text,"%s",inst);
}
void RexLang_FromFPU(RexLang* ll,Token* a){
    if(a->tt==TOKEN_STRING){
        Variable* v = Scope_FindVariable(&ll->ev.sc,a->str);
        if(v){
            if(RexLang_TypeFloat(v->typename)){
                if(RexLang_DrefType(ll,v->typename)){
                    RexLang_DrefIntoReg(ll,a,RexLang_REG_D_64);
                    
                    CStr typename = RexLang_TypeOfDref(ll,v->typename);
                    CStr typeselector = RexLang_TypeSelector_T(ll,typename);
                    RexLang_Indentation_Appendf(ll,&ll->text,"fstp %s[%s]",typeselector,RexLang_REG_D_64);
                    CStr_Free(&typename);
                    CStr_Free(&typeselector);
                }else{
                    CStr location = RexLang_Location(ll,a->str);
                    RexLang_Indentation_Appendf(ll,&ll->text,"fstp %s",location);
                    CStr_Free(&location);
                }
            }else if(RexLang_TypeInt(v->typename)){
                if(RexLang_DrefType(ll,v->typename)){
                    RexLang_DrefIntoReg(ll,a,RexLang_REG_D_64);
                    
                    CStr typename = RexLang_TypeOfDref(ll,v->typename);
                    CStr typeselector = RexLang_TypeSelector_T(ll,typename);
                    RexLang_Indentation_Appendf(ll,&ll->text,"fistp %s[%s]",typeselector,RexLang_REG_D_64);
                    CStr_Free(&typename);
                    CStr_Free(&typeselector);
                }else{
                    CStr location = RexLang_Location(ll,a->str);
                    RexLang_Indentation_Appendf(ll,&ll->text,"fistp %s",location);
                    CStr_Free(&location);
                }
            }else{
                Compiler_ErrorHandler(&ll->ev,"FromFPU -> Error: %s is not a int of float!",a->str);
            }
        }else{
            Compiler_ErrorHandler(&ll->ev,"FromFPU -> Error: var %s doesn't exist!",a->str);
        }
    }else{
        Compiler_ErrorHandler(&ll->ev,"FromFPU -> Error: %s is not a var!",a->str);
    }
}
void RexLang_CmpFPU(RexLang* ll,Token* a){
    RexLang_AtFPU(ll,a);
    RexLang_Indentation_Appendf(ll,&ll->text,"fcomp");
    RexLang_Indentation_Appendf(ll,&ll->text,"fstsw ax");
    RexLang_Indentation_Appendf(ll,&ll->text,"sahf");
    RexLang_Indentation_Appendf(ll,&ll->text,"fstp");
}

Token RexLang_ExecuteFAss(RexLang* ll,Token* a,Token* b,Token* op,CStr instname,CStr instnameupper){
    //Compiler_InfoHandler(&ll->ev,"%s: %s %s %s",instnameupper,a->str,op->str,b->str);
    
    if(RexLang_ErrorsInArg(ll,a)) return Token_Null();
    if(RexLang_ErrorsInArg(ll,b)) return Token_Null();

    RexLang_AtFPU(ll,a);
    RexLang_DoFPU(ll,b,instname);
    RexLang_FromFPU(ll,a);
    return Token_Cpy(b);
}
Token RexLang_ExecuteFAssSingle(RexLang* ll,Token* a,Token* op,CStr instname,CStr instnameupper){
    //Compiler_InfoHandler(&ll->ev,"%s: %s %s %s",instnameupper,a->str,op->str,b->str);
    
    if(RexLang_ErrorsInArg(ll,a)) return Token_Null();

    RexLang_AtRegSingle(ll,a,instname);
    return Token_Cpy(a);
}

Token RexLang_ExecuteF(RexLang* ll,Token* a,Token* b,Token* op,CStr instname,CStr instnameupper,Double (*inst)(Double,Double)){
    //Compiler_InfoHandler(&ll->ev,"%s: %s %s %s",instnameupper,a->str,op->str,b->str);
    
    if(RexLang_ErrorsInArg(ll,a)) return Token_Null();
    if(RexLang_ErrorsInArg(ll,b)) return Token_Null();

    if(a->tt==TOKEN_FLOAT && b->tt==TOKEN_FLOAT){
        return Token_New_F64(TOKEN_FLOAT,inst(a->v_f64,b->v_f64));
    }else{
        CStr typename_a = RexLang_VariableType(ll,a);
        
        CStr stack_name = RexLang_Variablename_Next(ll,".STACK",6);
        RexLang_Variable_Build_Decl(ll,stack_name,typename_a);
        CStr_Free(&typename_a);
        Token stack_t = Token_Move(TOKEN_STRING,stack_name);

        RexLang_AtFPU(ll,a);
        RexLang_DoFPU(ll,b,instname);
        RexLang_FromFPU(ll,&stack_t);
        return stack_t;
    }
}
Token RexLang_ExecuteFSingle(RexLang* ll,Token* a,Token* op,CStr instname,CStr instnameupper,Double (*inst)(Double)){
    //Compiler_InfoHandler(&ll->ev,"%s: %s%s",instnameupper,op->str,a->str);
    
    if(RexLang_ErrorsInArg(ll,a)) return Token_Null();

    if(a->tt==TOKEN_FLOAT){
        return Token_New_F64(TOKEN_FLOAT,inst(a->v_f64));
    }else{
        CStr typename_a = RexLang_VariableType(ll,a);
        //int realsize_a = RexLang_TypeRealSize(ll,a);
        
        CStr stack_name = RexLang_Variablename_Next(ll,".STACK",6);
        RexLang_Variable_Build_Decl(ll,stack_name,typename_a);
        CStr_Free(&typename_a);
        Token stack_t = Token_Move(TOKEN_STRING,stack_name);

        RexLang_AtFPU(ll,a);
        RexLang_DoFPUInst(ll,instname);
        RexLang_FromFPU(ll,&stack_t);
        return stack_t;
    }
}
Token RexLang_ExecuteFCmp(RexLang* ll,Token* a,Token* b,Token* op,CStr instname,CStr instnameupper,Boolean (*inst)(Double,Double)){
    //Compiler_InfoHandler(&ll->ev,"%s: %s %s %s",instnameupper,a->str,op->str,b->str);
    
    if(RexLang_ErrorsInArg(ll,a)) return Token_Null();
    if(RexLang_ErrorsInArg(ll,b)) return Token_Null();

    if(a->tt==TOKEN_FLOAT && b->tt==TOKEN_NUMBER){
        return Token_New_B1(TOKEN_REXLANG_BOOLEAN,inst(a->v_f64,b->v_f64));
    }else if(a->tt==TOKEN_FLOAT && b->tt==TOKEN_FLOAT){
        return Token_New_B1(TOKEN_REXLANG_BOOLEAN,inst(a->v_f64,b->v_f64));
    }else{
        //int realsize_a = RexLang_TypeRealSize(ll,a);
        //int realsize_b = RexLang_TypeRealSize(ll,b);
        
        CStr stack_name = RexLang_Variablename_Next(ll,".STACK",6);
        RexLang_Variable_Build_Decl(ll,stack_name,BOOL_TYPE);
        Token stack_t = Token_Move(TOKEN_STRING,stack_name);
        
        RexLang_AtFPU(ll,a);
        RexLang_CmpFPU(ll,b);
        RexLang_CmpAtSet(ll,&stack_t,instname);
        return stack_t;
    }
}
Token RexLang_ExecuteFJmp(RexLang* ll,Token* a,Token* b,Token* op,CStr instname,CStr instnameupper,Boolean (*inst)(Double,Double)){
    //Compiler_InfoHandler(&ll->ev,"%s: %s %s %s",instnameupper,a->str,op->str,b->str);
    
    if(RexLang_ErrorsInArg(ll,a)) return Token_Null();
    if(RexLang_ErrorsInArg(ll,b)) return Token_Null();

    if(a->tt==TOKEN_FLOAT && b->tt==TOKEN_NUMBER){
        return Token_New_B1(TOKEN_REXLANG_BOOLEAN,inst(a->v_f64,b->v_f64));
    }else if(a->tt==TOKEN_FLOAT && b->tt==TOKEN_FLOAT){
        return Token_New_B1(TOKEN_REXLANG_BOOLEAN,inst(a->v_f64,b->v_f64));
    }else{
        //int realsize_a = RexLang_TypeRealSize(ll,a);
        //int realsize_b = RexLang_TypeRealSize(ll,b);
        
        CStr stack_name = RexLang_Variablename_Next(ll,".STACK",6);
        RexLang_Variable_Build_Decl(ll,stack_name,BOOL_TYPE);
        Token stack_t = Token_Move(TOKEN_STRING,stack_name);
        
        RexLang_AtFPU(ll,a);
        RexLang_CmpFPU(ll,b);
        
        CStr label_true = RexLang_LogicCmp(ll,RexLang_LOG_TRUE);
        CStr label_end = RexLang_LogicCmp(ll,RexLang_LOG_END);

        RexLang_Indentation_Appendf(ll,&ll->text,"%s %s",instname,label_true);
        RexLang_IntoSet(ll,&stack_t,"0");
        RexLang_Indentation_Appendf(ll,&ll->text,"jmp %s",label_end);
        RexLang_Indentation_Appendf(ll,&ll->text,"%s:",label_true);
        RexLang_IntoSet(ll,&stack_t,"1");
        RexLang_Indentation_Appendf(ll,&ll->text,"%s:",label_end);

        CStr_Free(&label_true);
        CStr_Free(&label_end);
        return stack_t;
    }
}

Token Int_Float_Handler_Cast(RexLang* ll,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);

    if(a->tt==TOKEN_NUMBER){
        return Token_New_B1(TOKEN_FLOAT,a->v_i64);
    }else if(a->tt==TOKEN_STRING){
        //Variable* v = Scope_FindVariable(&ll->ev.sc,a->str);
        CStr stack_name = RexLang_Variablename_Next(ll,".STACK",6);
        
        RexLang_Variable_Build_Decl(ll,stack_name,F64_TYPE);
        
        Token stack_t = Token_Move(TOKEN_STRING,stack_name);
        RexLang_AtFPU(ll,a);
        RexLang_FromFPU(ll,&stack_t);
        return stack_t;
    }else{
        Environment_ErrorHandler(&ll->ev,"Cast(i64 -> f64): Error -> %s is from no possible type!",a->str);
        return Token_Null();
    }
}

#endif //!RexLangASMF