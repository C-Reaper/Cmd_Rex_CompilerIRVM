#include "../src/RexLang.h"
//#include "/home/codeleaded/System/Static/Library/IRVM_Exe.h"
#include "/home/codeleaded/System/Static/Library/IRVM_Comp.h"
#include <stdio.h>
#include <string.h>

#define PATH_SRC        "./code/Main.rex"
#define PATH_IRVM       "./asm/Main.iralx"
#define PATH_OPT_IRVM   "./asm/Opt.iralx"
#define PATH_ASM        "./asm/Output.asm"

int main(int argc,char** argv){
    char* target = argc > 1 ? argv[1] : "linux";
    char* libpath;

    if(argc > 2){
        fprintf(stderr,"Usage: %s [linux|windows]\n",argv[0]);
        return 2;
    }
    if(strcmp(target,"linux") == 0){
        libpath = "./lib/x64/Linux";
    }else if(strcmp(target,"windows") == 0){
        libpath = "./lib/x64/Win";
    }else{
        fprintf(stderr,"Unknown target '%s'. Use 'linux' or 'windows'.\n",target);
        return 2;
    }

    RexLang ll = RexLang_New(libpath,"./lib/x64/generic",PATH_SRC,PATH_IRVM);
    RexLang_Build(&ll);
    RexLang_SaveIRVM(&ll);
    int status = ll.ev.error ? 1 : 0;
    
    if(!ll.ev.error){
        IRVM vm = ll.vm;
        ll.vm = IRVM_Null();

        //IRVM vm = IRVM_New();
        //IRVM_AddScript(&vm,PATH_IRVM);
        
        IRVM_Build_Split(&vm);
        IRVM_Build(&vm);
        IRVM_Optimiser_Exe(&vm,0U,(
            IRVM_OPTIMISATION_AS    |
            IRVM_OPTIMISATION_SR    |
            IRVM_OPTIMISATION_CF    |
            IRVM_OPTIMISATION_CP    |
            IRVM_OPTIMISATION_CPP   |
            IRVM_OPTIMISATION_DCE   |
            IRVM_OPTIMISATION_CSE   |
            IRVM_OPTIMISATION_BBS   |
            IRVM_OPTIMISATION_BS    |
            IRVM_OPTIMISATION_UCE   |
            IRVM_OPTIMISATION_COR   |
            IRVM_OPTIMISATION_INC   |
            IRVM_OPTIMISATION_M2R   |
            IRVM_OPTIMISATION_PR    |
            0
        ));
        //IRVM_Print(&vm);
        IRVM_Save(&vm,PATH_OPT_IRVM);
        //IRVM_Free(&vm);
        
        IRVM_Comp exe = IRVM_Comp_New(vm,PATH_ASM);
        IRVM_Comp_Build(&exe);
        if(!exe.state){
            IRVM_Comp_CreateOutput(&exe);
        }
        if(exe.state){
            status = 1;
        }
        IRVM_Comp_Free(&exe);
    }

    RexLang_Free(&ll);
    return status;
}
