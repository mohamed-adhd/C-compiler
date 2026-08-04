//
// Created by bro on 8/4/26.
//

#ifndef C_COMPILER_MASM_H
#define C_COMPILER_MASM_H
typedef enum {
    OPERAND_NONE,OPERAND_REGISTER,OPERAND_IMMEDIATE,OPERAND_MEMORY,OPERAND_LABEL
} operand_type;

typedef enum {
    REG_RAX, REG_RCX, REG_RDX, REG_RBX,REG_RSP, REG_RBP, REG_RSI, REG_RDI
} register_id;

typedef struct {
    operand_type type;
    register_id reg;
    long im;
    char label[32];
} operand;
typedef struct {
    char mnemonic[16];
    operand op1;
    operand op2;     
} instruction;
#endif //C_COMPILER_MASM_H
