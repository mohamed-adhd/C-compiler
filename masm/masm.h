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
typedef enum{
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_COMMA,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_ver,
    TOKEN_NEWLINE,
    TOKEN_COLON,
    TOKEN_EOF
}tokentype;
typedef struct {
    const char *source;
    size_t position;
} asm_lexer;
typedef struct{
    tokentype type;
    char *txt;
    int val;
}token;
typedef struct {
    token *tokens;
    int pos;
    int count;
} asm_parser;
char peek(asm_lexer *l);
char advance(asm_lexer *l);
void skip_whitespace(asm_lexer *l);

typedef struct {
    operand_type type;
    register_id reg;
    long im;
    char label[32];
} operand;
typedef struct {
    int is_label;
    char label_name[32];
    instruction instr;
} asm_line;
typedef struct {
    char mnemonic[16];
    operand op1;
    operand op2;
} instruction;
#endif //C_COMPILER_MASM_H
