#include "lexer/lexer.h"
typedef enum{
    NODE_PROGRAM,
    NODE_FUNCTION,
    NODE_RETURN,
    NODE_CONSTANT,
    NODE_ADD,
    NODE_SUB,
    NODE_MUL,
    NODE_DIV,
    NODE_MOD,
    NODE_BINARY_OP
}nodetp;


typedef struct astnode astnode; 
struct astnode {
    nodetp type;
    char *name;      
    astnode *body;   
    astnode *expr;
    astnode *left;
    astnode *right;
    tokentype op;
    int value;        
    astnode *function;
};
typedef struct{
    token *tokens;
    int pos;
    int count;
}paarser;


void parser_init(paarser *p, token *tokens, int count);
astnode *parse_program(paarser *p);
astnode *parse_function(paarser *p);
astnode *parse_statement(paarser *p);
astnode *parse_expression(paarser *p);