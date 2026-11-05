#ifndef REXLANGDEFINES_H
#define REXLANGDEFINES_H

#include "/home/codeleaded/System/Static/Library/IRVM_Defines.h"
#include "/home/codeleaded/System/Static/Library/AlxParser.h"

#define TOKEN_REXLANG_LINECOMMENT        (TOKEN_IRVM_END+0)
#define TOKEN_REXLANG_BLOCKCOMMENTSTART  (TOKEN_IRVM_END+1)
#define TOKEN_REXLANG_BLOCKCOMMENTEND    (TOKEN_IRVM_END+2)
#define TOKEN_REXLANG_RETURN             (TOKEN_IRVM_END+3)
#define TOKEN_REXLANG_CONTINUE           (TOKEN_IRVM_END+4)
#define TOKEN_REXLANG_BREAK              (TOKEN_IRVM_END+5)
#define TOKEN_REXLANG_END                (TOKEN_IRVM_END+6)
#define TOKEN_REXLANG_IF                 (TOKEN_IRVM_END+7)
#define TOKEN_REXLANG_ELIF               (TOKEN_IRVM_END+8)
#define TOKEN_REXLANG_ELSE               (TOKEN_IRVM_END+9)
#define TOKEN_REXLANG_WHILE              (TOKEN_IRVM_END+10)
#define TOKEN_REXLANG_FOR                (TOKEN_IRVM_END+11)
#define TOKEN_REXLANG_IMPORT             (TOKEN_IRVM_END+12)
#define TOKEN_REXLANG_CONST              (TOKEN_IRVM_END+13)
#define TOKEN_REXLANG_STRUCT             (TOKEN_IRVM_END+14)
#define TOKEN_REXLANG_IMPL               (TOKEN_IRVM_END+15)
#define TOKEN_REXLANG_NAMESPACE          (TOKEN_IRVM_END+16)
#define TOKEN_REXLANG_BOOLEAN            (TOKEN_IRVM_END+17)
#define TOKEN_REXLANG_ASS                (TOKEN_IRVM_END+18)
#define TOKEN_REXLANG_ADD                (TOKEN_IRVM_END+19)
#define TOKEN_REXLANG_SUB                (TOKEN_IRVM_END+20)
#define TOKEN_REXLANG_MUL                (TOKEN_IRVM_END+21)
#define TOKEN_REXLANG_DIV                (TOKEN_IRVM_END+22)
#define TOKEN_REXLANG_NEG                (TOKEN_IRVM_END+23)
#define TOKEN_REXLANG_DRF                (TOKEN_IRVM_END+24)
#define TOKEN_REXLANG_ADR                (TOKEN_IRVM_END+25)
#define TOKEN_REXLANG_AND                (TOKEN_IRVM_END+26)
#define TOKEN_REXLANG_OR                 (TOKEN_IRVM_END+27)
#define TOKEN_REXLANG_XOR                (TOKEN_IRVM_END+28)
#define TOKEN_REXLANG_NOT                (TOKEN_IRVM_END+29)
#define TOKEN_REXLANG_LND                (TOKEN_IRVM_END+30)
#define TOKEN_REXLANG_LOR                (TOKEN_IRVM_END+31)
#define TOKEN_REXLANG_LOT                (TOKEN_IRVM_END+32)
#define TOKEN_REXLANG_NEQ                (TOKEN_IRVM_END+33)
#define TOKEN_REXLANG_EQU                (TOKEN_IRVM_END+34)
#define TOKEN_REXLANG_LES                (TOKEN_IRVM_END+35)
#define TOKEN_REXLANG_GRT                (TOKEN_IRVM_END+36)
#define TOKEN_REXLANG_LEQ                (TOKEN_IRVM_END+37)
#define TOKEN_REXLANG_GRQ                (TOKEN_IRVM_END+38)
#define TOKEN_REXLANG_ACS                (TOKEN_IRVM_END+39)
#define TOKEN_REXLANG_ARW                (TOKEN_IRVM_END+40)
#define TOKEN_REXLANG_DDOT               (TOKEN_IRVM_END+41)
#define TOKEN_REXLANG_DDDOT              (TOKEN_IRVM_END+42)
#define TOKEN_REXLANG_SUBS               (TOKEN_IRVM_END+43)
#define TOKEN_REXLANG_SUBSR              (TOKEN_IRVM_END+44)
#define TOKEN_REXLANG_ASD                (TOKEN_IRVM_END+45)
#define TOKEN_REXLANG_ASU                (TOKEN_IRVM_END+46)
#define TOKEN_REXLANG_ASM                (TOKEN_IRVM_END+47)
#define TOKEN_REXLANG_ASV                (TOKEN_IRVM_END+48)
#define TOKEN_REXLANG_MOD                (TOKEN_IRVM_END+49)
#define TOKEN_REXLANG_INC                (TOKEN_IRVM_END+50)
#define TOKEN_REXLANG_DEC                (TOKEN_IRVM_END+51)
#define TOKEN_REXLANG_CAL                (TOKEN_IRVM_END+52)
#define TOKEN_REXLANG_ASMBY              (TOKEN_IRVM_END+53)
#define TOKEN_REXLANG_REG                (TOKEN_IRVM_END+54)
#define TOKEN_REXLANG_SIZE               (TOKEN_IRVM_END+55)
#define TOKEN_REXLANG_PUB                (TOKEN_IRVM_END+56)
#define TOKEN_REXLANG_GLOBAL             (TOKEN_IRVM_END+57)
#define TOKEN_REXLANG_EXTERN             (TOKEN_IRVM_END+58)
#define TOKEN_REXLANG_DEFINE             (TOKEN_IRVM_END+59)
#define TOKEN_REXLANG_SHL                (TOKEN_IRVM_END+60)
#define TOKEN_REXLANG_SHR                (TOKEN_IRVM_END+61)
#define TOKEN_REXLANG_NULL               (TOKEN_IRVM_END+62)

