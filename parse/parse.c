#include "parse.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

void parser_init(paarser *p, token *tokens, int count) {
    p->count = count;
    p->tokens = tokens;
    p->pos = 0;
}

static astnode *new_node(nodetp type) {
    astnode *node = calloc(1, sizeof(astnode));
    if (node != NULL) {
        node->type = type;
    }
    return node;
}

astnode *parse_program(paarser *p) {
    astnode *node = new_node(NODE_PROGRAM);
    node->function=parse_function(p);
    return node;
}
astnode *parse_function(paarser *p) {
    astnode *node = new_node(NODE_FUNCTION);
    while (p->pos + 2 < p->count &&
           !(p->tokens[p->pos].type == TOKEN_INT &&
             p->tokens[p->pos + 1].type == TOKEN_IDENTIFIER &&
             p->tokens[p->pos + 2].type == TOKEN_LPAREN)) {
        p->pos++;
    }
    if (p->pos + 1 < p->count) {
        node->name=p->tokens[p->pos + 1].txt;
    }
    while(p->pos < p->count && p->tokens[p->pos].type!=TOKEN_RETURN) {
        p->pos++;
    }
    node->body=parse_statement(p);
    return node;
}
astnode *parse_statement(paarser *p) {
    if (p->tokens[p->pos].type == TOKEN_RETURN) {
        p->pos++;
        astnode *node = new_node(NODE_RETURN);
        node->expr = parse_expression(p);
        p->pos++;
        return node;
    } else if (p->tokens[p->pos].type ==TOKEN_PRINT) {
        p->pos++;
        p->pos++;
        astnode *node = new_node(NODE_PRINT);
        node->expr = parse_expression(p);
        p->pos++;
        p->pos++;
        return node;
    }
    fprintf(stderr, "you fucked up.....\n");
    exit(1);
}

astnode *parse_factor(paarser *p) {
    astnode *node = new_node(NODE_CONSTANT);
    if (p->tokens[p->pos].type == TOKEN_NUMBER) {
        node->value = p->tokens[p->pos].val;
        p->pos++;
        return node;
    }
    fprintf(stderr, "Expected a number nigga \n");
    free(node);
    return NULL;
}

astnode *parse_term(paarser *p) {
    astnode *left = parse_factor(p);
    while (p->tokens[p->pos].type == TOKEN_MUL ||p->tokens[p->pos].type == TOKEN_MOD) {
        tokentype op = p->tokens[p->pos].type;
        p->pos++;
        astnode *right = parse_factor(p);
        astnode *newnode = new_node(NODE_BINARY_OP);
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
    while (p->tokens[p->pos].type == TOKEN_ADD ||p->tokens[p->pos].type == TOKEN_SUB) {
        tokentype op = p->tokens[p->pos].type;
        p->pos++;
        astnode *right = parse_term(p);
        astnode *newnode = new_node(NODE_BINARY_OP);
        newnode->type = NODE_BINARY_OP;
        newnode->op = op;
        newnode->left = left;
        newnode->right = right;
        left = newnode;
    }
    return left;
}
