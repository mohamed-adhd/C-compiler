#include "codegen.h"
#include <fstream>

void codegen_program(astnode *program, FILE *out);
void codegen_function(astnode *function, FILE *out);
void codegen_statement(astnode *stmt, FILE *out);