#define REXLANG_TYPE                       "rex"
#define REXLANG_INDENTATION                "    "
#define REXLANG_INDENTATION_NONE           0
#define REXLANG_INDENTATION_FUNCTION       1

#define REXLANG_TYPE_NONE                  "void"
#define REXLANG_SELF                       "self"
#define REXLANG_BOOL                       ".BOOL"
#define REXLANG_IF                         "IF"
#define REXLANG_ELIF                       "ELIF"
#define REXLANG_ELSE                       "ELSE"
#define REXLANG_WHILE                      "WHILE"
#define REXLANG_FOR                        "FOR"
#define REXLANG_LOG                        "LOG"
#define REXLANG_LOG_TRUE                   "TRUE"
#define REXLANG_LOG_END                    "END"

#define LOGIC_TYPE                          "i8"
#define BOOL_TYPE                           "bool"
#define I8_TYPE                             "i8"
#define I16_TYPE                            "i16"
#define I32_TYPE                            "i32"
#define I64_TYPE                            "i64"
#define U8_TYPE                             "u8"
#define U16_TYPE                            "u16"
#define U32_TYPE                            "u32"
#define U64_TYPE                            "u64"
#define F32_TYPE                            "f32"
#define F64_TYPE                            "f64"
#define VOID_TYPE                           "void"
#define NULL_TYPE                           VOID_TYPE"*"
#define STR_TYPE                            "i8*"
#define CHAR_TYPE                           I32_TYPE
#define POINTER_TYPE                        "?*"
#define DREF_TYPE                           "!&"
#define STRUCT_TYPE                         ".struct"

#define BOOL_SIZE                           1
#define I8_SIZE                             1
#define I16_SIZE                            2
#define I32_SIZE                            4
#define I64_SIZE                            8
#define U8_SIZE                             1
#define U16_SIZE                            2
#define U32_SIZE                            4
#define U64_SIZE                            8
#define F32_SIZE                            4
#define F64_SIZE                            8
#define VOID_SIZE                           0
#define STR_SIZE                            8
#define CHAR_SIZE                           4
#define POINTER_SIZE                        8
#define DREF_SIZE                           8


