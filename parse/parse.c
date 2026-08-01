#include "parse.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

void parser_init(paarser *p, token *tokens, int count) {
    p->count = count;
    p->tokens = tokens;
    p->pos = 0;
}
astnode *parse_program(paarser *p) {
    astnode temp;
    temp.type=NODE_PROGRAM;
    temp.function=parse_function(p);
    return &temp;
}
astnode *parse_function(paarser *p) {
    astnode temp;
    temp.type=NODE_FUNCTION;
    temp.name=p->tokens[p->pos].txt;
    while(p->tokens[p->pos].type!=TOKEN_LPAREN) {
        p->pos++;
    }
    temp.body=parse_statement(p);
    return &temp;
}
astnode *parse_statement(paarser *p) {
    p->pos++;
    astnode temp;
    temp.type=NODE_RETURN;
    temp.expr=parse_expression(p);
    return &temp;
}

astnode *parse_factor(paarser *p) {
    astnode *node = malloc(sizeof(astnode));
    if (p->tokens[p->pos].type == TOKEN_NUMBER) {
        node->type = NODE_CONSTANT;
        node->value = p->tokens[p->pos].val;
        p->pos++; // consume the number
        return node;
    }
    fprintf(stderr, "nigga i need a number\n");
}

astnode *parse_term(paarser *p) {
    astnode *left = parse_factor(p);
    while (p->tokens[p->pos].type == TOKEN_MUL ||
           p->tokens[p->pos].type == TOKEN_DIV ||
           p->tokens[p->pos].type == TOKEN_MOD) {
        tokentype op = p->tokens[p->pos].type;
        p->pos++;
        astnode *right = parse_factor(p);
        astnode *newnode = malloc(sizeof(astnode));
        newnode->type = NODE_BINARY_OP;
        newnode->op = op;
        newnode->left = left;
        newnode->right = right;
        left = newnode;
    }
    return left;
}

astnode *parse_expression(paarser *p) {
    astnode *left = parse_term(p);
    while (p->tokens[p->pos].type == TOKEN_ADD ||
           p->tokens[p->pos].type == TOKEN_SUB) {
        tokentype op = p->tokens[p->pos].type;
        p->pos++;
        astnode *right = parse_term(p);
        astnode *newnode = malloc(sizeof(astnode));
        newnode->type = NODE_BINARY_OP;
        newnode->op = op;
        newnode->left = left;
        newnode->right = right;
        left = newnode;
    }
    return left;
}