#include "parse.h"

void parser_init(paarser *p, token *tokens, int count) {
    p->count = count;
    p->tokens = tokens;
    p->pos = 0;
}


astnode *parse_program(paarser *p);
astnode *parse_function(paarser *p);
astnode *parse_statement(paarser *p);
astnode *parse_expression(paarser *p);