#include "../src/RexLang.h"
#include "/home/codeleaded/System/Static/Library/IRVM_Exe.h"

#define PATH_SRC "./code/Main.rex"
#define PATH_ASM "./asm/Main.iralx"

int main(int argc,char** argv){
    //if(argc<=1){
    //    printf("Missing Src File ."RexLang_TYPE"!\n");
    //    return 1;
    //}
    //if(argc<=2){
    //    printf("Missing Output File .asm/s!\n");
    //    return 1;
    //}

    RexLang ll = RexLang_New("./bin",PATH_SRC,PATH_ASM,64);
    RexLang_Build(&ll);

    /*
    IRVM_Print(&ll.vm);
    IRVM_Build(&ll.vm);
    IRVM_Optimiser_Exe(&ll.vm,0U);
    IRVM_Print(&ll.vm);
    
    IRVM_Exe exe = IRVM_Exe_New(ll.vm);
    IRVM_Exe_Variable a = IRVM_Exe_Variable_New("a",IRVM_TYPE_I16,(Number[]){ 0UL }, IRVM_Impl_Size(IRVM_TYPE_I16),0,0);
    IRVM_Exe_Variable b = IRVM_Exe_Variable_New("b",IRVM_TYPE_I16,(Number[]){ 10UL },IRVM_Impl_Size(IRVM_TYPE_I16),0,0);
    IRVM_Exe_Variable c = IRVM_Exe_Variable_New("c",IRVM_TYPE_I16,(Number[]){ 20UL },IRVM_Impl_Size(IRVM_TYPE_I16),0,0);
    
    IRVM_Exe_CallFunction(&exe,"main",(IRVM_Exe_Variable[]){
        a,
        b,
        c,
        IRVM_Exe_Variable_Null()
    });
    
    IRVM_Exe_Variable_Print(&a);
    IRVM_Exe_Variable_Print(&b);
    IRVM_Exe_Variable_Print(&c);

    IRVM_Exe_Variable_Free(&c);
    IRVM_Exe_Variable_Free(&b);
    IRVM_Exe_Variable_Free(&a);

    IRVM_Exe_Free(&exe);
    ll.vm = IRVM_New();
    */

    RexLang_Free(&ll);
    return 0;
}
