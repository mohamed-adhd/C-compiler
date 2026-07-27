#include "token.h"

typedef enum{
    NODE_PROGRAM,
    NODE_FUNCTION,
    NODE_RETURN,
    NODE_CONSTANT
}nodetp;

struct astnode {
    nodetp type;
    char *name;      
    astnode *body;   
    astnode *expr;    
    int value;        
    astnode *function;
};