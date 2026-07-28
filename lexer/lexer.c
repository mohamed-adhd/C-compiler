
#include "lexer.h"


void lexer_init(Lexer l, const char* source){
    l.line=1;
    l.source=source;
    l.position=0;
}
void parser_init(paarser *p, token *tokens, int count){
    
}
astnode *parse_program(paarser *p);
astnode *parse_function(paarser *p);
astnode *parse_statement(paarser *p);
astnode *parse_expression(paarser *p);