#include "../src/RexLang.h"
//#include "/home/codeleaded/System/Static/Library/IRVM_Exe.h"
#include "/home/codeleaded/System/Static/Library/IRVM_Comp.h"

#define PATH_SRC    "./code/Main.rex"
#define PATH_IRVM   "./asm/Main.iralx"
#define PATH_ASM    "./asm/Output.asm"

int main(int argc,char** argv){
    //if(argc<=1){
    //    printf("Missing Src File ."RexLang_TYPE"!\n");
    //    return 1;
    //}
    //if(argc<=2){
    //    printf("Missing Output File .asm/s!\n");
    //    return 1;
    //}

    // "./lib/x64/Linux" | "./lib/x64/Win"
    RexLang ll = RexLang_New("./lib/x64/Win","./lib/x64/generic",PATH_SRC,PATH_IRVM);
    RexLang_Build(&ll);
    
    if(!ll.ev.error){
        IRVM vm = IRVM_New();
        IRVM_AddScript(&vm,PATH_IRVM);
        IRVM_Build(&vm);
        //IRVM_Optimiser_Exe(&vm,0U,IRVM_OPTIMISATION_AS);
        
        IRVM_Comp exe = IRVM_Comp_New(vm,PATH_ASM);
        IRVM_Comp_Build(&exe);
        IRVM_Comp_CreateOutput(&exe);
        IRVM_Comp_Free(&exe);
    }

    RexLang_Free(&ll);
    return 0;
}
