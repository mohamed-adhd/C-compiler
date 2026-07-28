#include "token.h"
#include <cstddef>

typedef enum{
    NODE_PROGRAM,
    NODE_FUNCTION,
    NODE_RETURN,
    NODE_CONSTANT
}nodetp;


typedef struct
{
    const char *source;
    size_t position;
    size_t line;
    size_t column;
}Lexer;


typedef struct astnode astnode; 
struct astnode {
    nodetp type;
    char *name;      
    astnode *body;   
    astnode *expr;    
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