typedef enum RexLang_Storage{
    REXLANG_STORAGE_NONE = 0,
    REXLANG_STORAGE_AUTO = 1,
    REXLANG_STORAGE_STACK = 2,
    REXLANG_STORAGE_GLOBAL = 3,
    REXLANG_STORAGE_STACK_STRUCT = 4,
    REXLANG_STORAGE_GLOBAL_STRUCT = 5,
    REXLANG_STORAGE_STACK_BLOCK = 6,
    REXLANG_STORAGE_GLOBAL_BLOCK = 7,
    REXLANG_STORAGE_STACK_STRUCT_BLOCK = 8,
    REXLANG_STORAGE_GLOBAL_STRUCT_BLOCK = 9,
    REXLANG_STORAGE_SSA = 10,
    REXLANG_STORAGE_RETURN = 11,
    REXLANG_STORAGE_PARAM = 12
} RexLang_Storage;

typedef enum RexLang_Destroy{
    REXLANG_DESTROY_NONE = 0,
    REXLANG_DESTROY_DESTRUCT = 1,
    REXLANG_DESTROY_FREE = 2,
    REXLANG_DESTROY_FULL = 3
} RexLang_Destroy;

typedef struct RexLangVariable{
    void* parent;
    RexLang_Storage storage;
    RexLang_Destroy destroy;
    union {
        CStr global;
        uint64_t id;
        uint64_t size;
    };
} RexLangVariable;

RexLangVariable RexLangVariable_New(void* parent,RexLang_Storage storage,RexLang_Destroy destroy){
    RexLangVariable sv;
    sv.parent = parent;
    sv.storage = storage;
    sv.destroy = destroy;
    sv.global = NULL;
    return sv;
}
TT_Type RexLangVariable_Type(RexLangVariable* sv){
    switch (sv->storage){
        case REXLANG_STORAGE_STACK:                 return TOKEN_IRVM_SSA;
        case REXLANG_STORAGE_GLOBAL:                return TOKEN_IRVM_SSA;
        case REXLANG_STORAGE_STACK_STRUCT:          return TOKEN_IRVM_SSA;
        case REXLANG_STORAGE_GLOBAL_STRUCT:         return TOKEN_IRVM_SSA;
        case REXLANG_STORAGE_STACK_BLOCK:           return TOKEN_IRVM_SSA;
        case REXLANG_STORAGE_GLOBAL_BLOCK:          return TOKEN_IRVM_SSA;
        case REXLANG_STORAGE_STACK_STRUCT_BLOCK:    return TOKEN_IRVM_SSA;
        case REXLANG_STORAGE_GLOBAL_STRUCT_BLOCK:   return TOKEN_IRVM_SSA;
        case REXLANG_STORAGE_SSA:                   return TOKEN_IRVM_SSA;
        case REXLANG_STORAGE_RETURN:                return TOKEN_IRVM_PARAM;
        case REXLANG_STORAGE_PARAM:                 return TOKEN_IRVM_PARAM;
    }
    return TOKEN_NONE;
}
void RexLangVariable_Free(RexLangVariable* sv){
    if(
        sv->storage == REXLANG_STORAGE_GLOBAL ||
        sv->storage == REXLANG_STORAGE_GLOBAL_BLOCK ||
        sv->storage == REXLANG_STORAGE_GLOBAL_STRUCT ||
        sv->storage == REXLANG_STORAGE_GLOBAL_STRUCT_BLOCK
    ){
        if(sv->destroy == REXLANG_DESTROY_FREE || sv->destroy == REXLANG_DESTROY_FULL){
            CStr_Free(&sv->global);
        }
    }
}

#endif //!REXLANGDEFINES_H