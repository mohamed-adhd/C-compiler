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
    ASM_TOKEN_NUMBER,
    ASM_TOKEN_IDENTIFIER,
    ASM_TOKEN_COMMA,
    ASM_TOKEN_LBRACE,
    ASM_TOKEN_RBRACE,
    ASM_TOKEN_ver,
    ASM_TOKEN_NEWLINE,
    ASM_TOKEN_COLON,
    ASM_TOKEN_POINT,
    ASM_TOKEN_EOF
}asm_tokentype;
typedef struct {
    const char *source;
    size_t position;
} asm_lexer;
typedef struct{
    asm_tokentype type;
    char *txt;
    int val;
}asm_token;
typedef struct {
    asm_token *tokens;
    int pos;
    int count;
} asm_parser;
char asm_peek(asm_lexer *l);
char asm_advance(asm_lexer *l);
void asm_skip_whitespace(asm_lexer *l);
asm_token asm_next_token(asm_lexer *l);
asm_token *tokenizer(asm_lexer *l, int *count);
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
