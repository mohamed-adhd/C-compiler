typedef enum{
    TOKEN_INT,
    TOKEN_RETURN,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_LPAREN,   
    TOKEN_RPAREN,   
    TOKEN_LBRACE,   
    TOKEN_RBRACE,   
    TOKEN_SEMICOLON,
    TOKEN_EOF
}tokentype;


typedef struct{
    tokentype type;
    char *txt;
    int val;
}token;